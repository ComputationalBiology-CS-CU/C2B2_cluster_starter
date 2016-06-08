# C2B2_cluster_starter

Basic introductions to how to use C2B2 cluster (including CUDA C/C++ program)



## 1. Getting your account

[Here](http://wiki.c2b2.columbia.edu/arcs/index.php/Forms) you can find the application form. It's recommended to run jobs in cluster, other than in your own computer.

Some instructions about how to fill in the application form:

1. Principle Investigator: Dr. Itsik Pe'er
2. Title -> simply student, or empty
3. Office/Lab Room Numer --> CSB506, Morningside Campus
4. Phone Number --> your phone number
5. Positon: should be "Rotation Student" or "Casual"; start and end dates are the start and end of the current semester or summer
6. ARCS/ICG Email Account - Yes/No/Forward only to Columbia Account --> you don't actually need this
7. SharePoint Acccount - Yes/No --> you don't actually need this
8. Cluster Account - Yes/No --> Yes!
9. Scanner - Yes/No --> you don't actually need this
10. Will work with sensitive data? Yes/No --> No
11. Requestor - should be yourself

Please fill in this form and email as instructed also cc-ing Prof. Pe'er. He will approve and you will get your account. SGE is a good general skill, and it's worth the efforts to get familiar with it. [This](http://wiki.c2b2.columbia.edu/arcs/index.php/Cluster_doc) is a general instruction to our cluster.





## 2. Sun Grid Engine (SGE) basics


### 2.1. Login

You need to login to the cluster, landing at "home" directory, which is not supposed to be the working directory. Forget about the "data" directory. We are mainly working on "scratch". Though it's reported less secure, but dropping all the scripts/codes/data there is what the lab usually do. So, work there.

e.g.
```
/ifs/home/c2b2/ip\_lab/sy2515/
/ifs/data/c2b2/ip\_lab/sy2515/
/ifs/scratch/c2b2/ip\_lab/sy2515/
```

Independently with these, you can use "qlogin" (or "qlogin -l mem=32G") to land on a computing node, which supports computation to happen. This also means the login node doesn't support computation, but just some basic file editing operations.

More frequently used commands in cluster: qlogin, qsub, qstat, qdel

### 2.2. Login

The default quota for your "scratch" directory should be 20GB. This is enough for running slight jobs. But if it's necessary, we can upgrade that. Check your quota with "df -h /ifs/scratch/c2b2/ip\_lab/sy2515". Once your program reports something like "can't write to disk", please check the left quota (this is a problem troubled me some time in the past).

### 2.3. Python

[Canopy](https://www.enthought.com/products/canopy/) and [Anaconda](https://www.continuum.io/downloads) are suggested to install locally in "scratch" directory, to run Python program. See [here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally#6-compile-and-run-c-program) for setting up the env to call these locally installed Python.

### 2.4. Job submission scripts

Find the following scripts for your reference. For a more detailed discussion (e.g., running an array job), see [here](http://wiki.c2b2.columbia.edu/arcs/index.php/Cluster_doc).

1. run.sh (vanilla)
2. run\_mt.sh (multi-threading)
3. run\_gpu.sh (GPU)


Please pay attention to the system memory configuration, and be careful about the memory you are applying. Also, short-time jobs are easier to be allocated, so estimate the running time and submit jobs with less time requirement (the "time" option in the script).


### 2.5. Questions?

Please send any question to "rt@c2b2.columbia.edu".






## 3. Install GCC locally

See [here](https://github.com/ComputationalBiology-CS-CU/gcc_install_locally).






## 4. GPU

1. Set up the env as instructed above.

2. After setting up the env, you get the nvcc compiler. Please note that, you can only compile CUDA C/C++ code in computing node, other than the login node. Also, note that after compiling, you can only run GPU jobs by submitting them into the GPU nodes (script as above), as you can't "qlogin" to a GPU node.

3. There are 44 GPU nodes, each having 3 GPU devices. I attached the details of these GPU's for you to get a sense of them (in "run\_gpu.sh.o9898871").

4. Please compile and run [CUDA samples](http://docs.nvidia.com/cuda/cuda-samples/index.html#getting-cuda-samples) to get familiar with running GPU jobs. As said there are at most 3 GPU devices at one GPU node, you can set "#$ -l gpu=1" from 1 to 3. but no more.






## 5. Other notes

1. Please send me you Github account such that I can add you to the Lab repo

2. Please make sure to document well (progress, core algorithms and program structures) for your code (e.g., in README), to make it not only usable, but also modifiable to benefit other projects




