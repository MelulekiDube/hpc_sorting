#!/bin/sh
#SBATCH —account=icts
#SBATCH —partition=curie
#SBATCH —nodes=2 —ntasks=8
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=4000
#SBATCH --job-name="omp"
#SBATCH --mail-user=dbxmel004@myuct.ac.za
#SBATCH --mail-type=BEGIN,END,FAIL
#module load mpi/mpich-3.3
#module load compilers/gcc820

##### Launch parallel job using srun
srun make run
echo 'Done'