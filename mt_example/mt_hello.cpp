#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5


using namespace std;


typedef struct thread_data{
	int  thread_id;
	char * message;		// heap (callor/malloc) to allocate, same for below
	float * data;
}thread_data;


void * WorkThread(void * threadarg)
{
	thread_data * my_data = (thread_data *) threadarg;
	cout << "Thread ID: " << my_data->thread_id << endl;
	sleep(1);

	pthread_exit(NULL);
}


int main ()
{
	int rc;
	int i;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void * status;

	// init and set joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// thread data prepare
	thread_data list_package[NUM_THREADS];
	for(int i=0; i < NUM_THREADS; i++)
	{
		thread_data my_data;
		my_data.thread_id = i;
		list_package[i] = my_data;
	}

	// init and release threads
	for(int i=0; i < NUM_THREADS; i++)
	{
		cout << "main(): creating thread, " << i << endl;
		rc = pthread_create(&threads[i], NULL, WorkThread, (void *)&list_package[i]);
		if (rc)
		{
			cout << "Error: unable to create thread," << rc << endl;
			exit(-1);
		}
	}

	// delete thread and wait
	pthread_attr_destroy(&attr);
	for(int i=0; i < NUM_THREADS; i++)
	{
		rc = pthread_join(threads[i], &status);
		if (rc)
		{
			cout << "Error: unable to join," << rc << endl;
			exit(-1);
		}
		cout << "Main: completed thread id:" << i;
		cout << " exiting with status:" << status << endl;
	}


	cout << "Main: program exiting." << endl;
	pthread_exit(NULL);
}

