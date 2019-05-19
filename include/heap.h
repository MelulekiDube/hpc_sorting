#ifndef HEAP_H
#define HEAP_H

typedef struct mmdata {
	int stindex;
	int index;
	int stvalue;
}mmdata;

typedef struct Heap{
     struct mmdata *arr;	
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
} Heap;

Heap *CreateHeap(int capacity,int heap_type);
void insert(Heap *h,  struct mmdata key);
void print(Heap *h);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
int is_empty(Heap *h);
mmdata PopMin(Heap *h);

#endif