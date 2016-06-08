#!/bin/bash
#$ -cwd -S /bin/bash -j y
#$ -pe smp 8
#$ -l mem=4G,time=5::
#$ -M sy2515@c2b2.columbia.edu -m bes

## setting up the LD path
export LD_LIBRARY_PATH=/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-gcc-4.9.1/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-cloog-0.18.1/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-gmp-4.3.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-isl-0.12.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-mpfr-2.4.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-mpc-0.8.1/lib:/opt/gridengine/hpc/lib/lx-amd64


./train
