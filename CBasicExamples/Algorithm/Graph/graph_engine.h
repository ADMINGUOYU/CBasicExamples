/*
File: graph_engine.h
LANGUAGE: C99
NOT MEANT TO BE RUN DIRECTLY
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
 - Minimum spanning tree (MST) algorithms
   - Prim's algorithm
   - Kruskal's algorithm
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
// Define ERRORS (ERRORs are ALL NEGATIVE numbers)
// =================================================
typedef enum Graph_error
{
    VALUE_ERROR = INT_MIN, // value error (e.g. invalid vertex index)
    NOT_FOUND_ERROR = -1   // not found error (e.g. edge not found)
} Graph_error;

// =================================================
// Define of graph representations
// =================================================
/* Graph LIST */
typedef struct Graph_list
{
    int V; // Number of vertices
    int E; // Number of edges
    // Adjacency list (weighted)
    // NOTE:
    //   - adjList[src] points to an array of edges, each edge is int[2]
    //   - adjList[src][k][0] is destination vertex
    //   - adjList[src][k][1] is edge weight
    //   - adjCapacity[src] stores allocated edge slots for src
    //   - adjSize[src] stores used edge slots for src
    int (*(*adjList))[2];
    int* adjCapacity;
    int* adjSize;
} Graph_list;
/* Graph MATRIX */
typedef struct Graph_matrix
{
    int V; // Number of vertices
    int E; // Number of edges
    // Adjacency matrix (weighted)
    // matrix[i][j] is the edge weight from i to j.
    // A value of 0 means no edge exists,
    // if you need 0 weights, you can use list representation.
    int** matrix;
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
    graph->adjList = (int (**)[2])malloc(V * sizeof(int (*)[2]));
    graph->adjCapacity = (int*)calloc(V, sizeof(int));
    graph->adjSize = (int*)calloc(V, sizeof(int));
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
    free(graph->adjCapacity);
    free(graph->adjSize);
    // Free the graph structure itself
    free(graph);
    return;
}
/*
   LIST - search for edge 
   if found, will return index of dest in the adjacency list of src
   (i.e. position in the list)
   if not found, will return NOT_FOUND_ERROR

   Safe to call if src/dest is out of range, will return VALUE_ERROR and print error message
   Safe to call if src has no adjacency list (i.e. no outgoing edge), will return NOT_FOUND_ERROR
*/
static inline int has_edge_list(Graph_list* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing)
    if ((src < 0) || (src >= graph->V) ||
        (dest < 0) || (dest >= graph->V))
    {
        printf("[ERROR] has_edge_list: 'src'/'dest' should be within [0, %d)\n", graph->V);
        return VALUE_ERROR; // return VALUE_ERROR to indicate error
    }

    // Check if dest is in the adjacency list of src
    if (graph->adjList[src] != NULL)
    {
        for (int i = 0; i < graph->adjSize[src]; ++i)
        {
            if (graph->adjList[src][i][0] == dest)
                return i; // return index of dest in the adjacency list of src
        }
    }

    return NOT_FOUND_ERROR; // return NOT_FOUND_ERROR to indicate edge not found
}
/* LIST - get edge weight */
static inline int get_edge_weight_list(Graph_list* graph, int src, int dest)
{
    // check if we have the edge
    int index = has_edge_list(graph, src, dest);
    // if not, error
    if (index == VALUE_ERROR || index == NOT_FOUND_ERROR)
    {
        // print a warning message as VALUE_ERROR is also a number
        printf("[ERROR] get_edge_weight_list: edge from %d to %d does not exist\n", src, dest);
        return VALUE_ERROR; // return VALUE_ERROR to indicate error
    }
    return graph->adjList[src][index][1]; // return the weight of the edge
}
/* LIST - addition of edge */
static inline Graph_list* add_edge_list(Graph_list* graph, int src, int dest, int weight)
{
    // NOTE: we expand memory allocation by 2 (i.e. 1, 2, 4, 8, ...)
    // NOTE: this is a directed graph, we only add dest to the adjacency list
    //       of src
    // NOTE: we ALLOW self-loop (i.e. edge from a vertex to itself),
    //       but we do NOT allow parallel edge (i.e. multiple edges from src to dest)

    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_list
    // ERROR checking (parallel edge NOT allowed)
    // We can check if dest is already in the adjacency list of src
    int index = has_edge_list(graph, src, dest);
    if (index == VALUE_ERROR)
    {
        // This means src/dest is out of range, we just return unchanged graph
        return graph;
    }
    if (index != NOT_FOUND_ERROR)
    {
        printf("[ERROR] add_edge_list: parallel edge from %d to %d is NOT allowed\n", src, dest);
        return graph; // return unchanged graph
    }

    // Check if we have enough space
    if (graph->adjList[src] == NULL)
    {
        graph->adjList[src] = (int (*)[2])malloc(1 * sizeof(int[2]));
        // Update allocated space
        graph->adjCapacity[src] = 1; // this is the space of the list to store adjacency vertex (NOT total memory size, does not count first two)
        graph->adjSize[src] = 0; // currently nothing inside
    }
    else if (graph->adjSize[src] + 1 > graph->adjCapacity[src])
    {
        // Double the capacity
        int new_capacity = graph->adjCapacity[src] * 2;
        // Use realloc to resize the existing array efficiently
        int (*new_array)[2] = (int (*)[2])realloc(graph->adjList[src], new_capacity * sizeof(int[2]));
        if (new_array != NULL) 
        {
            graph->adjList[src] = new_array;
            graph->adjCapacity[src] = new_capacity;
        }
        else
        {
            printf("[ERROR] add_edge_list: Memory reallocation failed\n");
            return graph;
        }
    }

    // Add edge (dest, weight) to the adjacency list of src
    graph->adjList[src][graph->adjSize[src]][0] = dest;
    graph->adjList[src][graph->adjSize[src]][1] = weight;
    // Update length of the adjacency list
    graph->adjSize[src] += 1;
    // Update number of edges
    graph->E += 1;

    // Return the updated graph
    return graph;  
}
/* LIST - addition of edge (no direction) -> TWO directed edges */
static inline Graph_list* add_edge_undirected_list(Graph_list* graph, int src, int dest, int weight)
{
    // Add the edge in both directions
    add_edge_list(graph, src, dest, weight);
    add_edge_list(graph, dest, src, weight);
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
    if (index == VALUE_ERROR)
    {
        // This means src/dest is out of range, we just return unchanged graph
        return graph;
    }
    if (index == NOT_FOUND_ERROR)
    {
        printf("[ERROR] remove_edge_list: no edge from %d to %d exists\n", src, dest);
        return graph; // return unchanged graph
    }

    // Remove dest from the adjacency list and
    // swap with the last element to avoid shifting
    graph->adjList[src][index][0] = graph->adjList[src][graph->adjSize[src] - 1][0];
    graph->adjList[src][index][1] = graph->adjList[src][graph->adjSize[src] - 1][1];

    // Update length of the adjacency list
    graph->adjSize[src] -= 1;

    // Update number of edges
    graph->E -= 1;

    // Check if we should shrink the allocated space
    // if it's 0, we KEEP it as is, DOES NOT SHRINK to 0
    if (graph->adjSize[src] > 0 
        && 
        graph->adjSize[src] < graph->adjCapacity[src] / 4)
    {
        // Shrink the capacity by half
        int new_capacity = graph->adjCapacity[src] / 2;
        // Use realloc to resize the existing array efficiently
        // realloc will keep the existing data and copy it to the new location if needed
        int (*new_array)[2] = (int (*)[2])realloc(graph->adjList[src], new_capacity * sizeof(int[2]));
        if (new_array != NULL) 
        {
            graph->adjList[src] = new_array;
            graph->adjCapacity[src] = new_capacity;
        }
        else
        {
            // NOTE: if shrinking fails, we can still keep the existing array, 
            //       just with more allocated space than needed, this is not a critical error
            printf("[ERROR] remove_edge_list: Memory reallocation failed during shrinking\n");
            // we can still continue
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
            for (int j = 0; j < graph->adjSize[i]; ++j)
                printf("(%2d,w=%2d) ", graph->adjList[i][j][0], graph->adjList[i][j][1]);
        printf("\n");
    }
    return;
}
/* MATRIX - creation */
static inline Graph_matrix* create_graph_matrix(int V)
{
    // PRINT WARNING: no 0 weight edges for matrix representation
    printf("[WARNING] create_graph_matrix: no 0 weight edges for matrix representation (0 means no edge), if you need 0 weights, you can use list representation\n");

    // create structure container
    Graph_matrix * graph = (Graph_matrix*)malloc(sizeof(Graph_matrix));
    // Initialize adjacency matrix structure members
    graph->V = V;
    graph->E = 0;
    // Allocate memory for adjacency matrix
    graph->matrix = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i)
        graph->matrix[i] = (int*)malloc(V * sizeof(int));
    // Initialize the adjacency matrix to 0
    // NO INITIAL EDGES
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            graph->matrix[i][j] = 0;
    // return the newly created graph
    return graph;
}
/* MATRIX - destruction */
static inline void free_graph_matrix(Graph_matrix* graph)
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

   if found, will return 1 (true)
   if not found, will return NOT_FOUND_ERROR
   if src/dest is out of range, will return VALUE_ERROR and print error message
