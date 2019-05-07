#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base.h"

int world_size, world_rank, max_rank;
/*
	@brief: The quick sort function implemented using omp
		The solution was adapted from Prasad Perera:		https://www.codeproject.com/KB/threads/Parallel_Quicksort/Parallel_Quick_sort_without_merge.pdf
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
void quick_sort_mpi(int *arr, int low, int hi, int index){
	if(hi > low){
		MPI_Status status;
		int share_rank = world_rank + pow(2, index); /*get proc we can share with*/
		++index;
		
		if(share_rank > world_size){ //if we have more processes
			quick_sort_serial(arr, low, hi);
			return;
		}
		int pi = partition(arr, low, hi);
		//send to the next available process
		MPI_Send(arr+pi, (hi-pi), MPI_INT, share_rank, pi , MPI_COMM_WORLD);
		quick_sort_mpi(arr, low, pi-1, index);
		MPI_Recv(arr+pi,(hi-pi),MPI_INT,share_rank,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		printf("received arr from %d as %d\n", share_rank, world_rank);
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
	printf("process %d received from %d arr of size %d\n", world_rank, proc_sending, sub_arr_size);
	quick_sort_mpi(number_buf, 0, sub_arr_size-1, index);
	MPI_Send(number_buf, sub_arr_size, MPI_INT, proc_sending, tag, MPI_COMM_WORLD);
	
	free(number_buf);
}

void init(int *arr, int size){
	 MPI_Init(NULL, NULL);
	 MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	 MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	 if (world_rank == 0){
		 int *arr = create(int, 20);
		 quick_sort_mpi(arr, 0, size-1, 0);
		 for(int i=0; i<20; ++i){
			printf("%d", arr[i]);
			if(i!=19)
				printf(", ");
			if(i==19)
				printf("\n");
		}
	 }else{
		 receive_message();
	 }
	 MPI_Finalize();
}

int main(){
	init(NULL, 20);
	return 0;
}