#!/bin/sh
#SBATCH --account=icts
#SBATCH --partition=curie
#SBATCH --nodes=2 --ntasks=4
#SBATCH --time=59:00
#SBATCH --mem-per-cpu=1000
#SBATCH --job-name="mpiqsrt"
#SBATCH --mail-user=dbxmel004@myuct.ac.za
#SBATCH --mail-type=BEGIN,END,FAIL

module load mpi/mpich-3.3
module load compilers/gcc820
##### Launch parallel job using srun
mkdir -p mpi_results
dir_n="mpi_results"
j=0
make mpi_sort
for filename in ./test_data/*; do
	cp $filename ./current_test
	fname="$dir_n/$j"
	srun -n4 --mpi=pmi2 ./bin/mpi_sort.out>"$fname"
	j=$((j+1))
done
echo 'Done'