*/
static inline int has_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing)
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
    {
        printf("[ERROR] has_edge_matrix: Invalid vertex indices\n");
        return VALUE_ERROR; // return VALUE_ERROR to indicate error (no edge)
    }

    // Return the value at the intersection of the two vertices
    // NOTE: this returns 1 if there is an edge, and NOT_FOUND_ERROR if there is no edge
    return (graph->matrix[src][dest] != 0) ? 1 : NOT_FOUND_ERROR;
}
/* MATRIX - addition of edge */
static inline Graph_matrix* add_edge_matrix(Graph_matrix* graph, int src, int dest, int weight)
{
    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_matrix
    // ERROR checking (parallel edge NOT allowed)
    int edge_check = has_edge_matrix(graph, src, dest);
    if (edge_check == VALUE_ERROR)
    {
        // This means src/dest is out of range, we just return unchanged graph
        return graph;
    }
    if (edge_check == 1)
    {
        printf("[ERROR] add_edge_matrix: parallel edge from %d to %d is NOT allowed\n", src, dest);
        return graph; // return unchanged graph
    }

    // NOTE: 0 is reserved by matrix representation to indicate no edge.
    if (weight == 0)
    {
        printf("[ERROR] add_edge_matrix: edge weight cannot be 0 (0 means no edge)\n");
        return graph;
    }

    // Set the value at the intersection of the two vertices to edge weight
    graph->matrix[src][dest] = weight;
    // Update number of edges
    graph->E += 1;
    // Return the updated graph
    return graph;
}
/* MATRIX - addition of edge (undirected) */
static inline Graph_matrix* add_edge_undirected_matrix(Graph_matrix* graph, int src, int dest, int weight)
{
    // Add the edge in both directions
    add_edge_matrix(graph, src, dest, weight);
    add_edge_matrix(graph, dest, src, weight);
    return graph;
}
/* MATRIX - removal of edge */
static inline Graph_matrix* remove_edge_matrix(Graph_matrix* graph, int src, int dest)
{
    // ERROR checking (for src and dest we use 0-indexing) -> checked in has_edge_matrix
    // ERROR checking (edge should exist to be removed)
    int edge_check = has_edge_matrix(graph, src, dest);
    if (edge_check == VALUE_ERROR)
    {
        // This means src/dest is out of range, we just return unchanged graph
        return graph;
    }
    if (edge_check == NOT_FOUND_ERROR)
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
    printf("Graph (Adjacency Matrix) - numbers are weights (0 means no edge):\n");
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
            if (graph->matrix[i][j] != 0)
                // Add edge from vertex i to vertex j in the new graph
                add_edge_list(new_graph, i, j, graph->matrix[i][j]);

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
            for (int j = 0; j < graph->adjSize[i]; ++j)
            {
                int dest = graph->adjList[i][j][0];
                int weight = graph->adjList[i][j][1];
                // Add edge from vertex i to vertex dest in the new graph
                add_edge_matrix(new_graph, i, dest, weight);
            }
    
    // Assert that the number of edges is the same
    if (new_graph->E != graph->E)
        printf("[WARNING] list_to_matrix: number of edges mismatch (list: %d, matrix: %d)\n", graph->E, new_graph->E);

    // Return the new graph
    return new_graph;
}

// =================================================
// Define of TREE representations (for traversals)
// =================================================
/* Tree Node */
typedef struct Graph_tree_node
{
    // the vertex this node represents (this is the KEY)
    int vertex;
    /* For tree saving children */
    struct Graph_tree_node** children; // array of pointers to child nodes
    int num_children;                  // number of child nodes
    int child_capacity;                // allocated capacity for children array
    /* For trees tracking of its parent - used in MST Kruskal */
    struct Graph_tree_node* parent;    // pointer to parent node
} Graph_tree_node;
/* Tree */
typedef struct Graph_tree
{
    Graph_tree_node* root;
} Graph_tree;

