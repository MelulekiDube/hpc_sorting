#!/bin/sh
#SBATCH --account=icts
#SBATCH --partition=curie
#SBATCH --nodes=1 --ntasks=4
#SBATCH --time=120:00
#SBATCH --mem-per-cpu=1000
#SBATCH --job-name="omp"
#SBATCH --mail-user=dbxmel004@myuct.ac.za
#SBATCH --mail-type=BEGIN,END,FAIL
export OMP_NUM_THREADS=8 
module load mpi/mpich-3.3
module load compilers/gcc820
make
srun make run
echo 'Done'
