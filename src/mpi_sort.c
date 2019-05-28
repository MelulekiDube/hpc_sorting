#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "base.h"
#include "exp.h"	
int world_size, world_rank, max_rank;
/*
	@brief: The quick sort function implemented using omp
		The solution was adapted from Prasad Perera:		https://www.codeproject.com/KB/threads/Parallel_Quicksort/Parallel_Quick_sort_without_merge.pdf
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
void quick_sort_mpi(int *arr, int low, int hi, int index){
	if(hi > low && low >= 0){
		MPI_Status status;
		int share_rank = world_rank + pow(2, index++); /*get proc we can share with*/
		if(share_rank >= world_size){ //if we have more processes
			extern void quick_sort_omp(int *arr, int low, int hi);
			quick_sort_serial(arr, low, hi);
			return;
		}
		int pi = partition(arr, low, hi);
		//send to the next available process
		if((pi-low)<(hi-pi)){
			MPI_Send(arr+low, (pi-low), MPI_INT, share_rank, pi , MPI_COMM_WORLD);
			quick_sort_mpi(arr, pi+1, hi, index);
			MPI_Recv(arr+low, (pi-low), MPI_INT,share_rank,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		}else{
			MPI_Send(arr+pi+1, (hi-pi), MPI_INT, share_rank, pi , MPI_COMM_WORLD);
			quick_sort_mpi(arr, low, pi-1, index);
			MPI_Recv(arr+pi+1,(hi-pi),MPI_INT,share_rank,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		}
	}
}

void receive_message(){
	MPI_Status status;
	int sub_arr_size, proc_sending, tag;
	
	int index = 0;
	while(pow(2, index++) <= world_rank); //find the rank
	--index;
	  // Probe for an incoming message from process zero
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // When probe returns, the status object has the size and other
    // attributes of the incoming message. Get the size of the message.
    MPI_Get_count(&status, MPI_INT, &sub_arr_size);
	proc_sending = status.MPI_SOURCE;
	tag = status.MPI_TAG;
    // Allocate a buffer just big enough to hold the incoming numbers
    int* number_buf = create(int, sub_arr_size);
    // Now receive the message with the allocated buffer
    MPI_Recv(number_buf, sub_arr_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	quick_sort_mpi(number_buf, 0, sub_arr_size-1, index);
	MPI_Send(number_buf, sub_arr_size, MPI_INT, proc_sending, tag, MPI_COMM_WORLD);
	free(number_buf);
}

void init(int *arr, int size){
	 MPI_Init(NULL, NULL);
	 MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	 MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	 if (world_rank == 0){
		double time = MPI_Wtime();
		quick_sort_mpi(arr, 0, size-1, 0);
		double duration = (MPI_Wtime()-time);
		printf("Size: %d\n", size);
		printf("\tDuration was: %f\n", duration);
		//MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
	 }else{
		 receive_message();
	 }
	 MPI_Finalize();
}

FILE_CONT* read_file(char *filename){
	int min, max, size; /*max = max number in file; min  min num in file*/
	FILE* file;
	if(!(file = fopen(filename, READ))){
		fprintf(stderr, "can't open %s\n" , filename);
		exit(2);
	}
	
	//initialize variables for creating the list
	fscanf(file,"%d %d %d", &size, &max, &min);
	//now create our list array
	int *arr = create(int, size);
	for(int i=0; i< size; ++i){
		fscanf(file, "%d", &arr[i]);
	}
	FILE_CONT* t = create(FILE_CONT, 1);
	t->size = size;
	t->arr = arr;
	fclose(file);
	return t;
}

int main(int argc, char *argv[]){
	FILE_CONT* fc = read_file("current_test");
	init(fc->arr, fc->size);
	return 0;
}