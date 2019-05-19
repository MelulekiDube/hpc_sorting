#include <omp.h>
#include "base.h"
#include <stdio.h>
/*
	@brief: The quick sort function implemented using omp
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
void quick_sort_omp(int *arr, int low, int hi){
	if(hi > low && low >= 0 ){
		int pi = partition(arr, low, hi);
		if(low < pi-1){
			#pragma omp task
			quick_sort_omp(arr, low, pi-1);
		}
		if(pi+1 <= hi){
			#pragma omp task
			quick_sort_omp(arr, pi+1, hi);
		}
	}
}