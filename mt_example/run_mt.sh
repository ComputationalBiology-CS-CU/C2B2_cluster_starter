#!/bin/bash
#$ -cwd -S /bin/bash -j y
#$ -pe smp 12
#$ -l mem=2G,time=5::
#$ -M sy2515@c2b2.columbia.edu -m bes



./mt_train
#./mt_hello

