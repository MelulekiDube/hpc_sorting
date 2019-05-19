#include "base.h"

/*
	@brief: The quick sort function
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
void quick_sort_serial(int *arr, int low, int hi){
	if(hi > low && low >= 0){
		int pi = partition(arr, low, hi);
		if(low < pi-1)
			quick_sort_serial(arr, low, pi-1);
		if(pi+1 < hi)
			quick_sort_serial(arr, pi+1, hi);
	}
}

