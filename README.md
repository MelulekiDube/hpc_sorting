# hpc_sorting
Implementation of serial quick sort, parallel quick sort and parallelsorting algortihm

#Project structure</ln>
├── include/<br/>
├── src/ <br/>
├── test_data/ <br/>
├── include/ <br/>
├── Makefile <br/>
├── README <br/>
├── script files


# Running the project
To run the project one would require to use the Makefile

The reles provided are:
- ```make``` to compile everything except the parallel regular sort implementations and mpi quick sort
- ```make run``` to run the experiment for using the OMP quick sort and serial quick sort
	output for this is omp_serial_results files
- ```make mpi_sort_run``` to compile and run the mpi quick sort implementations
- ```make psrs_run``` to compile and run the mpi psrs implementation
- ```make omp_psrs``` to compile and run the omp psrs implementation
-```make clean``` to clean the directory from all the object files and delete the build directory also removing the executables stores in the bin directory

### Note
For except for using make the other executions require for there to be a file called current_test in the current directory otherwise a file and null pointer exception errors may occure.