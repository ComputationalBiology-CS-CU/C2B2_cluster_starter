Courses: [here](http://cs.nyu.edu/courses/fall15/CSCI-GA.3033-004/), [here](http://courses.cms.caltech.edu/cs179/), [here](https://developer.nvidia.com/educators/existing-courses) and Google by yourself...

I find [this](http://www.es.ele.tue.nl/~mwijtvliet/5KK73/?page=mmcuda) useful to lean various matrix multiplication optimization techniques (and by the way learn more about GPU hardware details).

Since [cuBLAS](https://developer.nvidia.com/cublas) is more convenient to use for some BLAS routines and also deeply optimized on top of CUDA, I also attach a slightly modified cuBLAS matrix multiplication [example](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/tree/master/gpu_example/matrixMulCUBLAS) (from the [CUDA samples](http://docs.nvidia.com/cuda/cuda-samples/index.html#getting-cuda-samples)) in this dir. See [here](https://developer.nvidia.com/sites/default/files/akamai/cuda/files/Misc/mygpu.pdf) for a useful doc introducing cuBLAS routines in more details.


GPU tips:

1. Use the right timer: cuda event api
2. Note the limit on the scale of the block, and on the scale of the grid (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L24), [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L25) and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L26))
3. Note the limit on the GPU memory (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L10), and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L20) since you might use share-memory optimization)
4. GPU doesn't report some run-time errors, which means you need to test the accuracy of your program by yourself
5. Please release GPU memory whenever you don't need them
6. Share-memory within a block is a good scheme to optimize the code


