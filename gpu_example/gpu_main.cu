// System includes
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

// CUDA runtime
#include <cuda_runtime.h>

// Helper functions and utilities to work with CUDA
#include <helper_functions.h>

// private
#include "gpu_main.h"
#include "gpu_func.cuh"




using namespace std;





// Allocates a matrix with random float entries.
void randomInit(float * data, int size)
{
    for (int i = 0; i < size; ++i)
        data[i] = rand() / (float)RAND_MAX;
}




/**
 * Run a simple test of matrix multiplication using CUDA
 */
int matrixMultiply(int block_size, dim3 &dimsA, dim3 &dimsB)
{
	// Allocate host memory for matrices A and B
	unsigned int size_A = dimsA.x * dimsA.y;
	unsigned int mem_size_A = sizeof(float) * size_A;
	float * h_A = (float *)malloc(mem_size_A);
	unsigned int size_B = dimsB.x * dimsB.y;
	unsigned int mem_size_B = sizeof(float) * size_B;
	float *h_B = (float *)malloc(mem_size_B);


	// Initialize host memory
	/* initialize random seed: */
	srand (time(NULL));
	randomInit(h_A, size_A);
	randomInit(h_B, size_B);


	// Allocate device memory
	float *d_A, *d_B, *d_C;


	// Allocate host matrix C
	dim3 dimsC(dimsB.x, dimsA.y, 1);
	unsigned int mem_size_C = dimsC.x * dimsC.y * sizeof(float);
	float * h_C = (float *) malloc(mem_size_C);


	// Allocate device memory
	cudaMalloc((void **) &d_A, mem_size_A);
	cudaMalloc((void **) &d_B, mem_size_B);
	cudaMalloc((void **) &d_C, mem_size_C);


	// copy host memory to device
	cudaMemcpy(d_A, h_A, mem_size_A, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, mem_size_B, cudaMemcpyHostToDevice);


	// Setup execution parameters
	dim3 threads(block_size, block_size);
	dim3 grid(dimsB.x / threads.x, dimsA.y / threads.y);


	// Create and start timer
	printf("Computing result using CUDA Kernel...\n");

	// Performs warmup operation using matrixMul CUDA kernel
	if (block_size == 16)
	{
		matrixMulCUDA<16><<< grid, threads >>>(d_C, d_A, d_B, dimsA.x, dimsB.x);
	}
	else
	{
		matrixMulCUDA<32><<< grid, threads >>>(d_C, d_A, d_B, dimsA.x, dimsB.x);
	}

	printf("warmup done...\n");

	cudaDeviceSynchronize();



	// Allocate CUDA events that we'll use for timing
	cudaEvent_t start;
	cudaEventCreate(&start);
	cudaEvent_t stop;
	cudaEventCreate(&stop);
	// Record the start event
	cudaEventRecord(start, NULL);

	// Execute the kernel
	int nIter = 300;
	for (int j = 0; j < nIter; j++)
	{
		if (block_size == 16)
		{
			matrixMulCUDA<16><<< grid, threads >>>(d_C, d_A, d_B, dimsA.x, dimsB.x);
		}
		else
		{
			matrixMulCUDA<32><<< grid, threads >>>(d_C, d_A, d_B, dimsA.x, dimsB.x);
		}
	}

	// Record the stop event
	cudaEventRecord(stop, NULL);
	// Wait for the stop event to complete
	cudaEventSynchronize(stop);
	float msecTotal = 0.0f;
	cudaEventElapsedTime(&msecTotal, start, stop);


	// Compute and print the performance
	float msecPerMatrixMul = msecTotal / nIter;
	double flopsPerMatrixMul = 2.0 * (double)dimsA.x * (double)dimsA.y * (double)dimsB.x;
	double gigaFlops = (flopsPerMatrixMul * 1.0e-9f) / (msecPerMatrixMul / 1000.0f);
	printf(
		"Performance= %.2f GFlop/s, Time= %.3f msec, Size= %.0f Ops, WorkgroupSize= %u threads/block\n",
		gigaFlops,
		msecPerMatrixMul,
		flopsPerMatrixMul,
		threads.x * threads.y);



	// Copy result from device to host
	cudaMemcpy(h_C, d_C, mem_size_C, cudaMemcpyDeviceToHost);


	// result check
	printf("Checking computed result for correctness: ");
	bool correct = true;

	// test relative error by the formula
	//     |<x, y>_cpu - <x,y>_gpu|/<|x|, |y|>  < eps
	double eps = 1.e-6 ; // machine zero

	// calculate the reference from CPU
	float * ref_C = (float *)malloc(mem_size_C);
	for(int i=0; i<dimsA.y; i++)
	{
		for(int j=0; j<dimsB.x; j++)
		{
			// cal ref_C[i * dimsB.x + j]
			float value = 0;
			for(int k=0; k<dimsA.x; k++)
			{
				value += h_A[dimsA.x * i + k] * h_B[dimsB.x * k + j];
			}
			ref_C[i * dimsB.x + j] = value;
		}
	}

	// compare
	for (int i = 0; i < (int)(dimsC.x * dimsC.y); i++)
	{
		double abs_err = fabs(h_C[i] - ref_C[i]);
		double dot_length = dimsA.x;
		double abs_val = fabs(h_C[i]);
		double rel_err = abs_err/abs_val/dot_length;

		if (rel_err > eps)
		{
			printf("Error! Matrix[%05d]=%.8f, ref=%.8f error term is > %E\n", i, h_C[i], ref_C[i], eps);
			correct = false;
		}
	}

	printf("%s\n", correct ? "Result = PASS" : "Result = FAIL");


	// Clean up memory
	free(h_A);
	free(h_B);
	free(h_C);
	free(ref_C);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	printf("\nNOTE: The CUDA Samples are not meant for performance measurements. Results may vary when GPU Boost is enabled.\n");

	// cudaDeviceReset causes the driver to clean up all state. While
	// not mandatory in normal operation, it is good practice.  It is also
	// needed to ensure correct operation when the application is being
	// profiled. Calling cudaDeviceReset causes all profile data to be
	// flushed before the application exits
	cudaDeviceReset();

	if (correct)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}



/**
 * GPU Program main
 */
void gpu_main()
{
	printf("[Matrix Multiply Using CUDA] - Starting...\n");

	int block_size = 32;										// 1024 (32*32) is the current block size limit

	dim3 dimsA(5*2*block_size, 5*2*block_size, 1);				// TODO: change scales of input
	dim3 dimsB(5*4*block_size, 5*2*block_size, 1);				// TODO: change scales of input
	printf("MatrixA(%d,%d), MatrixB(%d,%d)\n", dimsA.x, dimsA.y, dimsB.x, dimsB.y);

	int matrix_result = matrixMultiply(block_size, dimsA, dimsB);

	cout << "MM func result: ";
	cout << matrix_result << endl;

	return;
}


