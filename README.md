# C2B2_cluster_starter

Basic introductions to how to use C2B2 cluster (including multi-threading C/C++ and CUDA C/C++ programming). **Note** that this post itself is not a detailed tutorial (will cite you more detailed tutorials when necessary), but just a basic introduction to make you set up and start research works quickly.



## 1. Getting your account

[Here](http://wiki.c2b2.columbia.edu/arcs/index.php/Forms) you can find the application form. It's recommended to run well-coded programs and long jobs in cluster, other than in your own computer, since you will enjoy the benefits of submitting multiple jobs to clusters to run without waiting them to finish in front of your screen, and also save your computer's life.

Some instructions about how to fill in the application form:

* Principle Investigator: Dr. Itsik Pe'er
* Title -> simply student, or empty
* Office/Lab Room Numer --> CSB506, Morningside Campus
* Phone Number --> your phone number
* Positon: should be "Rotation Student" or "Casual"; start and end dates are the start and end of the current semester or summer
* ARCS/ICG Email Account - Yes/No/Forward only to Columbia Account --> you don't actually need this
* SharePoint Acccount - Yes/No --> you don't actually need this
* Cluster Account - Yes/No --> Yes!
* Scanner - Yes/No --> you don't actually need this
* Will work with sensitive data? Yes/No --> No
* Requestor - should be yourself

Please fill in this form and email as instructed also cc-ing Prof. Pe'er. He will approve and you will get your account. SGE is a good general skill, and it's worth the efforts to get familiar with it. [This](http://wiki.c2b2.columbia.edu/arcs/index.php/Cluster_doc) is a general instruction to our cluster.





## 2. Sun Grid Engine (SGE) basics


### 2.1. Working directory

You need to login to the cluster, landing at `home` directory, which is not supposed to be the working directory. Forget about the `data` directory. We are mainly working on `scratch`. Though it's reported less secure, but dropping all the scripts/codes/data there is what the lab usually does. So, work there.

e.g.
```
/ifs/home/c2b2/ip_lab/sy2515/
/ifs/data/c2b2/ip_lab/sy2515/
/ifs/scratch/c2b2/ip_lab/sy2515/
```

Independently with these, you can use `qlogin` (or `qlogin -l mem=32G`) to land on a computing node (with the same three directories), which supports computation to happen. This also means the login node doesn't support computation, but just some basic file editing operations (because you might not be able to get enough memory in the login node, but you can with command like `qlogin -l mem=32G`).

More frequently used commands in cluster: qlogin, qsub, qstat, qdel. Detailed manual for SGE is [here](http://gridscheduler.sourceforge.net/htmlman/manuals.html).


### 2.2. Job submission scripts

Find the following scripts for your reference. For a more detailed discussion (e.g., running an array job), see [here](http://wiki.c2b2.columbia.edu/arcs/index.php/Cluster_doc).

1. [run.sh](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run.sh) (vanilla)
2. [run_mt.sh](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_mt.sh) (multi-threading)
3. [run_gpu.sh](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh) (GPU)

Please pay attention to the system memory configuration, and be careful about the memory you are applying. The same for the time you apply. So estimate the approximate running time and memory requirement before submitting your jobs (or submit trial jobs to see how much memory and time is actually needed).


### 2.3. Account quota

The default quota for your `scratch` directory should be 20GB. This is enough for running slight jobs. But if it's necessary, we can upgrade that. Check your quota with `df -h /ifs/scratch/c2b2/ip_lab/sy2515`. Once your program reports something like `can't write to disk`, please check your left quota.


### 2.4. Questions?

Please send any question to "rt@c2b2.columbia.edu" **with your uni emai address** (they will filter out addresses like "shuo.yang@columbia.edu" and "shuo@cs.columbia.edu").



## 3. Local Python

[Canopy](https://www.enthought.com/products/canopy/) and [Anaconda](https://www.continuum.io/downloads) are suggested to install locally in `scratch` directory, to run Python program with various useful supported libraries. See [here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally#6-compile-and-run-c-program) for setting up the env var to call these locally installed Python.

Note that please switch to Anaconda if you find some libraries are not supported by Canopy.

Note that Anaconda is preferred, since it contains an add-on for GPU Python (see [here](https://developer.nvidia.com/how-to-cuda-python)). You need to `conda install accelerate` and `conda install numbapro` after you deploy your Anaconda.




## 4. Local GCC

See [here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally).




## 5. Multi-threading C++

Once you code up your project in C/C++, you might want to spend a little more time to move onto a multi-threading version of your code. From [here](http://wiki.c2b2.columbia.edu/arcs/index.php/Cluster_doc#Additional_notes_.26_examples) you might have already realized that our cluster supports multi-threading jobs and multi-processing jobs. We've tried both, but we don't recommend the later one, since the usual coherence of our learning algorithms will probably make this less advantageous (Gradient descent has temporal dependency, and Gibbs sampling has both temporal and spatial dependency, which makes these algorithms not suitable for working among cluster nodes). But multi-threading jobs are definitely something worth trying, and will probably give you 10 around speed-up directly compared to your sequential C/C++ (assuming 12 cores are allocated in cluster computing node).

Before we start parallel, step-0 should be making your vanilla sequential version of C/C++ well organized and structured. You don't need to force yourself to be OOP, but it will definitely help if you struct your code well for example making some core algorithms and functions in a separate module. This step will help all parallel techniques we have here (multi-threading, multi-processing and GPU).

A simple first-step instruction to parallel your sequential C/C++ is: to find the way we can parallel and get a sense of how faster we could be. This requires some experience, but it's definitely a learned skill. Practically when starting, you need to first of all get familiar with some distributed computing concepts, like synchronization, memory sharing, mutex, etc. Please find yourself tutorials for parallel computing, like [this one](https://computing.llnl.gov/tutorials/parallel_comp/).

Finally, to be more concrete, please find the example of multi-threading C++ jobs [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/tree/master/mt_example). Note: you might need a newer gcc installed in cluster locally (see [here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally)) in order to compile the code, as some new C++ libraries might be used in this code (not sure though).






## 6. Hadoop or Spark?

Unfortunately, the cluster doesn't support these. These require specific storage architecture and composition of the cluster and cannot be easily intergated. Pretty much they would have to buy different storage and build completely different new cluster to run Hadoop or Spark.




## 7. GPU (Nvidia CUDA)

1. Set up the env var as instructed above ([here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally#6-compile-and-run-c-program))
2. After setting up the env var, you get the nvcc compiler. Please note that, you can only compile CUDA C/C++ code in computing node, other than the login node. Also, note that after compiling, you can only run GPU jobs by submitting them into the GPU nodes (script as [run_gpu.sh](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh)), as you can't `qlogin` to a GPU node (GPU devices are in seperate queues at cluster nodes that are accessible only for submitted jobs but not `qlogin` users)
3. There are 44 GPU nodes, each having 3 GPU devices (also 3 CPU cores associated with these GPU cores). I attached the details of these GPU's for you to get a sense of them (in [run_gpu.sh.o9898871](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/blob/master/run_gpu.sh.o9898871))
4. Please compile and run [CUDA examples](http://docs.nvidia.com/cuda/cuda-samples/index.html#getting-cuda-samples) to get familiar with running GPU jobs (since the compilation of all examples will take a while, it's recommended to submit a job with `make` as the command to run). You might want to especially check the **matrix multiplication** samples since most algorithms involve this type of computation on large-scale dataset (CUBLAS is said to achieve the best performance for large-scale matrix multiplication). As said there are at most 3 GPU devices at one GPU node, you can set `#$ -l gpu=1` from 1 to 3, but no more (as observed, even if you apply 1 GPU, you can still access 3 of them in some way)
5. If you modify [CUDA examples](http://docs.nvidia.com/cuda/cuda-samples/index.html#getting-cuda-samples) in order to build some functions for your original C++ program, you might need some header files that are located in `./common/inc/` of the CUDA example folder
6. Please see [here](https://github.com/ComputationalBiology-CS-CU/C2B2_cluster_starter/tree/master/gpu_example) for an example (basic matrix multiplication) with some CUDA programming tips
7. Python GPU: as discussed above, please use [Anaconda](https://developer.nvidia.com/how-to-cuda-python), and you need to `conda install accelerate` and `conda install numbapro` after you deploy your Anaconda. Please refer to [this](https://github.com/morrisyoung/CUDA_Python_starter) for more comments on Python CUDA.






## 8. Other notes

1. Please send me (Shuo, shuo@cs.columbia.edu) your Github account such that I can add you to the Lab Git repo (**NOTE**: please only push code to the lab repo when it's runnable, otherwise please develop in your own repo first of all)
2. Please make sure to document well (progress, core algorithms and program structures) for your code (e.g., in README), to make it not only usable, but also modifiable to benefit other projects


