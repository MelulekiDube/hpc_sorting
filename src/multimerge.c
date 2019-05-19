#include <stdlib.h>
#include <stdio.h>
#include "multimerge.h"
#include "heap.h"


int multimerge(int * starts[], const int lengths[], const int Number, 
			   int newArray[], const int newArrayLength)
{
 	// Create priority queue.  There will be at most one item in the priority queue
 	// for each of the Number lists.
 	Heap *priorities = CreateHeap(Number,0);

 	// Examine each of the Number start[] lists, place the first location into 
	// the priority 	queue if the list is not empty
 	for(int i=0; i<Number;i++){
		if (lengths[i]>0){
			mmdata temp = {.stindex= i, .index = 0, .stvalue= starts[i][0]};
			insert(priorities, temp);
		}
	}


	// As long as priorities is not empty, pull off the top member (the smallest 
	//value from list i), push it into the newArray, and place the next element from 
	// list i in the priority queue
	int newArrayindex = 0;  // index into the merged array
	while (!is_empty(priorities) && (newArrayindex<newArrayLength))
	{
		// grab the smallest element, and remove it from the priority queue
		mmdata xxx = PopMin(priorities);

		// insert this smallest element into the merged array
		newArray[newArrayindex++] = starts[xxx.stindex][xxx.index];

		// if start[xxx.stindex] is not empty, place the next member into priority
		if ( lengths[xxx.stindex]>(xxx.index+1))
		{
			mmdata temp = xxx;
			++temp.index;
			temp.stvalue = starts[xxx.stindex][temp.index];
			insert(priorities, temp);
		}
	}
	free(priorities);
	// return the logical size of the merged array
	return newArrayindex;
}
