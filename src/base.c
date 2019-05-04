#include "base.h"

/*
	@brief: The quick sort function
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
int partition(int *arr, int low, int hi){
	int pivot = arr[hi]; //set the value of the pivot
	int i = low-1; //index the smallest element
	for(int j=low; j < hi; ++j){
		if(arr[j]<pivot){//check if item j is smaller than the pivot
			swap_data(&arr[++i], &arr[j]);//swap item j with the element next to the smallest item
		}
	}
	swap_data(&arr[++i], &arr[hi]); //place j on the right track
	return i; //return the index of the pivot
}

/*
	@brief: Method to swap two data items from an array
	@param d1: pointer to the first data items we are swapping
	@param d2: pointer to the second data item we are swapping
*/
void swap_data(int* d1, int* d2){
	int temp = *d1;
	*d1 = *d2;
	*d2 =  temp;
}
