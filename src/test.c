#include "test.h"

int is_sorted(int *arr, int size){
	for (int i=1; i< size; ++i){
		if (arr[i] < arr[i-1])
			return 0;
	}
	return 1;
}