// =================================================
// Define of TREE functions
// =================================================
/* Create a tree with a root node (vertex as key) */
static inline Graph_tree* create_graph_tree(int root_vertex)
{
    // allocate memory for the tree structure and the root node
    Graph_tree* tree = (Graph_tree*)malloc(sizeof(Graph_tree));
    if (!tree) { printf("[ERROR] create_graph_tree: malloc failed\n"); return NULL; }
    Graph_tree_node* root = (Graph_tree_node*)malloc(sizeof(Graph_tree_node));
    if (!root) { printf("[ERROR] create_graph_tree: malloc failed\n"); free(tree); return NULL; }
    
    // initialize the root node
    root->vertex = root_vertex;
    root->children = NULL;
    root->num_children = 0;
    root->child_capacity = 0;
    root->parent = root; // we set this to itself (for Kruskal)

    // set the root of the tree
    tree->root = root;

    // return the newly created tree
    return tree;
}
/* Recursively free a subtree rooted at node */
static inline void free_tree_node_recursively(Graph_tree_node* node)
{
    // base case - if node is NULL, just return
    if (!node) return;
    // recursively free all children
    for (int i = 0; i < node->num_children; ++i)
        free_tree_node_recursively(node->children[i]);
    // free the children array and the node itself
    if (node->children) free(node->children);
    free(node);
}
/* Free entire tree */
static inline void free_graph_tree(Graph_tree* tree)
{
    // NOTE: the passed tree will be invalid after this function
    // if tree is NULL, just return
    if (!tree) return;
    // recursively free the whole tree starting from the root
    free_tree_node_recursively(tree->root);
    // free the tree structure itself
    free(tree);
}
/* Recursively find node by vertex key */
static inline Graph_tree_node* find_tree_node_recursively(Graph_tree_node* node, int vertex)
{
    // base case - if node is NULL, return NULL
    if (!node) return NULL;
    // if the current node matches the vertex, return it
    if (node->vertex == vertex) return node;
    // recursively search in the children (depth first strategy)
    for (int i = 0; i < node->num_children; ++i)
    {
        // call recursively on the child node
        Graph_tree_node* found = find_tree_node_recursively(node->children[i], vertex);
        if (found) return found;
    }
    return NULL;
}
/* Find node in tree by vertex key */
static inline Graph_tree_node* find_tree_node(Graph_tree* tree, int vertex)
{
    // if tree is NULL, just return NULL
    if (!tree) return NULL;
    // find on its root recursively
    return find_tree_node_recursively(tree->root, vertex);
}
/* Find root of a node*/
static inline Graph_tree_node* find_tree_root(Graph_tree_node* node)
{
    // if node is NULL, return NULL
    if (!node) return NULL;
    // if node is root (parent points to itself), return it
    if (node->parent == node) return node;
    // otherwise, recursively find the root
    return find_tree_root(node->parent);
}
/*
   Add a new node with new_vertex as child of parent_vertex.
   Returns pointer to newly created node or NULL on error.
*/
static inline Graph_tree_node* add_tree_node(Graph_tree* tree, int parent_vertex, int new_vertex)
{
    // NOTE: We enlarge the children array of the parent node by 2 (i.e. 1, 2, 4, 8, ...)

    // ERROR checking: tree should not be NULL, 
    // parent_vertex should exist
    if (!tree) { printf("[ERROR] add_tree_node: tree is NULL\n"); return NULL; }
    Graph_tree_node* parent = find_tree_node(tree, parent_vertex);
    if (!parent) { printf("[ERROR] add_tree_node: parent %d not found\n", parent_vertex); return NULL; }
    // ERROR checking: new_vertex should not already exist in the tree
    // it should be acyclic, so we check if new_vertex already exists in the tree
    if (find_tree_node(tree, new_vertex))
    {
        printf("[ERROR] add_tree_node: vertex %d already exists in the tree\n", new_vertex);
        return NULL;
    }

    // Create the new child node
    Graph_tree_node* child = (Graph_tree_node*)malloc(sizeof(Graph_tree_node));
    if (!child) { printf("[ERROR] add_tree_node: malloc failed\n"); return NULL; }
    child->vertex = new_vertex;
    child->children = NULL;
    child->num_children = 0;
    child->parent = parent;

    // Adjust the parent's children array
    if (parent->children == NULL)
    {
        // initial allocation for children array
        parent->children = (Graph_tree_node**)malloc(1 * sizeof(Graph_tree_node*));
        if (!parent->children) { printf("[ERROR] add_tree_node: malloc failed\n"); free(child); return NULL; }
        parent->child_capacity = 1;
    }
    else if (parent->num_children + 1 > parent->child_capacity)
    {
        // Expansion needed, double the capacity
        int new_capacity = parent->child_capacity * 2;
        Graph_tree_node** new_array = (Graph_tree_node**)realloc(parent->children, new_capacity * sizeof(Graph_tree_node*));
        if (!new_array) { printf("[ERROR] add_tree_node: realloc failed\n"); free(child); return NULL; }
        parent->children = new_array;
        parent->child_capacity = new_capacity;
    }
    // Add the new child to the parent's children array
    parent->children[parent->num_children] = child;
    parent->num_children += 1;

    // Return pointer to the newly created child node
    return child;
}
/*
   Remove node (and its subtree) identified by vertex.
   If the removed node is the root, the entire tree is freed and NULL is returned.
   Otherwise returns the (possibly updated to NULL) tree pointer.
*/
static inline Graph_tree* remove_tree_node(Graph_tree* tree, int vertex)
{
    // NOTE: After removal, if the parent's children array used < 1/4 of the allocated space, we shrink the allocation
    // NOTE: we expand previously by 2 and we shrink by 2 (used space ~ 1/2 after shrinking)
    
    // ERROR checking: tree should not be NULL, vertex should exist
    if (!tree) { printf("[ERROR] remove_tree_node: tree is NULL\n"); return NULL; }
    // ERROR checking: vertex should exist in the tree
    // We can find the target node by vertex key
    Graph_tree_node* target = find_tree_node(tree, vertex);
    if (!target) { printf("[ERROR] remove_tree_node: vertex %d not found\n", vertex); return tree; }

    /* If target is root, free whole tree and return NULL */
    if (target->parent == target) // this means target is root (we set root's parent to itself)
    {
        free_graph_tree(tree);
        return NULL;
    }

    // Find the target node and its parent
    // NOTE: we free everything in the subtree rooted at target,
    //       and we remove target from its parent's children array
    Graph_tree_node* parent = target->parent;
    int idx = -1;
    for (int i = 0; i < parent->num_children; ++i)
        if (parent->children[i] == target) { idx = i; break; }
    if (idx == -1) { printf("[ERROR] remove_tree_node: internal error\n"); return tree; }

    /* Remove from parent's array: swap-with-last then shrink */
    parent->children[idx] = parent->children[parent->num_children - 1];
    parent->num_children -= 1;
    // Check if we should shrink the allocated space
    // Again, if it's 0, we KEEP it as is, DOES NOT SHRINK to 0
    if (parent->num_children > 0
        &&
        parent->num_children < parent->child_capacity / 4)
    {
        // Shrink the capacity by half
        int new_capacity = parent->child_capacity / 2;
        Graph_tree_node** new_array = (Graph_tree_node**)realloc(parent->children, new_capacity * sizeof(Graph_tree_node*));
        if (new_array != NULL)
        {
            parent->children = new_array;
            parent->child_capacity = new_capacity;
        }
        else        {
            // NOTE: if shrinking fails, we can still keep the existing array,
            //       just with more allocated space than needed, this is not a critical error
            printf("[ERROR] remove_tree_node: Memory reallocation failed during shrinking\n");
            // we still can continue
        }
    }

    /* Free the removed subtree */
    free_tree_node_recursively(target);

    // Return the (possibly updated) tree pointer
    return tree;
}
/* Recursively print node with indentation */
static inline void print_tree_node_recursively(Graph_tree_node* node, int depth)
{
    if (!node) return;
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("%d\n", node->vertex);
    for (int i = 0; i < node->num_children; ++i)
        print_tree_node_recursively(node->children[i], depth + 1);
}
/* Recursively Collect total edge weight - operates on list representation */
static inline int collect_total_edge_weight_recursively_list(Graph_tree_node* node, Graph_list* graph)
{
    // base case - if node is NULL, return 0
    if (!node) return 0;
    // local variable to store total weight in the subtree rooted at node
    int total = 0;
    // loop all children
    for (int i = 0; i < node->num_children; ++i)
    {
        // we can get the edge weight from the child to its parent
        // since this is a tree, there is only one edge from child to parent
        Graph_tree_node* child = node->children[i];
        // we can get the edge weight from child to parent by looking at the adjacency list of the parent
        // we need to find the edge from node->vertex to child->vertex in the adjacency list of node->vertex
        int weight = get_edge_weight_list(graph, node->vertex, child->vertex);
        // check if the returned value is VALUE_ERROR
        if (weight == VALUE_ERROR)
        // if we get VALUE_ERROR, either is an error or if the weight is actually VALUE_ERROR, we only prompt user the warning
            printf("[WARNING] collect_total_edge_weight_recursively_list: edge from %d to %d is reported to have weight VALUE_ERROR(%d), please inspect if there's a earlier error printout.\n", node->vertex, child->vertex, weight);

        // we add the weight to total
        total += weight;
        // we also need to add the total weight in the subtree rooted at child
        total += collect_total_edge_weight_recursively_list(child, graph);
    }
    return total;
}
/* Collect total edge weight - operates on list representation */
static inline int collect_total_edge_weight_list(Graph_tree* tree, Graph_list* graph)
{
    // if tree is NULL, just return 0
    if (!tree) return 0;
    // init total weight to 0
    int total = 0;
    // collect total edge weight
    total = collect_total_edge_weight_recursively_list(tree->root, graph);
    // return the total weight
    return total;
}
/* Recursively Collect weight from root to a node - operates on list representation */
static inline int collect_root_to_node_weight_recursively_list(Graph_tree_node* node, Graph_list* graph)
{
    // base case - if node is NULL, return 0
    if (!node) return 0;
    // if node is root (parent points to itself), return 0
    if (node->parent == node) return 0;
    // get the edge weight from node to its parent (start from parent and TO this node)
    int weight = get_edge_weight_list(graph, node->parent->vertex, node->vertex);
    // check if the returned value is VALUE_ERROR
    if (weight == VALUE_ERROR)
        // if we get VALUE_ERROR, either is an error or if the weight is actually VALUE_ERROR, we only prompt user the warning
        printf("[WARNING] collect_root_to_node_weight_recursively_list: edge from %d to %d is reported to have weight VALUE_ERROR(%d), please inspect if there's a earlier error printout.\n", node->parent->vertex, node->vertex, weight);
    // recursively collect weight from root to parent and add the weight from parent to node
    return collect_root_to_node_weight_recursively_list(node->parent, graph) + weight;
}
/* Collect weight from root to a node - operates on list representation */
static inline int collect_root_to_node_weight_list(Graph_tree* tree, Graph_list* graph, int vertex)
{
    // if tree is NULL, just return 0
    if (!tree) return 0;
    // find the target node by vertex key
    Graph_tree_node* target = find_tree_node(tree, vertex);
    if (!target) { printf("[ERROR] collect_root_to_node_weight_list: vertex %d not found in the tree\n", vertex); return VALUE_ERROR; }
    // collect weight from root to the target node
    int total = collect_root_to_node_weight_recursively_list(target, graph);
    // return the total weight
    return total;
}
/* Print the whole tree (root first) */
static inline void print_graph_tree(Graph_tree* tree)
{
    if (!tree) { printf("[Graph Tree] (null)\n"); return; }
    printf("Graph Tree (root = %d):\n", tree->root ? tree->root->vertex : -1);
    if (tree->root) print_tree_node_recursively(tree->root, 0);
}

