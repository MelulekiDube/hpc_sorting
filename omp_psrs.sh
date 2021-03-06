#!/bin/sh
#SBATCH —account=icts
#SBATCH —partition=curie
#SBATCH —nodes=2 —ntasks=8
#SBATCH --time=30:00
#SBATCH --mem-per-cpu=4000
#SBATCH --job-name="mpiqsrt"
#SBATCH --mail-user=dbxmel004@myuct.ac.za
#SBATCH --mail-type=BEGIN,END,FAIL

#module load mpi/mpich-3.3
#module load compilers/gcc820
##### Launch parallel job using srun
export OMP_NUM_THREADS=4
module load mpi/mpich-3.3
module load compilers/gcc820 
dir_n="mpi_results"
j=0
for filename in ./test_data/*; do
	cp $filename ./current_test
	srun make omp_psrs
done
echo 'Done'