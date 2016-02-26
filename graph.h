
typedef int Label;   // a vertex label (should be numeric to index edge lists)

typedef struct {
    int data;   // any data you want to store about an edge
    Label u;        // end vertex of edge
    int weight;    // weight of an edge 
} Edge;

typedef struct {
    int   data;        // any data you want to store about a vertex
    Label  label;        // this vertex's label
    Edge  *edges;        // array of edges [0..num_edges-1]
    int   num_edges;     // number of edges
    int   max_num_edges; // current malloced size of edges array, num_edges <= max_num_edges
} Vertex;

typedef struct {
    void   *data;              // any data you want to store about a graph
    int num_houses;             // num of houses
    int num_schools;           // num of schools
    int    number_of_vertices; // num of house + num of school
    Vertex *vertices;          // array of house & school vertices
} Graph;

/*---------------------Prototypes---------------------------------*/
// create new graph
Graph *graph_new(int number_of_vertices);
// add edge u to vertex v, with data 
void  graph_add_edge(Graph *g, Label v, Label u, int data);
// delete edge u from v 
void  graph_del_edge(Graph *g, Label v, Label u);
// returns 1 or 0 depending on if edge u exists in  v  
int   graph_has_edge(Graph *g, Label v, Label u);
// returns v's edge array
Edge *graph_get_edge_array(Graph *g, Label v, int *num_edges);
// sets the v's data to data
void  graph_set_vertex_data(Graph *g, Label v, int data);
// calls exlpore on graph starting from v
void graph_default_explore(Graph *g, Label v);
// returns 1 if graph connected, 0 otherwise
int graph_check_connected(Graph *g, Label v);
// prints graph
void print_graph(Graph *g);
// free's graph
void free_graph(Graph *g);



