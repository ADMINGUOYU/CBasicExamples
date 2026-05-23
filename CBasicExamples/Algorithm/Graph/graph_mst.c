// Graph Minimum Spanning Tree (MST) algorithms - Prim's and Kruskal's
// Actual implementation resides in graph_engine.h

/*
Minimum Spanning Tree (MST):
 - A spanning tree of a graph is a subgraph that is a tree and connects all
   the vertices together.
 - A minimum spanning tree is a spanning tree with weight less than or equal
   to the weight of every other spanning tree. (smallest total edge weight)

BOTH ALGORITHMS ASSUME UNDIRECTED GRAPHS

Prim's Algorithm:
 - Prim's algorithm is a greedy algorithm that builds the MST by starting from
   an arbitrary vertex and repeatedly adding the smallest edge that connects
   a vertex in the tree to a vertex outside the tree until all vertices are
   included.
 - Time complexity: O(E log V) with a binary heap, O(V^2) with an adjacency
   matrix and no heap.
    - Explanation:
      - With a binary heap, MAXIMUM size: E (all edges) -> log E time for insert/extract
        For each vertex, we loop all its edges and insert them, that multiplies E,
        In total O(E log E) = O(E log V) since E is at most V^2.
        (O(log V^2) = O(log V))
      - With an adjacency matrix and no heap, we scan all vertices to find
        the minimum edge at each step, leading to O(V^2) time complexity. 
 - Space complexity: O(V) for the MST and O(V) for the priority queue.

Kruskal's Algorithm:
    - Kruskal's algorithm is a greedy algorithm that builds the MST by sorting all
      edges in non-decreasing order of their weight and repeatedly adding the
      smallest edge to the MST if it does not form a cycle until all vertices
      are included.
    - Time complexity: O(E log E) due to sorting the edges (you can use min-heap or
      things like merge sort), which is O(E log V) since E is at most V^2.
    - Space complexity: O(V) for the MST and O(E) for the edge list and union-find data structure.

Observations on Prim’s and Kruskal’s algorithms
Both algorithms are Greedy since they make the choice that looks best at the
moment
• Prim adds to MST lightest edge from 𝑆
• Kruskal adds to MST lightest edge that does not create a cycle.
For both Prim’s and Kruskal’s algorithm, we assumed that all the edges have
different weights.
If we remove this assumption (and allow some or even all edges to have the same
weight) the algorithms still work. The only thing that needs to be changed is
that, instead of choosing the smallest cost edge, we choose a smallest cost edge
(breaking ties arbitrarily).

Comparing Prim’s and Kruskal’s algorithms (running time)
 - Prim’s algorithm is faster than Kruskal’s algorithm when the graph is dense
 - Kruskal’s algorithm is faster than Prim’s algorithm when the graph is sparse
 - Both algorithms have the same running time when the graph is moderately dense
 WHY?
 - Kruskal indeed need to sort all the edges
 - Prim might not be overwhelmed by all the edges, it's incremental as it grows
   the connected component.
*/

// Include our graph engine
#include "graph_engine.h"

// Entry point
int alg_graph_mst_main(int argc, char* argv[])
{
    // Set up a simple undirected graph (in adjacency list representation)
    // NOTE: this example has TWO solutions equal to 37
    int num_vertices = 9;
    Graph_list* graph = create_graph_list(num_vertices);
    add_edge_undirected_list(graph, 0, 1, 4);
    add_edge_undirected_list(graph, 0, 7, 8);
    add_edge_undirected_list(graph, 1, 2, 8);
    add_edge_undirected_list(graph, 1, 7, 11);
    add_edge_undirected_list(graph, 2, 3, 7);
    add_edge_undirected_list(graph, 2, 5, 4);
    add_edge_undirected_list(graph, 2, 8, 2);
    add_edge_undirected_list(graph, 3, 4, 9);
    add_edge_undirected_list(graph, 3, 5, 14);
    add_edge_undirected_list(graph, 4, 5, 10);
    add_edge_undirected_list(graph, 5, 6, 2);
    add_edge_undirected_list(graph, 6, 7, 1);
    add_edge_undirected_list(graph, 6, 8, 6);
    add_edge_undirected_list(graph, 7, 8, 7);    

    // Perform Prim's MST algorithm starting from vertex 0
    Graph_tree* prim_mst_tree = prim_mst_list(graph, 0);
    // Perform Kruskal's MST algorithm starting from vertex 0
    Graph_tree* kruskal_mst_tree = kruskal_mst_list(graph, 0);

    // Get tree weight
    int prim_mst_weight = collect_total_edge_weight_list(prim_mst_tree, graph);
    int kruskal_mst_weight = collect_total_edge_weight_list(kruskal_mst_tree, graph);

    // Print results
    printf("\033[1;32m>>> Graph Minimum Spanning Tree (MST) >>>\033[0m\n");
    // print prim's MST total weight
    printf("  ├── \033[1mPrim's MST Total Weight   :\033[0m %d\n", prim_mst_weight);
    // print kruskal's MST total weight
    printf("  └── \033[1mKruskal's MST Total Weight:\033[0m %d\n", kruskal_mst_weight);
    // print the adjacency list of the original graph
    printf("\033[1;34mOriginal Graph (Adjacency List):\033[0m\n");
    print_graph_list(graph);
    // print the MST tree
    printf("\033[1;34mPrim's MST Tree:\033[0m\n");
    print_graph_tree(prim_mst_tree);
    printf("\033[1;34mKruskal's MST Tree:\033[0m\n");
    print_graph_tree(kruskal_mst_tree);

    // Clean up
    free_graph_list(graph);
    free_graph_tree(prim_mst_tree);
    free_graph_tree(kruskal_mst_tree);

    // Return
    return 0;
}