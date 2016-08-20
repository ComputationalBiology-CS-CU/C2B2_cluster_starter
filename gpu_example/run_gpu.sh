#!/bin/bash
#$ -cwd -S /bin/bash -j y
##$ -pe smp 3
#$ -l mem=10G,time=20::
#$ -l gpu=3
#$ -M sy2515@c2b2.columbia.edu -m bes


./main

