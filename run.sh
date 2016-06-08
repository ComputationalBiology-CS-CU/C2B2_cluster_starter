#!/bin/bash
#$ -cwd -S /bin/bash -j y
#$ -l mem=32G,time=10::
#$ -M sy2515@c2b2.columbia.edu -m bes

## set up the LD path
export LD_LIBRARY_PATH=/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-gcc-4.9.1/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-cloog-0.18.1/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-gmp-4.3.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-isl-0.12.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-mpfr-2.4.2/lib:/ifs/scratch/c2b2/ip_lab/sy2515/HPC/shuo-mpc-0.8.1/lib:/opt/gridengine/hpc/lib/lx-amd64


## set up the Enthought path
##export PATH=/ifs/scratch/c2b2/ip_lab/sy2515/Canopy/appdata/canopy-1.5.2.2785.rh5-x86_64/bin:$PATH



./train
##python para_init_tissuev.py

