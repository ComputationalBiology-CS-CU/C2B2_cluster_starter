#!/bin/bash
#$ -cwd -S /bin/bash -j y
## there are maximum 3 CPU cores associated with the maximum 3 GPU devices
##$ -pe smp 3
#$ -l mem=20G,time=40::
## the following can be set to 1-3, as there are at most 3 GPU devices at one GPU node
#$ -l gpu=1
#$ -M sy2515@c2b2.columbia.edu -m bes


# the program to run
#./saxpy
./train_gpu