// =================================================
// Graph traversal functions (BFS and DFS)
// =================================================
/* BFS traversal on list representation, returns traversal tree */
static inline Graph_tree* bfs_traversal_list(Graph_list* graph, int start_vertex)
{
    // NOTE: simple circular queue implemented locally (fixed capacity = graph->V)

    // ERROR checking: graph should not be NULL, start_vertex should be valid
    if (!graph) { printf("[ERROR] bfs_traversal_list: graph is NULL\n"); return NULL; }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] bfs_traversal_list: 'start_vertex' should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // create a traversal tree with start_vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    // Error checking for tree creation
    if (!tree) return NULL;

    // create a visited array to track visited vertices
    int* visited = (int*)calloc(graph->V, sizeof(int));
    if (!visited)
    {
        printf("[ERROR] bfs_traversal_list: calloc failed\n");
        free_graph_tree(tree);
        return NULL;
    }

    // simple circular queue (capacity = graph->V)
    int q_capacity = graph->V; // it should always > 0
    int* queue = (int*)malloc(q_capacity * sizeof(int));
    if (!queue)
    {
        printf("[ERROR] bfs_traversal_list: malloc failed for queue\n");
        free(visited);
        free_graph_tree(tree);
        return NULL;
    }

    // queue front and rear indices, and current size
    int q_front = 0;
    int q_rear = 0;
    int q_size = 0;

    // start BFS traversal from start_vertex
    // mark it as visited
    visited[start_vertex] = 1;
    // enqueue start_vertex
    queue[q_rear] = start_vertex;
    // update rear and size
    q_rear = (q_rear + 1) % q_capacity;
    q_size += 1;

    // Loop until the queue is empty
    while (q_size > 0)
    {
        // dequeue
        int current = queue[q_front];
        // update front and size
        q_front = (q_front + 1) % q_capacity;
        q_size -= 1;

        // for each neighbor of the current vertex, if it has not been visited,
        // we add it to the tree and enqueue it
        if (graph->adjList[current] != NULL)
            for (int i = 0; i < graph->adjSize[current]; ++i)
            {
                // get the next vertex from the adjacency list
                int next = graph->adjList[current][i][0];
                // if it has not been visited, we add it to the tree and enqueue it
                if (!visited[next])
                {
                    // mark it as visited
                    visited[next] = 1;
                    // add it to the tree as a child of current
                    if (!add_tree_node(tree, current, next))
                    {
                        free(queue);
                        free(visited);
                        free_graph_tree(tree);
                        return NULL;
                    }
                    // enqueue next (queue capacity == graph->V, should not overflow in BFS)
                    if (q_size >= q_capacity)
                    {
                        // unexpected overflow, abort safely
                        printf("[ERROR] bfs_traversal_list: unexpected queue overflow\n");
                        free(queue);
                        free(visited);
                        free_graph_tree(tree);
                        return NULL;
                    }
                    // enqueue next
                    queue[q_rear] = next;
                    q_rear = (q_rear + 1) % q_capacity;
                    q_size += 1;
                }
            }
    }

    // cleanup
    free(queue);
    free(visited);

    // return the traversal tree
    return tree;
}
/* DFS traversal on list representation, returns traversal tree */
static inline Graph_tree* dfs_traversal_list(Graph_list* graph, int start_vertex)
{
    // ERROR checking: graph should not be NULL, start_vertex should be valid
    if (!graph) { printf("[ERROR] dfs_traversal_list: graph is NULL\n"); return NULL; }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] dfs_traversal_list: 'start_vertex' should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // NOTE: we use a stack to track the vertices to visit,
    //       and we use a visited array to track visited vertices to avoid cycles

    // create a traversal tree with start_vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    if (!tree) return NULL;

    // create a visited array to track visited vertices
    int* visited = (int*)calloc(graph->V, sizeof(int));

    // We can implement the stack using two parallel arrays:
    // one for vertices and one for the next adjacency index to explore for each vertex
    int* stack_vertices = (int*)malloc(graph->V * sizeof(int));
    int* stack_next_adj_idx = (int*)malloc(graph->V * sizeof(int));
    if (!visited || !stack_vertices || !stack_next_adj_idx)
    {
        printf("[ERROR] dfs_traversal_list: memory allocation failed\n");
        if (visited) free(visited);
        if (stack_vertices) free(stack_vertices);
        if (stack_next_adj_idx) free(stack_next_adj_idx);
        free_graph_tree(tree);
        return NULL;
    }

    // start DFS traversal from start_vertex
    int top = 0;
    visited[start_vertex] = 1;
    stack_vertices[top] = start_vertex;
    stack_next_adj_idx[top] = 0;

    while (top >= 0)
    {
        // get the current vertex from the top of the stack
        int current = stack_vertices[top];

        // if no list or no more neighbor to process, pop
        if (graph->adjList[current] == NULL
            ||
            stack_next_adj_idx[top] >= graph->adjSize[current])
        {
            // return to backtrack
            top -= 1;
            continue;
        }

        // get the next vertex from the adjacency list
        int next = graph->adjList[current][stack_next_adj_idx[top]][0];
        // increment the next adjacency index for the current vertex
        stack_next_adj_idx[top] += 1;

        // if visited, we skip it
        if (!visited[next])
        {
            // mark it as visited
            visited[next] = 1;
            // add it to the tree as a child of current
            if (!add_tree_node(tree, current, next))
            {
                free(visited);
                free(stack_vertices);
                free(stack_next_adj_idx);
                free_graph_tree(tree);
                return NULL;
            }
            // push it to the stack for further exploration (depth first)
            top += 1;
            stack_vertices[top] = next;
            stack_next_adj_idx[top] = 0;
        }
    }

    // cleanup
    free(visited);
    free(stack_vertices);
    free(stack_next_adj_idx);

    // return the traversal tree
    return tree;
}

