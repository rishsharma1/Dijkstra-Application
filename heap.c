/*------------------Libraries---------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
/*-------------------------------------------------*/


/*---------------Prototypes--------------------------*/
void swap_heap_item(HeapItem *a,HeapItem *b);
void swap_mapping(int *a,int *b);
void sift_up(Heap *h,int pos,int (cmp)(int,int));
void sift_down(Heap *h,int pos,int (cmp)(int,int));
/*---------------------------------------------------*/




/* ----------------Create Heap---------------------------
 * This function will create a new heap, and return a
 * pointer to it.
 *------------------------------------------------------*/

Heap *createHeap() {

	//mallocs space for the heap struct
	Heap *h = (Heap*)malloc(sizeof(Heap));
	assert(h);

	h->H = (HeapItem*)malloc(sizeof(HeapItem)*INITIAL);
	assert(h->H);
	h->map = (int*)malloc(sizeof(int)*INITIAL);
	assert(h->map);

	//set the initial size
	h->size = INITIAL;
	//nothing in the heap,so start from 0
	h->n = 0;

	return h;
}


/* ------------------Insert Heap-----------------------------
 * This function will insert dataIndex and key into the heap,
 * and return HEAP_SUCCESS if it inserts successfully, or
 * HEAP_FAIL otherwise.
 *----------------------------------------------------------*/

 int insert_in_heap(Heap *h, int dataIndex,int key,int (cmp)(int,int)) {

 	if(h == NULL)	
 		return HEAP_FAIL;

 	//checking whether to realloc some memory
 	if(h->n >= h->size) {
 		h->size *= 2;
 		h->H = (HeapItem*)realloc(h->H,sizeof(HeapItem)*h->size);
 		assert(h->H);
 		h->map = (int*)realloc(h->map,sizeof(int)*h->size);
 		assert(h->map);
 	}
 	h->H[h->n].key = key;
 	h->H[h->n].dataIndex = dataIndex;
 	//inserts an index in the map, that maps to the Heap item
 	h->map[dataIndex] = h->n;
 	//Inserts relevant stuff into heap item
    h->n++;
 	

 	//now we need to sift up
 	sift_up(h,h->n-1,cmp);
 	return HEAP_SUCCESS;

}

/* ------------------Peek -----------------------------------
 * This function will return the dataIndex of the root, this
 * will be the first Heap item in the Heap.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap
 * Output: dataIndex of H[0]
 *----------------------------------------------------------*/

int peek(Heap *h) {
	assert(h->n > 0);
	return h->H[0].dataIndex;
}

/* ------------------Peek Key---------------------------------
 * This function will return the key of the root, this
 * will be the first Heap item in the Heap.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap
 * Output: key of H[0]
 *----------------------------------------------------------*/

int peekKey(Heap *h) {

	assert(h->n > 0);
	return h->H[0].key;
}


/* ------------------IsEmpty---------------------------------
 * This function will return 1 if the heap is empty, 0 if it
 * is not.
 * Input: Heap
 * Output: 1 or 0
 *----------------------------------------------------------*/


int isEmpty(Heap *h) {
	return (h->n == 0);
}



/* ------------------Remove Top-------------------------------
 * This function will return the data index of the root,remove it,
 * and  will re-heapify by performing sift down. The top will 
 * either be the maximum key or minimum key depending on the 
 * comparison function.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap.
 * Output: The HeapItem of the min or max priority.
 *----------------------------------------------------------*/

HeapItem removeTop(Heap *h,int (cmp)(int,int)) {

	assert(h->n > 0);

	//swap the root with the last item in the heap
	swap_heap_item(&h->H[0],&h->H[h->n-1]);
	//swap the mapping too
	swap_mapping(&h->map[h->H[0].dataIndex],&h->map[h->H[h->n-1].dataIndex]);

	//discard the last heap item 
	h->n--;

	//now perform sift down to restore min or max heap
	sift_down(h,0,cmp);
	return h->H[h->n];

}



/* ------------------Sift down--------------------------------
 * This function will heapify by taking the position "pos",
 * finding its children, finding the priority in the children
 * and compare that value with pos, depending on the comparison
 * it will swap and recurse untill it reaches the lead nodes.
 * Special case 1: The case where the root only has one child,
 * in that case just compare the root with the left child.
 *
 * Assumptions: We can assume the Heap exists.
 * Input: Heap,index of item
 * Output: None
 *----------------------------------------------------------*/

