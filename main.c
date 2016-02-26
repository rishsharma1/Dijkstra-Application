/*------------------Details------------------------
  Name: Rishabh Sharma 
  Student Number: 694739
  Date: 28/04/2015
 -------------------------------------------------*/


/*------------------Libraries---------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "graph.h"
#include "heap.h"
#include "set.h"
/*-------------------------------------------------*/

/*----------------Definitions----------------------*/
#define INF INT_MAX/2
#define MAX_KM 1000
/*-------------------------------------------------*/


/*---------------Prototypes------------------------*/
void printError();
void Dijkstra(Graph *g,int source,Universe *Uni,int (cmp)(int,int));
/*-------------------------------------------------*/



int main(int argc,char *argv[]) {


	int num_houses,num_schools;
	int u,v,weight;
	

	if(scanf("%d %d",&num_houses,&num_schools) != 2) {
		printError();
	}

	
	Graph *g = graph_new(num_houses+num_schools);
	g->num_houses = num_houses;
	g->num_schools = num_schools;
	g->number_of_vertices = num_houses+num_schools;

	while(scanf("%d %d %d",&u,&v,&weight) == 3) {

		if(u < 0 || v < 0 
		|| u>=(g->number_of_vertices) || v>=(g->number_of_vertices) || weight < 0){
			printError();
		}
		else {
			// the 0 represents unvisted 
			graph_add_edge(g,u,v,0);
			graph_add_edge(g,v,u,0);
			//adding weights 
			g->vertices[u].edges[g->vertices[u].num_edges-1].weight = weight;
			g->vertices[v].edges[g->vertices[v].num_edges-1].weight = weight;
		}

	}
	// checking if graph is connected 
	if(!graph_check_connected(g,0)) {
		fprintf(stderr, "Graph is not connected\n");
		exit(EXIT_FAILURE);
	}

	
	Universe *Uni = create_all_sets(g->num_schools,g->num_houses);

	//running Dijkstra on each school vertex
	for(int i=num_houses;i<g->number_of_vertices;i++) {

		Dijkstra(g,i,Uni,min_func);

	}

	setCover(Uni,g->num_houses);



	free_graph(g);
	
	return 0;
}

/*---------------Function definitions---------------------*/


/* ----------------Dijkstra----------------------------------
 * Dijkstra will ,starting from source vertex, insert the
 * smallest distance from source vertex to houses 
 * that are within 1km distance into Uni->sets[source], it will
 * prune any schools, and early terminate when smallest distance
 * reaches > 1km. 
 * Input: Graph,source,Array of sets,comparison function
 * Ouput: None.
 *-----------------------------------------------------------*/



void Dijkstra(Graph *g,int source,Universe *Uni,int (cmp)(int,int)) {

    int *dist;

    dist = (int*)malloc(sizeof(int)*(g->number_of_vertices));
    assert(dist);
   
    //set all vertices distance to INF
    for(int i=0;i<g->number_of_vertices;i++) {
        dist[i] = INF;
    }



    Heap *Q = createHeap();
    //insert everything into heap
    for(int i=0;i<g->number_of_vertices;i++) {

        if(i==source) {
            insert_in_heap(Q,i,0,cmp);
        }
        else {
            insert_in_heap(Q,i,INF,cmp);
        }
    }
    // update the distance of source 
    dist[source] = 0;
    
 

    while(!isEmpty(Q)) {


       
        HeapItem min  = removeTop(Q,cmp);

        // min can't be > 1000, if so we can stop
         if(min.key > MAX_KM) {
            break;
        }
        // prune any schools, only houses
        if(min.dataIndex < g->num_houses) {
            insert(Uni, source-g->num_houses, min.dataIndex);
        }       

        int n_edges;
        //get all edges from min.dataIndex
        Edge *edges = graph_get_edge_array(g,min.dataIndex,&n_edges);
       



        for(int i=0;i<n_edges;i++) {

        	//update all the edges distances if there is a minimum 
            if(dist[edges[i].u] > (dist[min.dataIndex] +edges[i].weight)) {
                
                dist[edges[i].u] = dist[min.dataIndex]+edges[i].weight;
                // update the heap
                changeKey(Q,edges[i].u,dist[edges[i].u],cmp);
                                
            }

        }
    
    }

    
}

/* ----------------Print Error---------------------------
 * Prints a message if any error.
 *------------------------------------------------------*/

void printError() {
	fprintf(stderr,"Incorrect Input.\n");
	exit(EXIT_FAILURE);
}




