// the control platform for multi-threading program

// use a linear regression (slope and intercept unknown) example to demonstrate the mt procedure
//	--> this is shortened from training neural network, in which extensive backpropogation is used



#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <math.h>				// /* exp */
#include <cstdlib>				// for mt
#include <pthread.h>			// for mt
#include <string.h>
#include <sys/time.h>
#include <vector>




using namespace std;




// local global variables
pthread_mutex_t mut;				// mutex used by all the threads
int num_thread = 12;				// TODO




// the data, the model, and the learning
//float * data_x;					// NOTE: we can only allocate these in opt_mt_control to make it global to all threads
//float * data_y;
int data_amount = 1000;

float slope = 3.0;					// TODO
float slope_learned = 0.0;
float intercept = 100.0;			// TODO
float intercept_learned = 0.0;

float alpha = 0.000001;				// TODO: learning rate
int iteration = 2000;				// TODO
int size_batch = 50;				// TODO
int * finish_table;					// finish table for all the samples in this batch ()
int * list_index;					//




// thread package
typedef struct package_thread
{
	int id;
	float * derivative;
	float * data_x;
	float * data_y;
	// and other information you want to pass to each thread work though this pointer
}package_thread;






// function declare (if necessary)
void package_alloc(package_thread *);
void package_free(package_thread *);
void * WorkPerThread(void *);
void opt_mt_control(float *, float *);
void aggregation_init(float *);
void aggregation_add(package_thread *, float *);
void aggregation_ave(float *);
void gradient_descent(float *);
float cal_error(float *, float *);
void rtrim(char *);
void ltrim(char *);
void trim(char *);







// initialize all the parameter space
void package_alloc(package_thread * package_pointer)
{
	(package_pointer->derivative) = (float *)calloc(2, sizeof(float));
}


// to release the above space
void package_free(package_thread * package_pointer)
{
	free(package_pointer->derivative);
}


// this is the working program for each thread
void * WorkPerThread(void * pointer)
{
	package_thread * package_pointer = (package_thread *)pointer;
	float * data_x = package_pointer->data_x;
	float * data_y = package_pointer->data_y;

	// allocate the memory for this thread (this is independent with other threads, but this should be visiable to the main thread -- aggregation)
	package_alloc(package_pointer);
	(package_pointer->derivative)[0] = 0;
	(package_pointer->derivative)[1] = 0;


	while(1)
	{
		int count = -1;
		//================ check whether there are still left samples to be processed ================
		// if there are, take into this thread; otherwise, terminate this thread
		pthread_mutex_lock(&mut);
		for(int i=0; i<size_batch; i++)
		{
			if(finish_table[i] == 0)
			{
				count = i;
				finish_table[i] = 1;
				break;
			}
		}
		pthread_mutex_unlock(&mut);

		if(count == -1)  // no left samples for processing
		{
			break;
		}


		//================ work on the current sample ================
		// calculate the derivative (2), and add to container
		int index = list_index[count];
		float x = data_x[index];
		float y = data_y[index];

		(package_pointer->derivative)[0] += 2 * (slope_learned * x + intercept_learned - y) * x;
		(package_pointer->derivative)[1] += 2 * (slope_learned * x + intercept_learned - y);

	}


	pthread_exit(NULL);
}