// =================================================
// Minimum Spanning Tree (MST) algorithms
// UNDIRECTED graphs only
// =================================================
/* Prim's MST on list representation */
static inline Graph_tree* prim_mst_list(Graph_list* graph, int start_vertex)
{
    // NOTE: this uses an array-based priority queue of edges: [from, to, weight]
    // NOTE: we use brute force linear scan to extract-min
    // just for demonstration purpose

    // PRINT WARNING FOR UNDIRECTED GRAPH
    printf("[WARNING] prim_mst_list: this function assumes the input graph is undirected, if it's directed, the result may not be a valid MST.\n");

    // ERROR checking: graph should not be NULL and should have at least 1 vertex
    if (!graph) { printf("[ERROR] prim_mst_list: graph is NULL\n"); return NULL; }
    if (graph->V <= 0)
    {
        printf("[ERROR] prim_mst_list: graph should have at least 1 vertex\n");
        return NULL;
    }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] prim_mst_list: start_vertex should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // create MST tree with the specified start vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    if (!tree) return NULL;

    // visited/in-MST tracking (init to 0)
    int* in_mst = (int*)calloc(graph->V, sizeof(int));
    if (!in_mst)
    {
        printf("[ERROR] prim_mst_list: calloc failed\n");
        free_graph_tree(tree);
        return NULL;
    }

    // array-based priority queue: each row is [from, to, weight]
    // at most we have all edges in the graph
    int pq_capacity = (graph->E > 0) ? graph->E : 1;
    int (*pq)[3] = (int (*)[3])malloc(pq_capacity * sizeof(int[3]));
    if (!pq)
    {
        printf("[ERROR] prim_mst_list: malloc failed for priority queue\n");
        free(in_mst);
        free_graph_tree(tree);
        return NULL;
    }
    // current size of the priority queue (marks last available index)
    int pq_size = 0;

    // start from the specified vertex
    in_mst[start_vertex] = 1;   // mark it as in MST
    if (graph->adjList[start_vertex] != NULL)   // look into it's adjacency list (edges out)
        for (int i = 0; i < graph->adjSize[start_vertex]; ++i)
        {
            /* FROM:  */ pq[pq_size][0] = start_vertex;
            /* TO:    */ pq[pq_size][1] = graph->adjList[start_vertex][i][0];
            /* WEIGHT:*/ pq[pq_size][2] = graph->adjList[start_vertex][i][1];
            pq_size += 1;
        }

    // build MST by selecting minimum crossing edge each time
    int mst_edges = 0;  // we expect V - 1 edges in the MST (if not not connected)
    while (pq_size > 0 && mst_edges < graph->V - 1)
    {
        // extract-min from array-based priority queue (linear scan)
        int min_idx = 0;
        for (int i = 1; i < pq_size; ++i)
            if (pq[i][2] < pq[min_idx][2]) min_idx = i;

        // get the current minimum edge from the priority queue
        int from = pq[min_idx][0];
        int to = pq[min_idx][1];

        // remove selected item by swap-with-last
        pq[min_idx][0] = pq[pq_size - 1][0];
        pq[min_idx][1] = pq[pq_size - 1][1];
        pq[min_idx][2] = pq[pq_size - 1][2];
        pq_size -= 1;

        // skip if destination vertex already in MST
        if (in_mst[to]) continue;

        // add the chosen edge to MST tree
        if (!add_tree_node(tree, from, to))
        {
            free(pq);
            free(in_mst);
            free_graph_tree(tree);
            return NULL;
        }

        // mark the new vertex as in MST
        in_mst[to] = 1;
        mst_edges += 1;

        // push outgoing edges from newly added vertex
        if (graph->adjList[to] != NULL)
            for (int i = 0; i < graph->adjSize[to]; ++i)
            {
                int next = graph->adjList[to][i][0];
                if (!in_mst[next])
                {
                    if (pq_size >= pq_capacity)
                    {
                        // this should not happen if graph->E is accurate
                        printf("[ERROR] prim_mst_list: unexpected priority queue overflow\n");
                        free(pq);
                        free(in_mst);
                        free_graph_tree(tree);
                        return NULL;
                    }
                    pq[pq_size][0] = to;
                    pq[pq_size][1] = next;
                    pq[pq_size][2] = graph->adjList[to][i][1];
                    pq_size += 1;
                }
            }
    }

    // if graph is disconnected, we only get MST of the reachable component from the start vertex
    if (mst_edges != graph->V - 1)
        printf("[WARNING] prim_mst_list: graph may be disconnected, MST covers reachable component from root %d\n", start_vertex);

    // cleanup and return
    free(pq);
    free(in_mst);
    return tree;
}
/* Kruskal's MST on list representation */
static inline Graph_tree* kruskal_mst_list(Graph_list* graph, int start_vertex)
{
    // NOTE: we used brute force sorting and union-find without optimizations
    //       just for demonstration purpose

    // PRINT WARNING FOR UNDIRECTED GRAPH
    printf("[WARNING] kruskal_mst_list: this function assumes the input graph is undirected, if it's directed, the result may not be a valid MST.\n");

    // ERROR checking: graph should not be NULL and should have at least 1 vertex
    if (!graph) { printf("[ERROR] kruskal_mst_list: graph is NULL\n"); return NULL; }
    if (graph->V <= 0)
    {
        printf("[ERROR] kruskal_mst_list: graph should have at least 1 vertex\n");
        return NULL;
    }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] kruskal_mst_list: start_vertex should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // create MST tree with the specified start vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    if (!tree) return NULL;

    // gather all edges into an array: [from, to, weight]
    int edge_capacity = (graph->E > 0) ? graph->E : 1;
    int (*edges)[3] = (int (*)[3])malloc(edge_capacity * sizeof(int[3]));
    if (!edges)
    {
        printf("[ERROR] kruskal_mst_list: malloc failed for edge array\n");
        free_graph_tree(tree);
        return NULL;
    }

    // we loop through the adjacency list and gather all edges
    // NOTE: if undirected graph,
    //       we will have both (u, v) and (v, u) in the adjacency list,
    //       we will gather both as separate edges,
    //       but it does not affect the correctness of Kruskal's algorithm
    //       since they have the same weight and connect the same two vertices
    //       we'll exclude it when checking for cycles
    int edge_count = 0;
    for (int from = 0; from < graph->V; ++from)
        if (graph->adjList[from] != NULL)
            for (int j = 0; j < graph->adjSize[from]; ++j)
            {
                if (edge_count >= edge_capacity)
                {
                    // this should not happen if graph->E is accurate
                    printf("[ERROR] kruskal_mst_list: unexpected edge array overflow\n");
                    free(edges);
                    free_graph_tree(tree);
                    return NULL;
                }
                // set up the edge as [from, to, weight]
                edges[edge_count][0] = from;
                edges[edge_count][1] = graph->adjList[from][j][0];
                edges[edge_count][2] = graph->adjList[from][j][1];
                edge_count += 1;
            }

    // sort edges by weight (ascending) using insertion sort
    // NOTE: brute force -> O(E^2) time
    for (int i = 1; i < edge_count; ++i)
    {
        int key_from = edges[i][0];
        int key_to = edges[i][1];
        int key_weight = edges[i][2];
        int j = i - 1;
        while (j >= 0 && edges[j][2] > key_weight)
        {
            edges[j + 1][0] = edges[j][0];
            edges[j + 1][1] = edges[j][1];
            edges[j + 1][2] = edges[j][2];
            j -= 1;
        }
        edges[j + 1][0] = key_from;
        edges[j + 1][1] = key_to;
        edges[j + 1][2] = key_weight;
    }

    // union-find arrays (make-set/find-set/union-set in-function only)
    // parent[i] is the parent of vertex i in the union-find structure
    // rank[i] is the rank of the tree rooted at vertex i (used for union by rank)
    int* parent = (int*)malloc(graph->V * sizeof(int));
    int* rank = (int*)calloc(graph->V, sizeof(int)); // init all to 0
    if (!parent || !rank)
    {
        printf("[ERROR] kruskal_mst_list: malloc/calloc failed for union-find arrays\n");
        if (parent) free(parent);
        if (rank) free(rank);
        free(edges);
        free_graph_tree(tree);
        return NULL;
    }
    for (int i = 0; i < graph->V; ++i) parent[i] = i; // make-set

    // store selected MST edges as [u, v]
    // selected_edges[i][0] is u, selected_edges[i][1] is v
    // At most we have V - 1 edges selected for MST
    int selected_capacity = (graph->V > 1) ? (graph->V - 1) : 1;
    int (*selected_edges)[2] = (int (*)[2])malloc(selected_capacity * sizeof(int[2]));
    if (!selected_edges)
    {
        printf("[ERROR] kruskal_mst_list: malloc failed for selected edge array\n");
        free(parent);
        free(rank);
        free(edges);
        free_graph_tree(tree);
        return NULL;
    }
    int selected_count = 0;

    // Kruskal: scan sorted edges and keep edges that connect two different sets
    // LOOP all edges
    for (int i = 0; i < edge_count && selected_count < graph->V - 1; ++i)
    {
        // get start and end vertices of the edge
        int u = edges[i][0];
        int v = edges[i][1];

        // find-set(u) with path compression
        int root_u = u; // root of the set containing u
        while (parent[root_u] != root_u) root_u = parent[root_u];
        int walk_u = u; // walk variable to perform path compression for u
        while (parent[walk_u] != walk_u)
        {
            // we suppress all nodes along the path to point to the ultimate parent
            int next = parent[walk_u];
            parent[walk_u] = root_u;
            walk_u = next;
        }

        // find-set(v) with path compression
        int root_v = v;
        while (parent[root_v] != root_v) root_v = parent[root_v];
        int walk_v = v;
        while (parent[walk_v] != walk_v)
        {
            int next = parent[walk_v];
            parent[walk_v] = root_v;
            walk_v = next;
        }

        // if roots differ, keep this edge and union the two sets
        if (root_u != root_v)
        {
            // add this edge to selected edges
            selected_edges[selected_count][0] = u;
            selected_edges[selected_count][1] = v;
            selected_count += 1;

            // union by rank -> lesser walk time (optimization)
            // NOTE: why we don't increase rank when ranks are different?
            //       because the tree with smaller rank is attached under the
            //       one with larger rank, so the rank of the resulting tree
            //       does not increase in this case (stays with the larger tree)
            if (rank[root_u] < rank[root_v])
                parent[root_u] = root_v;
            else if (rank[root_u] > rank[root_v])
                parent[root_v] = root_u;
            else
            {
                // if ranks are equal, we can choose either one as new root,
                // here we choose root_u
                parent[root_v] = root_u;
                rank[root_u] += 1;
            }
        }
        else // if roots are the same, this edge would form a cycle, we skip it
            continue;
    }

    // Build a rooted tree from selected undirected MST edges, starting at start_vertex.
    // We repeatedly attach a new vertex when exactly one endpoint is already in the tree.
    int* in_tree = (int*)calloc(graph->V, sizeof(int));
    if (!in_tree)
    {
        printf("[ERROR] kruskal_mst_list: calloc failed for in_tree array\n");
        free(selected_edges);
        free(parent);
        free(rank);
        free(edges);
        free_graph_tree(tree);
        return NULL;
    }
    // we start with the specified vertex in the tree
    in_tree[start_vertex] = 1;

    // NOW, we have to build our tree from pieces of edges
    // we loop until no more edge can be attached,
    // in the worst case we attach V - 1 edges (if connected)
    int attached_edges = 0;
    int progress = 1;   // flag, finally becomes 0 when we cannot attach any more edge
    while (progress)
    {
        progress = 0;
        // loop all selected edges
        // This loop will run MANY MANY times to attach all edges
        for (int i = 0; i < selected_count; ++i)
        {
            // get the two endpoints of the edge
            int a = selected_edges[i][0];
            int b = selected_edges[i][1];
            // if exactly one endpoint is in the tree,
            // we can attach the other endpoint to the tree
            if (in_tree[a] && !in_tree[b])
            {
                if (!add_tree_node(tree, a, b))
                {
                    free(in_tree);
                    free(selected_edges);
                    free(parent);
                    free(rank);
                    free(edges);
                    free_graph_tree(tree);
                    return NULL;
                }
                // make b in tree too
                in_tree[b] = 1;
                attached_edges += 1;
                // we mark that we added something
                progress = 1;
            }
            // we also check the other direction since the edge is undirected,
            // we can attach a to b if b is in the tree but a is not in the tree
            else if (!in_tree[a] && in_tree[b])
            {
                if (!add_tree_node(tree, b, a))
                {
                    free(in_tree);
                    free(selected_edges);
                    free(parent);
                    free(rank);
                    free(edges);
                    free_graph_tree(tree);
                    return NULL;
                }
                // make a in tree too
                in_tree[a] = 1;
                attached_edges += 1;
                // we mark that we added something
                progress = 1;
            }
            else
            {
                // either both endpoints are already in the tree (should not happen in a tree)
                // or both endpoints are not in the tree (we cannot attach this edge yet)
                // in either case, we skip this edge for now
                continue;
            }
        }
    }

    // if graph is disconnected, we only get component reachable from start_vertex
    printf("[DEBUG] kruskal_mst_list: selected_edges = %d\n", selected_count);
    if (attached_edges != graph->V - 1)
        printf("[WARNING] kruskal_mst_list: graph may be disconnected, MST covers reachable component from root %d\n", start_vertex);

    // cleanup and return
    free(in_tree);
    free(selected_edges);
    free(parent);
    free(rank);
    free(edges);

    // return the MST tree
    return tree;
}

