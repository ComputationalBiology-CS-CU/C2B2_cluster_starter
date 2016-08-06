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



using namespace std;



// local global variables
pthread_mutex_t mut;			// mutex used by all the threads
int * finish_table;				// finish table for all the samples in this batch ()
int size_batch = 30;			// TODO
int number_thread = 12;			// TODO


//==== the data and the model
float * data_x;
float * data_y;
float * data_amount = 1000;

float slope = 3.0;					// TODO
float slope_learned = 0.0;
float intercept = 5.0;				// TODO
float intercept_learned = 0.0;

float alpha = 0.1;					// TODO: learning rate
int iteration = 100;				// TODO





typedef struct package_thread
{
	int id;
	float * derivative;
	// and other information you want to pass to each thread work though this pointer
}package_thread;





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
	// allocate the memory for this thread (this is independent with other threads, but this should be visiable to the main thread -- aggregation)
	package_alloc(package_pointer);

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




	}

	pthread_exit(NULL);
}





void opt_mt_control()
{
	cout << "[@@@] entering the current mini-batch (in multi-treading mode)..." << endl;


	//==== the target
	float * derivative = calloc(2, sizeof(float));


	//=============================== multi-threading parameter initialization ===============================
	// allocating all the other threads from here
	pthread_t threads[num_thread];
	void * status;
	pthread_attr_t attr;
	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//
	finish_table = (int *)calloc(size_batch, sizeof(int));
	//
	pthread_mutex_init(&mut, NULL);
	memset(&threads, 0, sizeof(threads));



	//=============================== thread local memory preparation (not yet allocation heap) ===============================
	package_thread para_array[num_thread];
	for(int i=0; i<num_thread; i++)
	{
		package_dev package;
		package.id = i;
		// allocate value space in thread work --> to save time here
		para_array[i] = package;
	}



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
	// free attribute and wait for the other threads
	aggregation_init(derivative);
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
		aggregation_add(&para_array[i], &derivative);
	}
	aggregation_ave(&derivative);



	//===================== regularize, gradient descent, and release space =====================
	/// gradient descent
	gradient_descent(derivative);


	//// free the memory allocated for these threads
	for(int i=0; i<num_thread; i++)
	{
		package_free(&para_array[i]);
	}


	//===================== finish and quit =====================
	free(finish_table);
	cout << "[@@@] finishing the current mini-batch (in multi-treading mode)..." << endl;
}




// set the deravetives to 0
void aggregation_init(float * derivative)
{
	derivative_pointer[0] = 0;
	derivative_pointer[1] = 0;
}


// add the results from one thread into the final repo
void aggregation_add(package_thread * para_array_pointer, float * derivative)
{
	derivative[0] += para_array_pointer->derivative[0];
	derivative[1] += para_array_pointer->derivative[1];
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




float cal_error()
{
	float error_total;
	for(int i=0; i<data_amount; i++)
	{
		error = (x[i] * slope_learned + intercept_learned) - y[i];
		error = pow(error, 2.0);
		error_total += error;
	}
	return error_total;
}



int main()
{

	//======== simulate the data
	float * data_x = malloc();
	float * data_y = malloc();


	// init the slope_learned and intercept_learned
	slope_learned = 1.0;
	intercept_learned = 1.0;


	//======== opt
	for(int i=0; i<iteration; i++)
	{
		cout << "working on iter#" << i << endl;
		opt_mt_control();
		float error = cal_error();
		cout << "real model:" << slope << intercept << endl;
		cout << "current learned model:" << slope_learned << intercept_learned << endl;
		cout << "current error:" << error << endl;
	}





	//======== report
	cout << "the real slope and intercept are " << slope << ", " << intercept << endl;
	cout << "the learned slope and intercept are " << slope_learned << ", " << intercept_learned << endl;


	return 0;
}

