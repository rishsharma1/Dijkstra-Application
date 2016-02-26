/*------------------Libraries---------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include "heap.h"
/*-------------------------------------------------*/




/*---------------Function definitions---------------------*/

/* ----------------Create Set---------------------------
 * This function will create a set of size "size_of_set"
 * ,the representation of this set is through a bit array
 * where if an element of index "i" is present, it is 
 * represented as a 1, otherwise its a 0.
 * Input: size of set
 * Output: Pointer to a set
 *------------------------------------------------------*/

set_t *create_set(int size_of_set) {

	set_t *set_item = (set_t*)malloc(sizeof(set_t));
	assert(set_item);
	set_item->set = (int *)malloc(sizeof(int)*size_of_set);
	assert(set_item->set);

	for(int i=0;i<size_of_set;i++) {
		set_item->set[i] = 0;
	}
	// the items covered
	set_item->covered_items = 0;
	// flag, where 1 represents the set being covered, 0 otherwise
	set_item->covered = 0;

	return set_item;
}


/* ----------------Create All Sets---------------------
 * This creates an universe of sets, essentially an array
 * of sets. In this case the set is represented differently
 * to create_set() function, the sets now store the index of
 * each element they represent. 
 * Input: size of universe, size of set
 * Ouput: a pointer to an array of sets.
 *------------------------------------------------------*/

Universe *create_all_sets(int size_of_universe,int size_of_set) {

	Universe *Uni = (Universe *)malloc(sizeof(Universe));
	assert(Uni);
	Uni->size_of_universe =  size_of_universe;

	Uni->sets = (set_t *)malloc(sizeof(set_t)*size_of_universe);
	assert(Uni->sets);

	for(int i=0;i<size_of_universe;i++) {

		Uni->sets[i].set = (int *)malloc(sizeof(int)*size_of_set);
		assert(Uni->sets[i].set);
		
		// the items covered
		Uni->sets[i].covered_items = 0;
		// flag, where 1 represents the set being covered, 0 otherwise
		Uni->sets[i].covered = 0;
	}

	return Uni;
}

/* ----------------Insert--------------------------------
 * Inserts element in set "set_index" in Uni
 * Input: Universe of sets,set_index,element 
 * Ouput: None.
 *------------------------------------------------------*/


void insert(Universe *Uni,int set_index,int  element) {

	assert(Uni);
	// inserting the element in relevant set
	Uni->sets[set_index].set[Uni->sets[set_index].covered_items++] = element;

}

/* ----------------Print Sets----------------------------
 * prints all the sets present in Uni
 * Input: Universe of sets
 * Ouput: None.
 *------------------------------------------------------*/


void print_sets(Universe *Uni) {

	for(int i=0;i<Uni->size_of_universe;i++) {
		printf("Set school %d: ",i);
		
		for(int j=0;j<Uni->sets[i].covered_items;j++) {
			fprintf(stderr,"%d ",Uni->sets[i].set[j]);
			
			
		}
		fprintf(stderr,"number of uncovered_items: %d\n",
		Uni->sets[i].covered_items);
	}

}

/* ----------------Sets Difference------------------------
 * Gives the difference for A, in B, finds the items in A
 * that dont occur in B.
 * Input: Set,Set
 * Ouput: Set difference
 *------------------------------------------------------*/

int set_diff(set_t *A,set_t *B) {
	int diff = 0;

	for(int i=0;i<A->covered_items;i++) {

		if(!B->set[A->set[i]]) {
			diff++;
		}
	}
	return diff;
}


/* ----------------Set Cover--------------------------------
 * Initially the set cover inserts all the sets indices, and 
 * the number of items they cover into a Max Heap. Then 
 * It keeps poping the set with the maximum number of 
 * covered houses, through that it updates the covered
 * set, which keeps track which houses are covered. Then
 * the covered array is used to update all the uncovered sets 
 * covered_items according which houses are covered in the
 * covered array.And we repeat this until we cover all houses.
 * Input: Universe of sets,number of houses  
 * Ouput: None.
 *------------------------------------------------------*/

void setCover(Universe *Uni,int num_houses) {



	Heap *Q = createHeap();

	set_t *covered = create_set(num_houses);
	

	for(int i=0;i<Uni->size_of_universe;i++) {
		insert_in_heap(Q,i,Uni->sets[i].covered_items,max_func);
	}

	
 
	while(covered->covered_items < num_houses) {

		
		if(isEmpty(Q)) {
			break;
		}

		HeapItem max = removeTop(Q,max_func);
		

		
		//updating the covered set
		for(int i=0;i<Uni->sets[max.dataIndex].covered_items;i++) {

			//if its not covered 
			if(!covered->set[Uni->sets[max.dataIndex].set[i]]) {
					//set to the house being covered 
					covered->set[Uni->sets[max.dataIndex].set[i]] = 1;
					//increment number of covered houses
					covered->covered_items++;
			}
		}
	
		//flag the current set to be covered 
		Uni->sets[max.dataIndex].covered = 1;
				
 		//update the other sets
		for(int i=0;i<Uni->size_of_universe;i++) {

				//if they are not already covered
				if(!Uni->sets[i].covered) {

					//find set diff and update the heap
					int diff = set_diff(&Uni->sets[i],covered);
					changeKey(Q,i,diff,max_func);
				}
				
		}
		
	}

	if(covered->covered_items < num_houses) {
		fprintf(stderr, "Not enough houses to cover all houses.\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=0; i<Uni->size_of_universe;i++) {
		if(Uni->sets[i].covered) {
			printf("%d\n",i);
		}
	}


	destroyHeap(Q);
	free_set(covered);
	free_Universe(Uni);


}

/* ----------------Free set------------------------------
 * Free the set
 * Input: Set
 * Ouput: None.
 *------------------------------------------------------*/

void free_set(set_t *s) {
	free(s->set);
	free(s);
	s=NULL;
}

/* ----------------Free Universe---------------------------
 * Free the array of sets.
 * Input: Universe of sets
 * Ouput: None.
 *------------------------------------------------------*/

void free_Universe(Universe *Uni) {

	free(Uni->sets);
	free(Uni);
	Uni=NULL;
}




