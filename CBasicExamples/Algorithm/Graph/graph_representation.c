// Graph
// Adjacency Matrix/List Representation

/*
Adjacency Matrix:
- A 2D array where the value at row i and column j indicates the presence of
  an edge between vertices i and j.
- Space Complexity: O(V^2), where V is the number of vertices.
- Time Complexity: O(1) for edge existence check, O(V) for iterating over
  neighbors.

Adjacency List:
- A list of lists (or a dictionary) where each vertex has a list of its
  adjacent vertices.
- Space Complexity: O(V + E), where V is the number of vertices and E is the
  number of edges.
- Time Complexity: O(V) for edge existence check, O(k) for iterating over
  neighbors, where k is the number of neighbors of a vertex.

Conversion between representations:
- To convert from an adjacency matrix to an adjacency list, iterate through
  the matrix and for each edge found, add the corresponding vertices to the
  adjacency list.
- To convert from an adjacency list to an adjacency matrix, create a 2D array
  initialized to 0, and for each vertex in the adjacency list, set the corresponding
  entries in the matrix to 1 for each adjacent vertex.
- Complexity of conversion:
  O(V^2) for matrix to list, O(V + E) for list to matrix (matrix creation
  would take up O(V^2)).
  Explanation:
  - The conversion from an adjacency matrix to an adjacency list requires
    iterating through the entire matrix, which takes O(V^2) time.
  - The conversion from an adjacency list to an adjacency matrix requires
    initializing a 2D array and then setting the appropriate entries, which
    takes O(V + E) time.

* When and Why you see O(V + E) in graph algorithms:
  - It is a summation
  - sigma[for each vertex v in V] (1 + degree(v))
    - 1 + degree(v) accounts for the vertex itself and its adjacent edges.
    - '1' accounts for processing that vertex (we should loop all vertices)
  - simplify:
    - sigma[for each vertex v in V] (1) + sigma[for each vertex v in V] (degree(v))
    - = O(V) + O(2E) = O(V + E)
    - * undirected graph: sum of degrees is 2E
    - * directed graph: sum of out-degrees is E
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Graph structure using adjacency list
typedef struct Graph_list
{
    int V; // Number of vertices
    int E; // Number of edges
    // Adjacency list
    // Array of pointers to arrays (or lists) of adjacent vertices)
    // NOTE: length of each adjacency list can vary,
    //       we use adjList[i][0] to store the MEMORY allocated
    //       we use adjList[i][1] to store the length of the adjacency list
    //       of vertex i.
    // Example:
    // adjList[0] points to an array of vertices adjacent to vertex 0
    int** adjList;
} Graph_list;

// Graph structure using adjacency matrix
typedef struct Graph_matrix
{
    int V; // Number of vertices
    int E; // Number of edges
    // Adjacency matrix
    // 2D array where matrix[i][j] is 1 if there is an
    // edge from vertex i to vertex j, otherwise 0
    // shape: V x V
    // NOTE: we use char to save some space
    char** matrix;
} Graph_matrix;

// Function prototypes
// List representation
Graph_list* create_graph_list(int V);
Graph_list* add_edge_list(Graph_list* graph, int src, int dest);
void print_graph_list(Graph_list* graph);
void free_graph_list(Graph_list* graph);
// Matrix representation
Graph_matrix* create_graph_matrix(int V);
Graph_matrix* add_edge_matrix(Graph_matrix* graph, int src, int dest);
void print_graph_matrix(Graph_matrix* graph);
void free_graph_matrix(Graph_matrix* graph);
// Conversion functions
Graph_list* matrix_to_list(Graph_matrix* graph);
Graph_matrix* list_to_matrix(Graph_list* graph);

// Function implementations
// List representation
Graph_list* create_graph_list(int V)
{
    // create structure container
    Graph_list * graph = (Graph_list*)malloc(sizeof(Graph_list));
    // Initialize adjacency list structure members
    graph->V = V;
    graph->E = 0;
    // Allocate memory for adjacency list
    graph->adjList = (int**)malloc(V * sizeof(int*));
    // Initialize the adjacency lists (Make them all NULL pointer first)
    for (int i = 0; i < V; ++i) graph->adjList[i] = NULL;
    // return the newly created graph
    return graph;
}
Graph_list* add_edge_list(Graph_list* graph, int src, int dest)
{
    // NOTE: we expand memory allocation by 2 (i.e. 1, 2, 4, 8, ...)
    // NOTE: this is a directed graph, we only add dest to the adjacency list
    //       of src
    // NOTE: we ALLOW self-loop (i.e. edge from a vertex to itself),
    //       but we do NOT allow parallel edge (i.e. multiple edges from src to dest)

    // ERROR checking (for src and dest we use 0-indexing)
    if ((src < 0) || (src >= graph->V) ||
        (dest < 0) || (dest >= graph->V))
    {
        printf("[ERROR] add_edge_list: 'src'/'dest' should be within [0, %d)\n", graph->V);
        return graph; // return unchanged graph
    }
    // ERROR checking (parallel edge NOT allowed)
    // We can check if dest is already in the adjacency list of src
    if (graph->adjList[src] != NULL)
    {
        for (int i = 0; i < graph->adjList[src][1]; ++i)
        {
            if (graph->adjList[src][2 + i] == dest)
            {
                printf("[ERROR] add_edge_list: parallel edge from %d to %d is NOT allowed\n", src, dest);
                return graph; // return unchanged graph
            }
        }
    }

    // Check if we have enough space
    if (graph->adjList[src] == NULL)
    {
        // NOTE:
        // we use adjList[i][0] to store the MEMORY allocated
        // we use adjList[i][1] to store the length of the adjacency list
        graph->adjList[src] = (int*)malloc((2 + 1) * sizeof(int));
        // Update allocated space
        graph->adjList[src][0] = 1; // this is the space of the list to store adjacency vertex (NOT total memory size, does not count first two)
        graph->adjList[src][1] = 0; // currently nothing inside
    }
    else if (graph->adjList[src][1] + 1 > graph->adjList[src][0])
    {
        // Double the capacity
        int new_capacity = graph->adjList[src][0] * 2;
        // Use realloc to resize the existing array efficiently
        int *new_array = (int*)realloc(graph->adjList[src], (2 + new_capacity) * sizeof(int));
        if (new_array != NULL) 
        {
            graph->adjList[src] = new_array;
            graph->adjList[src][0] = new_capacity;
        }
        else
        {
            printf("[ERROR] add_edge_list: Memory reallocation failed\n");
            return graph;
        }
    }

    // Add dest to the adjacency list of src
    graph->adjList[src][2 + graph->adjList[src][1]] = dest;
    // Update length of the adjacency list
    graph->adjList[src][1] += 1;
    // Update number of edges
    graph->E += 1;

    // Return the updated graph
    return graph;  
}
void print_graph_list(Graph_list* graph)
{
    // Print the graph in adjacency list format
    printf("Graph (Adjacency List):\n");
    for (int i = 0; i < graph->V; ++i)
    {
        printf("  [Vertex %2d]: ", i);
        if (graph->adjList[i] != NULL)
            for (int j = 0; j < graph->adjList[i][1]; ++j)
                printf("%2d ", graph->adjList[i][2 + j]);
        printf("\n");
    }
    return;
}
void free_graph_list(Graph_list* graph)
{
    // NOTE: the passed graph will be invalid after this function

    // Free each adjacency list
    for (int i = 0; i < graph->V; ++i)
        if (graph->adjList[i] != NULL)
            free(graph->adjList[i]);
    // Free the adjacency list array and the graph structure
    free(graph->adjList);
    // Free the graph structure itself
    free(graph);
    return;
}
// Matrix representation
Graph_matrix* create_graph_matrix(int V)
{
    // create structure container
    Graph_matrix * graph = (Graph_matrix*)malloc(sizeof(Graph_matrix));
    // Initialize adjacency matrix structure members
    graph->V = V;
    graph->E = 0;
    // Allocate memory for adjacency matrix
    graph->matrix = (char**)malloc(V * sizeof(char*));
    for (int i = 0; i < V; ++i)
        graph->matrix[i] = (char*)malloc(V * sizeof(char));
    // Initialize the adjacency matrix to 0
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            graph->matrix[i][j] = 0;
    // return the newly created graph
    return graph;
}
Graph_matrix* add_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing)
    if ((src < 0) || (src >= graph->V) ||
        (dest < 0) || (dest >= graph->V))
    {
        printf("[ERROR] add_edge_matrix: 'src'/'dest' should be within [0, %d)\n", graph->V);
        return graph; // return unchanged graph
    }
    // ERROR checking (parallel edge NOT allowed)
    if (graph->matrix[src][dest] == 1)
    {
        printf("[ERROR] add_edge_matrix: parallel edge from %d to %d is NOT allowed\n", src, dest);
        return graph; // return unchanged graph
    }

    // Add edge to the adjacency matrix
    graph->matrix[src][dest] = 1;
    // Update number of edges
    graph->E += 1;

    // Return the updated graph
    return graph;
}
void print_graph_matrix(Graph_matrix* graph)
{
    // Print the graph in adjacency matrix format
    printf("Graph (Adjacency Matrix):\n");
    printf("%6c", ' ');
    // Print column headers
    for (int j = 0; j < graph->V; ++j)
        printf("%2d ", j);
    printf("\n");
    for (int i = 0; i < graph->V; ++i)
    {
        printf("%2c%2d: ", ' ', i);
        for (int j = 0; j < graph->V; ++j)
            printf("%2d ", graph->matrix[i][j]);
        printf("\n");
    }
    return;
}
void free_graph_matrix(Graph_matrix* graph)
{
    // NOTE: the passed graph will be invalid after this function

    // Free each row of the adjacency matrix
    for (int i = 0; i < graph->V; ++i)
        free(graph->matrix[i]);
    // Free the adjacency matrix array and the graph structure
    free(graph->matrix);
    // Free the graph structure itself
    free(graph);
    return;
}
// Conversion functions
Graph_list* matrix_to_list(Graph_matrix* graph)
{
    // Create a new graph in adjacency list representation
    Graph_list* new_graph = create_graph_list(graph->V);

    // Iterate through the adjacency matrix and populate the adjacency list
    for (int i = 0; i < graph->V; ++i)
        for (int j = 0; j < graph->V; ++j)
            if (graph->matrix[i][j] == 1)
                // Add edge from vertex i to vertex j in the new graph
                add_edge_list(new_graph, i, j);

    // Assert that the number of edges is the same
    if (new_graph->E != graph->E)
        printf("[WARNING] matrix_to_list: number of edges mismatch (matrix: %d, list: %d)\n", graph->E, new_graph->E);
    
    // Return the new graph
    return new_graph;
}
Graph_matrix* list_to_matrix(Graph_list* graph)
{
    // Create a new graph in adjacency matrix representation
    Graph_matrix* new_graph = create_graph_matrix(graph->V);

    // Iterate through the adjacency list and populate the adjacency matrix
    for (int i = 0; i < graph->V; ++i)
        if (graph->adjList[i] != NULL)
            for (int j = 0; j < graph->adjList[i][1]; ++j)
            {
                int dest = graph->adjList[i][2 + j];
                // Add edge from vertex i to vertex dest in the new graph
                add_edge_matrix(new_graph, i, dest);
            }
    
    // Assert that the number of edges is the same
    if (new_graph->E != graph->E)
        printf("[WARNING] list_to_matrix: number of edges mismatch (list: %d, matrix: %d)\n", graph->E, new_graph->E);

    // Return the new graph
    return new_graph;
}

// Entry point
int alg_graph_representation_main(int argc, char* argv[])
{
    // Set up
    int V = 5; // number of vertices
    Graph_list* graph_list = create_graph_list(V);
    Graph_matrix* graph_matrix = create_graph_matrix(V);

    // Add edges (make a dense graph)
    for (int i = V - 1; i >= 0; --i)
        for (int j = V - 1; j >= 0; --j)
            if (i != j) // no self-loop
            {
                add_edge_list(graph_list, i, j);
                add_edge_matrix(graph_matrix, i, j);
            }

    // Make conversions
    Graph_list* converted_list = matrix_to_list(graph_matrix);
    Graph_matrix* converted_matrix = list_to_matrix(graph_list);

    // Print results
    printf("\033[1;32m>>> Graph Representation >>>\033[0m\n");
    printf("Original Graph (Adjacency List):\n");
    print_graph_list(graph_list);
    printf("Original Graph (Adjacency Matrix):\n");
    print_graph_matrix(graph_matrix);
    printf("Converted Graph (Adjacency List):\n");
    print_graph_list(converted_list);
    printf("Converted Graph (Adjacency Matrix):\n");
    print_graph_matrix(converted_matrix);

    // Free memory
    free_graph_list(graph_list);
    free_graph_matrix(graph_matrix);
    free_graph_list(converted_list);
    free_graph_matrix(converted_matrix);

    return 0;
}