void sift_down(Heap *h,int pos,int (cmp)(int,int)) {

	int child_left = 2*pos + 1;
	int child_rght = child_left+1;

	
	/* The case where the root only has one child: */
	if(child_left < h->n && child_rght >= h->n) {
		if(cmp(h->H[child_left].key,h->H[pos].key)) {
			swap_heap_item(&h->H[pos],&h->H[child_left]);

			swap_mapping(&h->map[h->H[pos].dataIndex],
			&h->map[h->H[child_left].dataIndex]);
		}
	}

	//cannot sift down anymore.
	if(child_left >= h->n || child_rght >= h->n) {
		return;
	}

	int priority_index;
	// select the child with higher priority depending on the compare function 
	if(cmp(h->H[child_left].key,h->H[child_rght].key)) {
		priority_index = child_left;
	}
	else {
		priority_index = child_rght;
	}
	
	//compare with pos and perform sift down according to the compare function 
	if(cmp(h->H[priority_index].key,h->H[pos].key)) {
		swap_heap_item(&h->H[pos],&h->H[priority_index]);

		swap_mapping(&h->map[h->H[pos].dataIndex],
		&h->map[h->H[priority_index].dataIndex]);

		sift_down(h,priority_index,cmp); 
	}

}

/* ------------------Sift up----------------------------------
 * This function will heapify by taking the position "pos",
 * finding its parent, compares it, and depending on the 
 * comparison function it will swap.
 * recurse until reach the root.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap,index of item, cmp
 * Output: None
 *----------------------------------------------------------*/

void sift_up(Heap *h,int pos,int (cmp)(int,int)) {
	//ensure pos > 0 as we cannot go beyond root
	if(pos <= 0) {
		return;
	}
	
	//NOTE: (pos-1)/2 is parent of pos
	if(cmp(h->H[pos].key,h->H[(pos-1)/2].key)) {
		swap_heap_item(&h->H[(pos-1)/2],&h->H[pos]);

		//we also want swap the mapping 
		swap_mapping(&h->map[h->H[(pos-1)/2].dataIndex],
		&h->map[h->H[pos].dataIndex]);

		sift_up(h,(pos-1)/2,cmp);
	}
}

/* ------------------Change Key--------------------------------
 * This function will update the key value to delta, by finding
 * it using its dataIndex in map.Then it will heapify.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap,dataIndex,change in key
 * Output: None
 *----------------------------------------------------------*/

void changeKey(Heap *h,int dataIndex,int delta,int (cmp)(int,int)) {

	assert(h->n > 0);
	//get the position of the node 
	int node_pos = h->map[dataIndex];

	//update the key of node_pos to delta
	h->H[node_pos].key = delta;
	
	//re-heapify the heap according comparison function
	if(cmp(h->H[node_pos].key,h->H[(node_pos-1)/2].key) && node_pos > 0) {
		sift_up(h,node_pos,cmp);
	}
	else {
		sift_down(h,node_pos,cmp);
	}


}

/* ------------------Destroy Heap------------------------------
 * This will free any memory allocated for the heap.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap
 * Output: None
 *----------------------------------------------------------*/

 void destroyHeap(Heap *h) {

 	free(h->H);
 	free(h->map);
 	free(h);
 }


 /* ------------------Swap Heap Item-------------------------
 * This will will be used to swap positions of two heap items
 * in the heap. 
 * Input: HeapItem,HeapItem
 * Output: None
 *----------------------------------------------------------*/
 
void swap_heap_item(HeapItem *a,HeapItem *b) {
  
  	HeapItem temp;
  	temp = *a;
  	*a = *b;
  	*b = temp;
}

 /* ------------------Swap Mapping---------------------------
 * This will will be used to swap mapping positions.
 * in the heap. 
 * Input: int,int
 * Output: None
 *----------------------------------------------------------*/

void swap_mapping(int *a,int *b) {
  	
  	int temp;
  	temp = *a;
  	*a = *b;
 	*b = temp;

}
/* ------------------Print Heap---------------------------------
 * This function will print the heap, starting from the root.
 * Assumptions: We can assume the Heap exists.
 * Input: Heap
 * Output: None.
 *----------------------------------------------------------*/

void Print_Heap(Heap *h) {

	assert(h->n > 0);

	for(int i=0;i<h->n;i++) {

		fprintf(stderr,"{dataIndex = %d Weight = %d}\n",
		h->H[i].dataIndex,h->H[i].key);
	}
	
}

/* ----------------Minimum compare------------------------
 * The comparison function for min Heap.
 * Input: int,int
 * Ouput: 1 or 0
 *------------------------------------------------------*/

int min_func(int a,int b) {
	 if(a <= b)
	 	return 1;
	 return 0;
}

/* ----------------Maximum compare------------------------
 * The comparison function for max Heap.
 * Input: int,int
 * Ouput: 1 or 0
 *------------------------------------------------------*/

int max_func(int a,int b) {
	if(a >= b)
		return 1;
	return 0;
}


 	
 	

 