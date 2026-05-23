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
/* Print the whole tree (root first) */
static inline void print_graph_tree(Graph_tree* tree)
{
    if (!tree) { printf("[Graph Tree] (null)\n"); return; }
    printf("Graph Tree (root = %d):\n", tree->root ? tree->root->vertex : -1);
    if (tree->root) print_tree_node_recursively(tree->root, 0);
}

#endif // _ALG_GRAPH_ENGINE_H_