/*------------------Details------------------------
  Name: Rishabh Sharma 
  Student Number: 694739
  Date: 20/04/2015
 -------------------------------------------------*/

/*----------------Definitions----------------------*/
//used for determining success of insertion into heap
#define HEAP_SUCCESS 1
#define HEAP_FAIL 0
//initial heap size
#define INITIAL 10
/*-------------------------------------------------*/

typedef struct item {
	int key; // key for deciding the position in the heap
	int dataIndex; // the payload index provided by the calling program
}HeapItem;

typedef struct heap {
	HeapItem *H; //the underlying array 
    int *map; // map[i] is index into H of location of payload with dataIndex == i
    int n; //the number of items currently in the heap
	int size; //the maximum number of items allowed in the heap
}Heap;


/*---------------------Prototypes---------------------------------*/

//returns a pointer to a new,empty heap
Heap *createHeap();
//inserts dataIndex into h. 
//Returns HEAP_SUCCESS if it has inserted, or HEAP_FAIL otherwise 
 int insert_in_heap(Heap *h, int dataIndex,int key,int (cmp)(int,int));
//returns the data index of the root
int peek(Heap *h);
//returns the key of the root
int peekKey(Heap *h);
//removes the root, returns the data index to it, and re-heapifies it
//may end up changing other items map values
HeapItem removeTop(Heap *h,int (cmp)(int,int));
//adds delta to the key of dataIndex and then re-heapifies it
void changeKey(Heap *h,int dataIndex,int delta,int (cmp)(int,int));
//free memory allocated in heap creation
void destroyHeap(Heap *h);
//checks if the heap is empty
int isEmpty(Heap *h);
//prints the heap
void Print_Heap(Heap *h);
// min compare function
int min_func(int a,int b);
// max compare function
int max_func(int a,int b);
