#!/bin/bash 
#SBATCH --job-name=spotify-kmeans 
#SBATCH --time=0-0:5
#SBATCH --cpus-per-task=8
#SBATCH --hint=compute_bound
export OMP_NUM_THREADS=1

./main 2500