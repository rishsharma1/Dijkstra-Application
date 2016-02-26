/*------------------Libraries---------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
/*-------------------------------------------------*/


/*----------------Definitions----------------------*/
#define INITIAL 10
#define FAIL 0
#define SUCCESS 1 
/*-------------------------------------------------*/

/*--------------Reference--------------------------
 * Using Andrew Turpin's graph.c template
 *-------------------------------------------------*/

 /*---------------Prototypes-----------------------*/
 void swap(Edge *a,Edge *b);
 /*------------------------------------------------*/



/* ------------------Graph New--------------------------------
 * This function will create a graph with an input of 
 * number_of_vertices and return a pointer to it.
 * Input: number of vertices 
 * Output: pointer to a Graph.
 *----------------------------------------------------------*/

Graph *
graph_new(int number_of_vertices) {
    assert(number_of_vertices > 0);
    int i;
    Graph *g = NULL;

    g = (Graph*)malloc(sizeof(Graph));
    assert(g);
    g->number_of_vertices = number_of_vertices;

    g->vertices = (Vertex*)malloc(sizeof(Vertex)*number_of_vertices);
    assert(g->vertices);

    for(i=0;i<number_of_vertices;i++) {
    	//Initial allocation of memory
    	g->vertices[i].max_num_edges = INITIAL;
    	//Initialising number of edges 
    	g->vertices[i].num_edges = 0;
    	//mallocing array of edges
    	g->vertices[i].edges = (Edge*)malloc(sizeof(Edge)*INITIAL);
    	assert(g->vertices[i].edges);

    }


    return g;
}

/* ------------------Add Edge--------------------------------
 * For a given vertex v, this function will add the edge u and
 * any data to be added with that edge.
 * Input: Graph, Vertex Label, Edge Label,data
 * Output: None.
 *----------------------------------------------------------*/
void  
graph_add_edge(Graph *g, Label v, Label u, int data) {
    assert(g);
    //no negative vertices
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);

    if(g->vertices[v].num_edges >= g->vertices[v].max_num_edges) {
    	g->vertices[v].max_num_edges *= 2;

    	g->vertices[v].edges = (Edge *)realloc(g->vertices[v].edges
        ,sizeof(Edge)*g->vertices[v].max_num_edges);
    	assert(g->vertices[v].edges);
    }
   
    //add an edge to vertex v, and incrementing the edge count
    g->vertices[v].edges[g->vertices[v].num_edges++].u = u;
    g->vertices[v].data = data;


}

/* ------------------Graph Delete Edge-------------------------
 * This function will delete Edge u, from Vertex v, but will not
 * delete duplicates. 
 * Input: Graph,Vertex,Edge.
 * Ouput: None.
 *----------------------------------------------------------*/
void  
graph_del_edge(Graph *g, Label v, Label u) {

    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);

    for(int i=0;i<g->vertices[v].num_edges;i++) {

    	if(g->vertices[v].edges[i].u == u) {
    		
    		//swap the edge to be deleted with last item in edges array
    		swap(&g->vertices[v].edges[i],
    		&g->vertices[v].edges[g->vertices[v].num_edges-1]);
    		
    		//update number of edges 
    		g->vertices[v].num_edges--;
    		// found one therefore we can assume to break out of loop
    		break;
    	}
    }

}

/* ------------------EdgeArray-------------------------------
 * This function will return the array of Edges of a vertex.
 * Input: Graph,Vertex,num_edges
 * Output: An array of Edges.
 *----------------------------------------------------------*/
Edge *
graph_get_edge_array(Graph *g, Label v, int *num_edges) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);

    *num_edges = g->vertices[v].num_edges;

    return g->vertices[v].edges;
}

/* ------------------Graph Has Edge---------------------------
 * If a graph has certain Edge at a certain vertex, this
 * function returns a 1, otherwise a 0.
 * Input: Graph,Vertex,Edge
 * Output: 1 or 0
 *----------------------------------------------------------*/
int
graph_has_edge(Graph *g, Label v, Label u) {


    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);

    for(int i=0;i<g->vertices[v].num_edges;i++) {

    	if(g->vertices[v].edges[i].u == u) {
    		return 1;
    
    	}
    }

    return 0;
}


/* ------------------Graph Set Vertex Data--------------------
 * Sets the data for the a vertex to "data".
 * Input: Graph,Vertex,Data
 * Output: None.
 *----------------------------------------------------------*/

void  
graph_set_vertex_data(Graph *g, Label v, int data) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    
    g->vertices[v].data = data;
}


/* ------------------Graph Explore----------------------------
 * Starting from vertex v, it recursively visit v's edges, and
 * call exlpore on their egdges until whole graph is visted.
 * Input: Graph,Vertex
 * Output: None.
 *----------------------------------------------------------*/
void 
graph_default_explore(Graph *g, Label v) {
    if (g->vertices[v].data)
        return;

    g->vertices[v].data = 1; // mark as visited

    for(int i = 0 ; i < g->vertices[v].num_edges ; i++)
        graph_default_explore(g, g->vertices[v].edges[i].u);
}

/* ------------------Graph Check connected--------------------
 * It will call explore from source vertex v, and the result of
 * that should be that all vertices had been visted,thus the graph
 * being connected and returning SUCCESS, however if there is 
 * a vertex that is not visted, then the graph is not connected
 * and we return FAIL.
 * Input: Graph,Vertex
 * Output: SUCCESS OR FAIL.
 *----------------------------------------------------------*/
int 
graph_check_connected(Graph *g, Label v) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);

        // First explore from v
    graph_default_explore(g, v);

        // Now do the rest (if any)
    for(int i = 0; i < g->number_of_vertices ; i++) {

       
        if(!g->vertices[i].data) {

            return FAIL;
        }
    }
    return SUCCESS;
}

/* ------------------Print Graph--------------------------------
 * Prints the entire Graph.
 * Input: Graph
 * Output: None.
 *-------------------------------------------------------------*/


void print_graph(Graph *g) {

    //for each vertex we would like to print their edges 
    for(int i=0;i<g->number_of_vertices;i++) {

        for(int j=0;j<g->vertices[i].num_edges;j++) {
            fprintf(stderr,"Vertex %d Edge with %d of Weight %d\n"
            ,i,g->vertices[i].edges[j].u,g->vertices[i].edges[j].weight);
        }
    }
}

/* ------------------Free Graph--------------------------------
 * Free's the entire Graph.
 * Input: Graph
 * Output: None.
 *-------------------------------------------------------------*/

void free_graph(Graph *g) {

    for(int i=0;i<g->number_of_vertices;i++) {
        free(g->vertices[i].edges);
    }
    free(g->vertices);
    free(g);
}

/* ------------------Swap---------------------------------------
 * Used to swap two edges.
 * Input: Edge,Edge
 * Output: None.
 *-------------------------------------------------------------*/

void
swap(Edge *a,Edge *b) {
  Edge temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