void opt_mt_control(float * data_x, float * data_y)
{
	//==== the target
	float * derivative = (float *)calloc(2, sizeof(float));



	//=============================== multi-threading parameter initialization ===============================
	// allocating all the other threads from here
	pthread_t threads[num_thread];
	void * status;
	pthread_attr_t attr;
	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	// set mutex
	pthread_mutex_init(&mut, NULL);
	memset(&threads, 0, sizeof(threads));



	//=============================== thread local memory preparation (not yet allocation heap) ===============================
	package_thread para_array[num_thread];
	for(int i=0; i<num_thread; i++)
	{
		package_thread package;
		package.id = i;
		// allocate value space in thread work --> to save time here
		package.data_x = data_x;
		package.data_y = data_y;
		para_array[i] = package;
	}



	//=============================== prepare the list_index, and the mt control list ===============================
	list_index = (int *)calloc(size_batch, sizeof(int));
	// TODO: random sample size_batch indices from [0, data_amount)
	for(int i=0; i<size_batch; i++)
	{
		int index = rand() % data_amount;
		list_index[i] = index;				// NOTE: may have repeated index
	}
	finish_table = (int *)calloc(size_batch, sizeof(int));



	//=============================== thread initialization ===============================
	for(int i=0; i<num_thread; i++)
	{
		cout << "main() : creating thread#" << i+1 << endl;
		int rc = pthread_create(&threads[i], NULL, WorkPerThread, (void *)&para_array[i]);
		if(rc)
		{
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
	}



	//===================== waiting for all the threads to terminate, and aggregate =====================
	aggregation_init(derivative);
	// free attribute and wait for the other threads
	pthread_attr_destroy(&attr);
	for(int i=0; i<num_thread; i++)
	{
		int rc = pthread_join(threads[i], &status);
		if(rc)
		{
			cout << "Error:unable to join " << rc << endl;
			exit(-1);
		}
		cout << "Main: completed thread#" << i+1;
		cout << " exiting with status: " << status;
		cout << ", aggregate it's results..." << endl;

		// aggregate the results from this current thread
		aggregation_add(&para_array[i], derivative);
	}
	aggregation_ave(derivative);



	//=============================== destroy the list_index, and mt control list ===============================
	free(list_index);
	free(finish_table);



	//===================== regularize, gradient descent, and release space =====================
	/// gradient descent
	gradient_descent(derivative);



	//===================== finish and quit =====================
	//// free the memory allocated for these threads
	for(int i=0; i<num_thread; i++)
	{
		package_free(&para_array[i]);
	}
	free(derivative);
	cout << "[@@@] finishing the current mini-batch (in multi-treading mode)..." << endl;
}




// set the deravetives to 0
void aggregation_init(float * derivative)
{
	derivative[0] = 0;
	derivative[1] = 0;
}


// add the results from one thread into the final repo
void aggregation_add(package_thread * para_array_pointer, float * derivative)
{
	derivative[0] += (para_array_pointer->derivative)[0];
	derivative[1] += (para_array_pointer->derivative)[1];
}


// average the summed results from all threads
void aggregation_ave(float * derivative)
{
	derivative[0] = derivative[0] / size_batch;
	derivative[1] = derivative[1] / size_batch;
}


void gradient_descent(float * derivative)
{
	slope_learned = slope_learned - alpha * derivative[0];
	intercept_learned = intercept_learned - alpha * derivative[1];
}




float cal_error(float * data_x, float * data_y)
{
	float error_total;
	for(int i=0; i<data_amount; i++)
	{
		float error = (slope_learned * data_x[i] + intercept_learned) - data_y[i];
		error = pow(error, 2.0);
		error_total += error;
	}

	return error_total;
}




int main()
{

	//======== simulate the data
	float * data_x = (float *)calloc(data_amount, sizeof(float));
	float * data_y = (float *)calloc(data_amount, sizeof(float));

	//== load data_x
	char filename[100];
	filename[0] = '\0';
	strcat(filename, "./data_x.txt");
	puts("data_x, the current file worked on is:");
	puts(filename);

	FILE * file_in = fopen(filename, "r");
	if(file_in == NULL)
	{
		fputs("File error\n", stderr); exit (1);
	}
	int input_length = 100;
	char input[input_length];
	int counter = 0;
	while(fgets(input, input_length, file_in) != NULL)
	{
		trim(input);

		float dosage = stof(input);
		data_x[counter] = dosage;
		counter += 1;
	}
	fclose(file_in);

	//== load data_y
	//char filename[100];
	filename[0] = '\0';
	strcat(filename, "./data_y.txt");
	puts("data_y, the current file worked on is:");
	puts(filename);

	//FILE * file_in = fopen(filename, "r");
	file_in = fopen(filename, "r");
	if(file_in == NULL)
	{
		fputs("File error\n", stderr); exit (1);
	}
	//int input_length = 100;
	//char input[input_length];
	//int counter = 0;
	counter = 0;
	while(fgets(input, input_length, file_in) != NULL)
	{
		trim(input);

		float dosage = stof(input);
		data_y[counter] = dosage;
		counter += 1;
	}
	fclose(file_in);



	// init the slope_learned and intercept_learned
	float r = 0.0;
	r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));		// NOTE: 10 is suggested based on real slope 3.0
	slope_learned = r;
	r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/200));	// NOTE: 200 is suggested based on real intercept 100
	intercept_learned = r;
	cout << "init slope_learned and intercept_learned: " << slope_learned << " " << intercept_learned << endl;



	vector<float> error_list;
	//======== opt
	for(int i=0; i<iteration; i++)
	{
		cout << "working on iter#" << i << endl;
		opt_mt_control(data_x, data_y);
		float error = cal_error(data_x, data_y);
		cout << "real model:" << slope << ", " << intercept << endl;
		cout << "current learned model:" << slope_learned << ", " << intercept_learned << endl;
		cout << "current error:" << error << endl;

		error_list.push_back(error);
	}


	// save error list
	filename[0] = '\0';
	strcat(filename, "./error_list.txt");
    FILE * file_out = fopen(filename, "w+");
    if(file_out == NULL)
    {
        fputs("File error\n", stderr); exit(1);
    }
    for(int i=0; i<error_list.size(); i++)
    {
		char buf[100];
    	sprintf(buf, "%f\n", error_list[i]);
    	fwrite(buf, sizeof(char), strlen(buf), file_out);
    }
	fclose(file_out);



	//======== report
	cout << "the real slope and intercept are " << slope << ", " << intercept << endl;
	cout << "the final learned slope and intercept are " << slope_learned << ", " << intercept_learned << endl;


	return 0;
}




void rtrim(char * str)
{
	size_t n;
	n = strlen(str);
	while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\t' || str[n-1] == '\n'))
	{
		n--;
	}
	str[n] = '\0';
}

void ltrim(char * str)
{
	while (str[0] != '\0' && (str[0] == ' ' || str[0] == '\t'))
	{
		str++;
	}
}

void trim(char * str)
{
  rtrim(str);
  ltrim(str);
}


