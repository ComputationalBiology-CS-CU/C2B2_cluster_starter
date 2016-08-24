#!/bin/bash
#$ -cwd -S /bin/bash -j y
#$ -l mem=32G,time=20::
#$ -l gpu=3
#$ -M sy2515@c2b2.columbia.edu -m bes


./matrixMulCUBLAS
