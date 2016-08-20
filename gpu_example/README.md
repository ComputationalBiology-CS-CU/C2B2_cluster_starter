GPU tips:

1. Use the right timer: cuda event api
2. Note the limit on the scale of the block, and on the scale of the grid (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L24), [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L25) and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L26))
3. Note the limit on the GPU memory (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L10), and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L20) since you might use share-memory optimization)
4. GPU doesn't report some run-time errors, which means you need to test the accuracy of your program by yourself


