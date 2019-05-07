/*
*	Base header file for the Sorting algorithms
* 	Created  by: Meluleki Dube
*	Data: 28 April 2019
*/
#ifndef BASE_H
#define BASE_H
#define create(type, size) ((type*)malloc(sizeof(type)*size))
/*
	@brief: Method to do the partition for the quick sort algorithm
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
int partition(int *arr, int low, int hi);
/*
	@brief: The quick sort functions
	@param arr: the array that we are sorting
	@param low: where we start sorting
	@param hi: we we stop sorting
*/
void quick_sort_serial(int *arr, int low, int hi); //serial quick sort
void quick_sort_omp(int *arr, int low, int hi); //quick sort implemented using omp
void quick_sort_mpi(int *arr, int low, int hi, int index);//quick sort implemented with mpi

/*
	@brief: Method to swap two data items from an array
	@param d1: pointer to the first data items we are swapping
	@param d2: pointer to the second data item we are swapping
*/
void swap_data(int* d1, int* d2);

#endif