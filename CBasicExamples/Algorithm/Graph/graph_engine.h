/*
File: graph_engine.h
LANGUAGE: C99
NOT MENT TO BE RUN DIRECTLY
PLEASE MAKE SURE YOU DON'T HAVE NAMING CONFLICTS

FUNCTIONS HERE WILL BE DEFINED IN 'static inline' STYLE,
MEANING THEY ARE HEADER-ONLY AND CAN BE INCLUDED IN MULTIPLE
SOURCE FILES WITHOUT LINKING ISSUES.

This is a pure algorithm header file for graph related algorithms;
Including:
 - Graph representation
   - Adjacency matrix
   - Adjacency list
 - Graph traversal
   - Depth first search (DFS)
   - Breadth first search (BFS)
 - Shortest path algorithms
   - Dijkstra's algorithm
   - Bellman-Ford algorithm
   - Floyd-Warshall algorithm
*/

// Include guards still apply
// during compilation, we compile EACH source file separately,
// this prevents a single .c source file getting this header multiple times
#ifndef _ALG_GRAPH_ENGINE_H_
#define _ALG_GRAPH_ENGINE_H_

// Include necessary standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// =================================================
// Define of graph representations
// =================================================
/* Graph LIST */
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
/* Graph MATRIX */
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

// =================================================
// Graph representation functions
// =================================================
/* LIST - creation */
static inline Graph_list* create_graph_list(int V)
{
    // create structure container
    Graph_list * graph = (Graph_list*)malloc(sizeof(Graph_list));
    // Initialize adjacency list structure members
    graph->V = V;
    graph->E = 0;
    // Allocate memory for adjacency list
    graph->adjList = (int**)malloc(V * sizeof(int*));
    // Initialize all adjacency list pointers to NULL
    for (int i = 0; i < V; ++i)
        graph->adjList[i] = NULL;
    return graph;
}
/* LIST - destruction */
static inline void free_graph_list(Graph_list* graph)
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
/*
   LIST - search for edge 
   if found, will return index of dest in the adjacency list of src
   (i.e. position in the list)
   if not found, will return -1

   Safe to call if src/dest is out of range, will return -1 and print error message
   Safe to call if src has no adjacency list (i.e. no outgoing edge), will return -1
*/
static inline int has_edge_list(Graph_list* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing)
    if ((src < 0) || (src >= graph->V) ||
        (dest < 0) || (dest >= graph->V))
    {
        printf("[ERROR] has_edge_list: 'src'/'dest' should be within [0, %d)\n", graph->V);
        return -1; // return -1 to indicate error
    }

    // Check if dest is in the adjacency list of src
    if (graph->adjList[src] != NULL)
    {
        for (int i = 0; i < graph->adjList[src][1]; ++i)
        {
            if (graph->adjList[src][2 + i] == dest)
                return i; // return index of dest in the adjacency list of src
        }
    }

    return -1; // return -1 to indicate edge not found
}
/* LIST - addition of edge */
static inline Graph_list* add_edge_list(Graph_list* graph, int src, int dest)
{
    // NOTE: we expand memory allocation by 2 (i.e. 1, 2, 4, 8, ...)
    // NOTE: this is a directed graph, we only add dest to the adjacency list
    //       of src
    // NOTE: we ALLOW self-loop (i.e. edge from a vertex to itself),
    //       but we do NOT allow parallel edge (i.e. multiple edges from src to dest)

    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_list
    // ERROR checking (parallel edge NOT allowed)
    // We can check if dest is already in the adjacency list of src
    if (has_edge_list(graph, src, dest) != -1)
    {
        printf("[ERROR] add_edge_list: parallel edge from %d to %d is NOT allowed\n", src, dest);
        return graph; // return unchanged graph
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
/* LIST - removal of edge */
static inline Graph_list* remove_edge_list(Graph_list* graph, int src, int dest)
{
    // NOTE: this is a directed graph, we only remove dest from the adjacency list
    //       of src
    // NOTE: we ALLOW self-loop (i.e. edge from a vertex to itself),
    //       but we do NOT allow parallel edge (i.e. multiple edges from src to dest)

    // After removal, if the list used < 1/4 of the allocated space, we shrink the allocation
    // NOTE: we expand previously by 2 and we shrink by 2 (used space ~ 1/2 after shrinking),
    //       this is a common strategy to balance time and space efficiency

    /* ERROR checking + FIND index */
    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_list
    // ERROR checking (edge should exist to be removed)
    // Find index of dest in the adjacency list of src
    int index = has_edge_list(graph, src, dest);
    if (index == -1)
    {
        printf("[ERROR] remove_edge_list: no edge from %d to %d exists\n", src, dest);
        return graph; // return unchanged graph
    }

    // Remove dest from the adjacency list and
    // swap with the last element to avoid shifting
    graph->adjList[src][2 + index] = graph->adjList[src][2 + graph->adjList[src][1] - 1];

    // Update length of the adjacency list
    graph->adjList[src][1] -= 1;

    // Update number of edges
    graph->E -= 1;

    // Check if we should shrink the allocated space
    // if it's 0, we KEEP it as is, DOES NOT SHRINK to 0
    if (graph->adjList[src][1] > 0 
        && 
        graph->adjList[src][1] < graph->adjList[src][0] / 4)
    {
        // Shrink the capacity by half
        int new_capacity = graph->adjList[src][0] / 2;
        // Use realloc to resize the existing array efficiently
        // realloc will keep the existing data and copy it to the new location if needed
        int *new_array = (int*)realloc(graph->adjList[src], (2 + new_capacity) * sizeof(int));
        if (new_array != NULL) 
        {
            graph->adjList[src] = new_array;
            graph->adjList[src][0] = new_capacity;
        }
        else
        {
            // NOTE: if shrinking fails, we can still keep the existing array, 
            //       just with more allocated space than needed, this is not a critical error
            printf("[ERROR] remove_edge_list: Memory reallocation failed during shrinking\n");
            return graph;
        }
    }

    // Return the updated graph
    return graph;
}
/* LIST UTILITY - print the list */
static inline void print_graph_list(Graph_list* graph)
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
/* MATRIX - creation */
static inline Graph_matrix* create_graph_matrix(int V)
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
    // NO INITIAL EDGES
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            graph->matrix[i][j] = 0;
    // return the newly created graph
    return graph;
}
/* MATRIX - destruction */
static void free_graph_matrix(Graph_matrix* graph)
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
/* 
   MATRIX - search for edge
   DIFFERENT from list, return 0 (false) or 1 (true) directly
*/
static inline int has_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing)
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
    {
        printf("[ERROR] has_edge_matrix: Invalid vertex indices\n");
        return 0; // return 0 to indicate error (no edge)
    }

    // Return the value at the intersection of the two vertices
    // NOTE: this will return 1 if there is an edge, and 0 if there is no edge
    return graph->matrix[src][dest];
}
/* MATRIX - addition of edge */
static inline Graph_matrix* add_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_matrix
    // ERROR checking (parallel edge NOT allowed)
    if (has_edge_matrix(graph, src, dest) == 1)
    {
        printf("[ERROR] add_edge_matrix: parallel edge from %d to %d is NOT allowed\n", src, dest);
        return graph; // return unchanged graph
    }

    // Set the value at the intersection of the two vertices to 1
    graph->matrix[src][dest] = 1;
    // Update number of edges
    graph->E += 1;
    // Return the updated graph
    return graph;
}
/* MATRIX - removal of edge */
static inline Graph_matrix* remove_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_matrix
    // ERROR checking (edge should exist to be removed)
    if (has_edge_matrix(graph, src, dest) == 0)
    {
        printf("[ERROR] remove_edge_matrix: no edge from %d to %d exists\n", src, dest);
        return graph; // return unchanged graph
    }

    // Set the value at the intersection of the two vertices to 0
    graph->matrix[src][dest] = 0;
    // Update number of edges
    graph->E -= 1;
    // Return the updated graph
    return graph;
}
/* MATRIX UTILITY - print the matrix */
static inline void print_graph_matrix(Graph_matrix* graph)
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
/* UTILITY CONVERSION - matrix to list */
static inline Graph_list* matrix_to_list(Graph_matrix* graph)
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
/* UTILITY CONVERSION - list to matrix */
static inline Graph_matrix* list_to_matrix(Graph_list* graph)
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


#endif // _ALG_GRAPH_ENGINE_H_