// =================================================
// Shortest path algorithms
// =================================================
/* Bellman-Ford shortest path tree on list representation */
static inline Graph_tree* bellmanford_sp_list(Graph_list* graph, int start_vertex)
{
    // ERROR checking: graph should not be NULL and should have at least 1 vertex
    if (!graph) { printf("[ERROR] bellmanford_sp_list: graph is NULL\n"); return NULL; }
    if (graph->V <= 0)
    {
        printf("[ERROR] bellmanford_sp_list: graph should have at least 1 vertex\n");
        return NULL;
    }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] bellmanford_sp_list: start_vertex should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // create shortest path tree with the specified start vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    if (!tree) return NULL;

    // A large value to represent infinity,
    // we use LLONG_MAX/4 to avoid potential overflow when adding edge weights
    // as we might have INF + INF, though it's not valid and smaller than INF originally
    long long inf = LLONG_MAX / 4;

    // allocate Bellman-Ford arrays
    // This array stores the shortest distance from start_vertex to each vertex, init to "infinity"
    long long* dist = (long long*)malloc(graph->V * sizeof(long long));
    // This parent array is used to reconstruct the shortest path tree, parent[i] is the parent of
    // vertex i in the shortest path tree, init to NOT_FOUND_ERROR (indicating no parent)
    int* parent = (int*)malloc(graph->V * sizeof(int));
    if (!dist || !parent)
    {
        printf("[ERROR] bellmanford_sp_list: malloc failed\n");
        if (dist) free(dist);
        if (parent) free(parent);
        free_graph_tree(tree);
        return NULL;
    }
    for (int i = 0; i < graph->V; ++i)
    {
        dist[i] = inf;  // init all distances to "infinity"
        parent[i] = NOT_FOUND_ERROR; // init all parents to NOT_FOUND_ERROR (indicating no parent)
    }

    // distance to the start vertex is 0, and we can consider it as its own parent in the tree
    dist[start_vertex] = 0;
    parent[start_vertex] = start_vertex;

    // relax all edges at most V - 1 times (as a tree, at most V - 1 edges)
    for (int it = 0; it < graph->V - 1; ++it)
    {
        // we use a flag to check if any distance was updated in this iteration
        int changed = 0;

        // loop through all edges and relax them
        for (int u = 0; u < graph->V; ++u)
            // if the distance to u is not infinity, we can try to relax edges out of u
            // NOTE: if the parent is INF (not connected), we don't expect
            //       we're able to relax it.
            if (dist[u] != inf && graph->adjList[u] != NULL)
                // loop through all edges out of u and try to relax them
                for (int j = 0; j < graph->adjSize[u]; ++j)
                {
                    // destination vertex v
                    int v = graph->adjList[u][j][0];
                    // weight of edge (u, v)
                    int w = graph->adjList[u][j][1];
                    // calculate new candidate distance to v through u
                    long long candidate = dist[u] + (long long)w;
                    // if through u is shorter, update distance and parent for v
                    if (candidate < dist[v])
                    {
                        dist[v] = candidate;
                        parent[v] = u;
                        // we mark that we updated something in this iteration
                        changed = 1;
                    }
                }
        // if no distance was updated in this iteration, we can stop early
        if (!changed) break;
    }

    // detect negative-weight cycle reachable from start_vertex
    // We run relaxation one more time
    for (int u = 0; u < graph->V; ++u)
        if (dist[u] != inf && graph->adjList[u] != NULL)
            for (int j = 0; j < graph->adjSize[u]; ++j)
            {
                int v = graph->adjList[u][j][0];
                int w = graph->adjList[u][j][1];
                // if we can still relax, that means we have a negative-weight cycle
                if (dist[u] + (long long)w < dist[v])
                {
                    printf("[ERROR] bellmanford_sp_list: negative-weight cycle detected from start vertex %d\n", start_vertex);
                    free(dist);
                    free(parent);
                    free_graph_tree(tree);
                    return NULL;
                }
            }

    // build rooted shortest path tree from parent array
    int* in_tree = (int*)calloc(graph->V, sizeof(int));
    if (!in_tree)
    {
        printf("[ERROR] bellmanford_sp_list: calloc failed for in_tree array\n");
        free(dist);
        free(parent);
        free_graph_tree(tree);
        return NULL;
    }

    // mark the start vertex as in the tree
    in_tree[start_vertex] = 1;

    // we count the number of reachable vertices
    // (in case the graph is disconnected)
    int reachable_count = 0;
    for (int i = 0; i < graph->V; ++i)
        if (dist[i] != inf) reachable_count += 1;

    // log attached edges
    int attached_edges = 0;
    // flag to check if we attached something
    int progress = 1;
    // loop until we cannot attach any more
    while (progress)
    {
        progress = 0;
        // loop through all vertices and try to attach them to the tree
        for (int v = 0; v < graph->V; ++v)
        {
            // skip starting vertex, unreachable vertices, and already attached vertices
            if (v == start_vertex || dist[v] == inf || in_tree[v]) continue;
            // look for it's parent
            int p = parent[v];
            // parent should not be NOT_FOUND_ERROR and should be already in the tree,
            // then we can attach v to the tree
            // NOTE: if dist[v] != inf it should have a valid parent, checking just in case.
            if (p != NOT_FOUND_ERROR && in_tree[p])
            {
                // add v to the tree as a child of p
                if (!add_tree_node(tree, p, v))
                {
                    free(in_tree);
                    free(dist);
                    free(parent);
                    free_graph_tree(tree);
                    return NULL;
                }
                // mark v in tree too
                in_tree[v] = 1;
                // log that we attached one more edge
                attached_edges += 1;
                // set flag
                progress = 1;
            }
        }
    }

    // sanity check: we should have attached exactly reachable_count - 1 edges
    // if the tree is correctly built
    if (attached_edges != reachable_count - 1)
        printf("[WARNING] bellmanford_sp_list: could not attach all reachable vertices into shortest path tree from root %d\n", start_vertex);

    // cleanup and return
    free(in_tree);
    free(dist);
    free(parent);

    // return the shortest path tree
    return tree;
}
/* Dijkstra shortest path tree on list representation */
static inline Graph_tree* dijkstra_sp_list(Graph_list* graph, int start_vertex)
{
    // ERROR checking: graph should not be NULL and should have at least 1 vertex
    if (!graph) { printf("[ERROR] dijkstra_sp_list: graph is NULL\n"); return NULL; }
    if (graph->V <= 0)
    {
        printf("[ERROR] dijkstra_sp_list: graph should have at least 1 vertex\n");
        return NULL;
    }
    if (start_vertex < 0 || start_vertex >= graph->V)
    {
        printf("[ERROR] dijkstra_sp_list: start_vertex should be within [0, %d)\n", graph->V);
        return NULL;
    }

    // NOTE: Dijkstra requires non-negative edge weights
    // loop all edges to check for negative weights, if found, we print error and return NULL
    for (int u = 0; u < graph->V; ++u)
        if (graph->adjList[u] != NULL)
            for (int j = 0; j < graph->adjSize[u]; ++j)
                if (graph->adjList[u][j][1] < 0)
                {
                    printf("[ERROR] dijkstra_sp_list: negative edge weight detected (%d -> %d, w=%d)\n",
                        u, graph->adjList[u][j][0], graph->adjList[u][j][1]);
                    printf("[NOTE] dijkstra_sp_list: consider using Bellman-Ford for graphs with negative edge weights\n");
                    return NULL;
                }

    // create shortest path tree with the specified start vertex as root
    Graph_tree* tree = create_graph_tree(start_vertex);
    if (!tree) return NULL;

    // A large value to represent infinity,
    // we use LLONG_MAX/4 to avoid potential overflow when adding edge weights
    // as we might have INF + INF, though it's not valid and smaller than INF originally
    long long inf = LLONG_MAX / 4;

    // allocate Dijkstra arrays
    // dist array stores the shortest distance from start_vertex to each vertex, init to "infinity"
    long long* dist = (long long*)malloc(graph->V * sizeof(long long));
    // parent array is used to reconstruct the shortest path tree, parent[i] is the parent of
    // vertex i in the shortest path tree, init to NOT_FOUND_ERROR (indicating no parent)
    int* parent = (int*)malloc(graph->V * sizeof(int));
    // settled array tracks whether a vertex's shortest distance is finalized (settled),
    // init to 0 (not settled)
    // settled means that node is connected and fully explored
    int* settled = (int*)calloc(graph->V, sizeof(int));
    if (!dist || !parent || !settled)
    {
        printf("[ERROR] dijkstra_sp_list: malloc/calloc failed\n");
        if (dist) free(dist);
        if (parent) free(parent);
        if (settled) free(settled);
        free_graph_tree(tree);
        return NULL;
    }

    // initialize dist and parent arrays
    for (int i = 0; i < graph->V; ++i)
    {
        dist[i] = inf;
        parent[i] = NOT_FOUND_ERROR;
    }

    // distance to the start vertex is 0, and we can consider it as its own parent in the tree
    dist[start_vertex] = 0;
    parent[start_vertex] = start_vertex;

    // O(V^2 + E) Dijkstra using linear extract-min over unsettled vertices
    // loop V times to settle at most V vertices (in a tree, at most V - 1 edges)
    for (int it = 0; it < graph->V; ++it)
    {
        int u = NOT_FOUND_ERROR;

        // find the unsettled vertex with the smallest distance
        // loop the dist array to find the next vertex to settle
        // NOTE: at this time, its parent has already been set
        // NOTE: the first run will settle start_vertex
        for (int i = 0; i < graph->V; ++i)
            if (!settled[i] && (u == NOT_FOUND_ERROR || dist[i] < dist[u]))
                u = i;

        // remaining vertices are unreachable
        if (u == NOT_FOUND_ERROR || dist[u] == inf) break;

        // mark u as settled (we connect u to the connected set S)
        settled[u] = 1;

        // relax all outgoing edges from u, as now u is in S
        if (graph->adjList[u] != NULL)
            // loop all it's outgoing edges
            for (int j = 0; j < graph->adjSize[u]; ++j)
            {
                // get destination vertex v and weight w of edge (u, v)
                int v = graph->adjList[u][j][0];
                int w = graph->adjList[u][j][1];
                // calculate start_vertex -> ... -> u -> v as a candidate path to v
                long long candidate = dist[u] + (long long)w;
                // if smaller, relax v and update parent for v
                if (!settled[v] && candidate < dist[v])
                {
                    dist[v] = candidate;
                    parent[v] = u;
                }
            }
    }

    // build rooted shortest path tree from parent array
    int* in_tree = (int*)calloc(graph->V, sizeof(int));
    if (!in_tree)
    {
        printf("[ERROR] dijkstra_sp_list: calloc failed for in_tree array\n");
        free(dist);
        free(parent);
        free(settled);
        free_graph_tree(tree);
        return NULL;
    }

    // mark the start vertex as in the tree
    in_tree[start_vertex] = 1;

    // we count the number of reachable vertices (in case the graph is disconnected)
    int reachable_count = 0;
    for (int i = 0; i < graph->V; ++i)
        if (dist[i] != inf) reachable_count += 1;

    // log attached edges
    int attached_edges = 0;
    // flag to check if we attached something
    int progress = 1;
    // loop until we cannot attach any more
    while (progress)
    {
        progress = 0;
        // loop through all vertices and try to attach them to the tree
        for (int v = 0; v < graph->V; ++v)
        {
            // skip starting vertex, unreachable vertices, and already attached vertices
            if (v == start_vertex || dist[v] == inf || in_tree[v]) continue;
            // look for it's parent, if parent is valid and already in the tree,
            // we can attach v to the tree
            int p = parent[v];
            // NOTE: if dist[v] != inf, it should have a valid parent, we check just in case.
            if (p != NOT_FOUND_ERROR && in_tree[p])
            {
                // add v to the tree as a child of p
                if (!add_tree_node(tree, p, v))
                {
                    free(in_tree);
                    free(dist);
                    free(parent);
                    free(settled);
                    free_graph_tree(tree);
                    return NULL;
                }
                // mark v in tree too
                in_tree[v] = 1;
                attached_edges += 1;
                // we mark that we added something
                progress = 1;
            }
        }
    }

    // sanity check: we should have attached exactly reachable_count - 1 edges if
    // the tree is correctly built
    if (attached_edges != reachable_count - 1)
        printf("[WARNING] dijkstra_sp_list: could not attach all reachable vertices into shortest path tree from root %d\n", start_vertex);

    // cleanup and return
    free(in_tree);
    free(dist);
    free(parent);
    free(settled);

    // return the shortest path tree
    return tree;
}
/*
   Floyd-Warshall all-pairs shortest path matrix on matrix representation
   Please convert to matrix to proceed, please make sure there's no 0 weight
   The returned matrix is a V * V matrix,
   where dist[i][j] is the shortest distance from vertex i to vertex j,
   we use VALUE_ERROR to represent unreachable in the final int matrix,
   Remember to free the returned matrix after use
*/
static inline int** floydwarshall_sp_matrix(Graph_matrix* graph)
{
    // ERROR checking: graph should not be NULL and should have at least 1 vertex
    if (!graph) { printf("[ERROR] floydwarshall_sp_matrix: graph is NULL\n"); return NULL; }
    if (graph->V <= 0)
    {
        printf("[ERROR] floydwarshall_sp_matrix: graph should have at least 1 vertex\n");
        return NULL;
    }
    if (!graph->matrix)
    {
        printf("[ERROR] floydwarshall_sp_matrix: graph->matrix is NULL\n");
        return NULL;
    }
    for (int i = 0; i < graph->V; ++i)
        if (!graph->matrix[i])
        {
            printf("[ERROR] floydwarshall_sp_matrix: graph->matrix[%d] is NULL\n", i);
            return NULL;
        }

    // allocate result matrix (V * V)
    // we use long long for intermediate calculations
    long long** dist = (long long**)malloc(graph->V * sizeof(long long*));
    if (!dist)
    {
        printf("[ERROR] floydwarshall_sp_matrix: malloc failed for row pointers\n");
        return NULL;
    }
    for (int i = 0; i < graph->V; ++i)
    {
        dist[i] = (long long*)malloc(graph->V * sizeof(long long));
        if (!dist[i])
        {
            printf("[ERROR] floydwarshall_sp_matrix: malloc failed for row %d\n", i);
            for (int r = 0; r < i; ++r) free(dist[r]);
            free(dist);
            return NULL;
        }
    }
    // allocate a final int matrix to return, we will convert long long dist back to int at the end
    int** final_dist = (int**)malloc(graph->V * sizeof(int*));
    if (!final_dist)    {
        printf("[ERROR] floydwarshall_sp_matrix: malloc failed for final_dist row pointers\n");
        for (int r = 0; r < graph->V; ++r) free(dist[r]);
        free(dist);
        return NULL;
    }
    for (int i = 0; i < graph->V; ++i)
    {
        final_dist[i] = (int*)malloc(graph->V * sizeof(int));
        if (!final_dist[i])        {
            printf("[ERROR] floydwarshall_sp_matrix: malloc failed for final_dist row %d\n", i);
            for (int r = 0; r < graph->V; ++r) free(dist[r]);
            free(dist);
            for (int r = 0; r < i; ++r) free(final_dist[r]);
            free(final_dist);
            return NULL;
        }
    }

    // A large value to represent infinity
    long long inf = LLONG_MAX / 4;

    // initialize distance matrix from adjacency matrix
    // NOTE:
    //   - dist[i][i] = 0 (distance to itself)
    //   - graph->matrix[i][j] != 0 means there is a directed edge i -> j
    //   - graph->matrix[i][j] == 0 means no edge, set to "infinity"
    for (int i = 0; i < graph->V; ++i)
        for (int j = 0; j < graph->V; ++j)
        {
            // if i == j, distance to itself is 0
            if (i == j) dist[i][j] = 0LL;
            // if there's an edge, assign the direct edge weight
            else if (graph->matrix[i][j] != 0)
                // assign direct edge weight
                dist[i][j] = (long long)graph->matrix[i][j];
            else dist[i][j] = inf;
        }

    // Floyd-Warshall core:
    // we try each vertex k as an intermediate vertex and relax all pairs (i, j)
    for (int k = 0; k < graph->V; ++k)
    {
        // relax all pairs (i, j) through intermediate vertex k
        for (int i = 0; i < graph->V; ++i)
            for (int j = 0; j < graph->V; ++j)
            {
                // if either side is "infinity", path i -> k -> j is not valid
                if (dist[i][k] == inf || dist[k][j] == inf) continue;
                // candidate path through k
                long long candidate = dist[i][k] + dist[k][j];
                // relax if better
                if (candidate < dist[i][j])
                    dist[i][j] = candidate;
            }

        // negative cycle detection:
        // if any dist[i][i] < 0 after processing intermediate vertex set {0..k},
        // then a negative cycle exists
        for (int i = 0; i < graph->V; ++i)
            if (dist[i][i] < 0)
            {
                printf("[ERROR] floydwarshall_sp_matrix: negative-weight cycle detected\n");
                for (int r = 0; r < graph->V; ++r) free(dist[r]);
                free(dist);
                for (int r = 0; r < graph->V; ++r) free(final_dist[r]);
                free(final_dist);
                return NULL;
            }
    }

    // convert long long dist back to int in final_dist,
    // with sanity check for overflow
    for (int i = 0; i < graph->V; ++i)
        for (int j = 0; j < graph->V; ++j)
        {
            if (dist[i][j] == inf) final_dist[i][j] = VALUE_ERROR;
            else if (dist[i][j] > INT_MAX || dist[i][j] <= VALUE_ERROR)
            {
                printf("[ERROR] floydwarshall_sp_matrix: distance from %d to %d exceeds int range\n", i, j);
                for (int r = 0; r < graph->V; ++r) free(dist[r]);
                free(dist);
                for (int r = 0; r < graph->V; ++r) free(final_dist[r]);
                free(final_dist);
                return NULL;
            }
            else final_dist[i][j] = (int)dist[i][j];
        }
    
    // free intermediate long long dist matrix
    for (int i = 0; i < graph->V; ++i) free(dist[i]);
    free(dist);

    // return the all-pairs shortest path matrix
    return final_dist;
}

#endif // _ALG_GRAPH_ENGINE_H_