

typedef struct {
	int *set;
	int covered_items; // items it covers 
	int covered;	// 0 or 1 
}set_t;

typedef struct {
	set_t *sets;	// array of set_t
	int size_of_universe;	// number of sets
}Universe;


/*---------------------Prototypes---------------------------------*/

// create an array of sets
Universe *create_all_sets(int size_of_universe,int size_of_set);
// insert element into set 
void insert(Universe *Uni,int set_index,int element); 
// create an bit array representation of a set
set_t *create_set(int size_of_set);
// print all sets 
void print_sets(Universe *Uni);
// find the minimum sets it takes to cover all houses
void setCover(Universe *Uni, int num_houses);
// free the array of sets
void free_Universe(Universe *Uni);
// free the set
void free_set(set_t *s);
// set difference 
int set_diff(set_t *A,set_t *B);