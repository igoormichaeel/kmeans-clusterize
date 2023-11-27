#!/bin/bash 
#SBATCH --job-name=spotify-kmeans 
#SBATCH --time=0-2:59
#SBATCH --cpus-per-task=32
#SBATCH --hint=compute_bound
#SBATCH --exclusive

pascalanalyzer ./main -c 1,2 -i 2500 -o pascalanalyzer_output.json