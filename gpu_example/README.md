I find [this](http://www.es.ele.tue.nl/~mwijtvliet/5KK73/?page=mmcuda) useful to lean various matrix multiplication optimization techniques (and by the way learn more about GPU hardware details).



GPU tips:

1. Use the right timer: cuda event api
2. Note the limit on the scale of the block, and on the scale of the grid (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L24), [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L25) and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L26))
3. Note the limit on the GPU memory (see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L10), and [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871#L20) since you might use share-memory optimization)
4. GPU doesn't report some run-time errors, which means you need to test the accuracy of your program by yourself
5. Please release GPU memory whenever you don't need them
6. Share-memory within a block is a good scheme to optimize the code

