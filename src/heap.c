/* Sudhanshu Patel sudhanshuptl13@gmail.com */
/*
Min Heap implementation in c
*/
#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
#include "base.h"
/*
 Array Implementation of MinHeap data Structure
*/

Heap *CreateHeap(int capacity,int heap_type){
    Heap *h = (Heap * ) malloc(sizeof(Heap)); //one is number of heap

    //check if memory allocation is fails
    if(h == NULL){
        exit(0);
    }
    h->heap_type = heap_type;
    h->count=0;
    h->capacity = capacity;
    h->arr = create(mmdata, capacity*sizeof(mmdata)); //size in bytes

    //check if allocation succeed
    if ( h->arr == NULL){
        exit(0);
    }
    return h;
}

void insert(Heap *h, mmdata key){
    if( h->count < h->capacity){
        h->arr[h->count] = key;
        heapify_bottom_top(h, h->count);
        h->count++;
    }
}

void heapify_bottom_top(Heap *h, int index){
    mmdata temp;
    int parent_node = (index-1)/2;

    if(h->arr[parent_node].stvalue > h->arr[index].stvalue){
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    mmdata temp;

    if(left >= h->count || left <0)
        left = -1;
    if(right >= h->count || right <0)
        right = -1;

    if(left != -1 && h->arr[left].stvalue < h->arr[parent_node].stvalue)
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->arr[right].stvalue < h->arr[min].stvalue)
        min = right;

    if(min != parent_node){
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, min);
    }
}

mmdata PopMin(Heap *h){
    mmdata pop;
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count-1];
    h->count--;
    heapify_top_bottom(h, 0);
    return pop;
}

int is_empty(Heap *h){
	return h->count == 0;
}

void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",h->arr[i].stvalue);
    }
    printf("->__/\\__\n");
}