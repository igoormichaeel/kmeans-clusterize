#!/bin/bash 
#SBATCH --job-name=spotify-kmeans 
#SBATCH --time=0-2:59
#SBATCH --cpus-per-task=32
#SBATCH --hint=compute_bound
#SBATCH --exclusive

pascalanalyzer ./main -c 2,4 -i 2500 -o pascalanalyzer_output2.json