// Graph Traversal Algorithms
// Actual implementation resides in graph_engine.h

/*
Breadth-First Search (BFS):
- BFS is a graph traversal algorithm that explores the graph level by level.
- It uses a queue data structure to keep track of the vertices to be explored next.
- BFS is particularly useful for finding the shortest path in an UNWEIGHTED graph. <- It gives you the smallest number of edges (depth) to the root
- Time complexity: O(V + E), where V is the number of vertices and E is the number of edges.
  - Explanation:
    - Each vertex is enqueued and dequeued at most once, contributing O(V).
    - Each edge is explored at most once when we check the adjacent vertices, contributing O(E).
    - Therefore, the overall time complexity is O(V + E).
    (Sum, for all vertex (1 + degree(vertex))) -> where SUM(degree(vertex)) = 2E for undirected graph, E for directed graph.
- Space complexity: O(V), due to the queue and the visited set.

Depth-First Search (DFS):
- DFS is a graph traversal algorithm that explores as far as possible along each branch before backtracking.
- It can be implemented using recursion or an explicit stack.
- DFS is useful for tasks like topological sorting, detecting cycles, and solving puzzles.
- Time complexity: O(V + E), where V is the number of vertices and E is the number of edges.
  - Explanation:
    - Each vertex is visited at most once, contributing O(V).
    - Each edge is explored at most once when we check the adjacent vertices, contributing O(E).
    - Therefore, the overall time complexity is O(V + E).
- Space complexity: O(V) in the worst case (when the graph is a linear chain), due to the recursion stack or explicit stack.
*/

// Include our graph engine
#include "graph_engine.h"

// Entry point
int alg_graph_traversal_main(int argc, char* argv[])
{
    // Set up a simple undirected graph (in adjacency list representation)
    int num_vertices = 10;
    Graph_list* graph = create_graph_list(num_vertices);
    add_edge_undirected_list(graph, 0, 1, 1);
    add_edge_undirected_list(graph, 0, 2, 1);
    add_edge_undirected_list(graph, 1, 3, 1);
    add_edge_undirected_list(graph, 1, 4, 1);
    add_edge_undirected_list(graph, 2, 5, 1);
    add_edge_undirected_list(graph, 2, 6, 1);
    add_edge_undirected_list(graph, 3, 7, 1);
    add_edge_undirected_list(graph, 4, 7, 1);
    add_edge_undirected_list(graph, 5, 8, 1);
    add_edge_undirected_list(graph, 6, 8, 1);
    add_edge_undirected_list(graph, 7, 9, 1);
    add_edge_undirected_list(graph, 8, 9, 1);

    // convert a set of adjacency matrix
    Graph_matrix* graph_matrix = list_to_matrix(graph);

    // Perform BFS and DFS traversals
    Graph_tree* bfs_tree = bfs_traversal_list(graph, 0); // BFS starting from vertex 0
    Graph_tree* dfs_tree = dfs_traversal_list(graph, 0); // DFS starting from vertex 0

    // Print results
    printf("\033[1;32m>>> Graph Traversal (Results) >>>\033[0m\n");
    // print the adjacency list of the original graph
    printf("\033[1;34mOriginal Graph (Adjacency List):\033[0m\n");
    print_graph_list(graph);
    // print the adjacency matrix of the original graph
    printf("\033[1;34mOriginal Graph (Adjacency Matrix):\033[0m\n");
    print_graph_matrix(graph_matrix);
    // print the BFS tree
    printf("\033[1;34mBFS Tree:\033[0m\n");
    print_graph_tree(bfs_tree);
    // print the DFS tree
    printf("\033[1;34mDFS Tree:\033[0m\n");
    print_graph_tree(dfs_tree);

    // Clean up
    free_graph_list(graph);
    free_graph_matrix(graph_matrix);
    free_graph_tree(bfs_tree);
    free_graph_tree(dfs_tree);

    // Return
    return 0;
}