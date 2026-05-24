// Graph Shortest Path
// Actual implementation resides in graph_engine.h

/*
Shortest Path Problem
Input:
n Directed graph G = (V, E).
    – An undirected edge can be considered as two directed edges.
n Source s, destination t.
n Weight w(e) = length of edge e (w(e) can be negative)
Shortest path problem: Find the shortest path from s to t

Shortest Path Algorithms                       Rep         Time              Space
Bellman-Ford    Single-Source                  Adj List    O(VE)             O(V)
In DAG          Single-Source DAG              Adj List    O(V + E)          O(V)
Dijkstra        Single-Source Non-Neg Weights  Adj List    O((V + E) log V)  O(V)
All-Pairs 1     All-Pairs                      Adj Matrix  O(V^4)            O(V^2)
All-Pairs 2     All-Pairs                      Adj Matrix  O(V^3 log V)      O(V^2)
Floyd-Warshall  All Pairs                      Adj Matrix  O(V^3)            O(V^2)

Note. The shortest path problem is not well defined if the graph
contains negative-weight cycles.(Repeating C can create arbitrarily
negative s-t path.)
So we will always assume no negative cycles exist.

Subpath Optimality
Lemma (Cut and Paste Argument):
Let 𝑃 = (𝑠, ... , 𝑢, ... , 𝑡) be a shortest 𝑠 − 𝑡 path. Then the subpaths
𝑃1 = (𝑠, ... , 𝑢) and 𝑃2 = (𝑢, ... , 𝑡) must also be, respectively,
shortest 𝑠−𝑢 and 𝑢−𝑡 paths.
Pf: (by contradiction)
 -  Suppose the subpath 𝑃1 = (𝑠, ... , 𝑢) is not the shortest 𝑠−𝑢 path;
    i.e., there is another path 𝑃′1 from 𝑠 to 𝑢 that is shorter than 𝑃1.
 -  Then we can replace 𝑃1 with 𝑃′1,
    this creates 𝑃′ = 𝑃′1 𝑃2, a 𝑠−𝑡 path shorter than 𝑃.
 -  This contradicts the choice of 𝑃 as a shortest 𝑠−𝑡 path. Impossible!
 -  Same proof works for the subpath from 𝑢 to 𝑡.

Concept of Relaxation
Let 𝑣.𝑑 be shortest distance found so far from starting node 𝑠 to node 𝑣,
and
𝑣.𝑝 be the last node in the current shortest path from 𝑠 to node 𝑣.

Relaxing edge (𝑢, 𝑣) means checking whether taking shortest path to 𝑢
and then edge (𝑢, 𝑣) gives an even shorter path to 𝑣 improving known
shortest path to 𝑣.

Example:
S -> v.p -> v (total v.d currently)
We check if S -> (u.p) -> u -> v (total u.d + w(u, v)) is shorter than current v.d.
If it is, we update v.d and v.p to reflect the new shortest path.
<- We check if (u, v) contributes to a shorter path.

Pseudo-code for Relaxation:
Relax(𝑢, 𝑣)
If 𝑢.𝑑 + 𝑤(𝑢, 𝑣) < 𝑣.𝑑 Then
    𝑣.𝑑 = 𝑢.𝑑 + 𝑤(𝑢, 𝑣)
    𝑣.𝑝 = 𝑢

Bellman-Ford Algorithm (slowly connect to the center and actively adjusts)
 - Initially, we set 𝑣. 𝑑 = ∞ for all nodes,
   except the starting node 𝑠 for which 𝑠. 𝑑 = 0
 - Relax all edges once, in no particular order. (try all edges)
   After finishing, 𝑣.𝑑 < ∞ for all neighbors of 𝑠,
   or equivalently for all nodes that are connected with 𝑠 through
   a path with length 1 edge. (for others, 𝑣.𝑑 is still ∞)
 - Relax all edges a 2nd time (in no particular order).
   After finishing, 𝑣.𝑑 < ∞ for all nodes that can be reached from 𝑠
   through a path with length 1 or 2.
   A relaxation may only decrease distances so 𝑣.𝑑 is the shortest
   distance for paths with maximum length 2.
 - In general, after relaxing all edges for the 𝑖-th time,
   𝑣.𝑑 is the shortest distance for paths with maximum length 𝑖 edges.
 - Assuming no negative cycles, what is the max number of edges in a path?
   A path may have at most 𝑉 − 1 edges.
   Thus, after relaxing all edges 𝑉 − 1 times,
   𝑣.𝑑 is the actual shortest distance between 𝑣 and 𝑠.
Recurrence:
𝑣.𝑑[𝑖] = min 𝑢, (𝑢,𝑣) ∈ 𝐸 {𝑢.𝑑[𝑖 − 1] + 𝑤(𝑢, 𝑣)}
𝑣.𝑑[0] = ∞
Remark:
 - Can be run in parallel.
 - Used on massive graphs (even if no negative edges).
 - Can also detect whether there is a negative cycle.

Shortest path in a Directed Acyclic Graph (DAG)
𝛿(𝑠, 𝑣) will store shortest path distance from 𝑠 to 𝑣 
By subpath optimality, we have:
𝛿(𝑠, 𝑣) = min 𝑢, (𝑢,𝑣) ∈ 𝐸 {𝛿(𝑠, 𝑢) + 𝑤(𝑢, 𝑣)}
Unlike in Bellman-Ford, each edge will only be relaxed once.
    | WHY? Because the graph is a DAG, we can perform a topological
    |      sort of the vertices.
    | NO EDGE WILL COME BACK FROM THE FAR SIDE
    | (PREVIOUSLY, S <-> A <-> B <-> C <-> S, after first relaxation,
    | we targeted C <-> S we need more relaxation to find if
    | S <-> A <-> B <-> C is shorter)
    | YOU NEED ALL NEIGHBORS TO BE READY, MIGHT NOT IN TOPOLOGICAL ORDER
    | BUT IN DAG, ALL NEIGHBORS ARE READY WHEN YOU PROCESS A NODE
 - In a topologically sorted order, when we relax edge (𝑢, 𝑣),
   we have already computed the shortest path to 𝑢.
 - Therefore, we can compute the shortest path to 𝑣 in one pass
   through the edges in topological order.
We need to ensure that when 𝑣 is processed,
ALL 𝑢 with (𝑢, 𝑣) ∈ 𝐸 have already been processed,
so 𝛿(𝑠, 𝑢) holds the correct value when 𝑣 is processed,
Pseudo-code:
DAG-Shortest-Path(𝐺, 𝑠)
    topologically sort the vertices of 𝐺
    for each vertex 𝑣 ∈ 𝑉
        𝑣.𝑑 ← ∞
        𝑣.𝑝 ← 𝑛𝑖𝑙
        𝑠.𝑑 ← 0
    for each vertex 𝑢 in topological order
        for each vertex 𝑣 ∈ 𝐴𝑑𝑗[𝑢]
            if 𝑣.𝑑 > 𝑢.𝑑 + 𝑤(𝑢, 𝑣) then
                𝑣.𝑑 ← 𝑢.𝑑 + 𝑤(𝑢, 𝑣)
                𝑣.𝑝 ← 𝑢
Each vertex loop deg(𝑢) times, we count each outer loop 1.
Sigma(1 + deg(𝑢)) = O(V + E)
NOTE: as acyclic, we at most have V-1 edges, if lower,
      the graph may not be connected

Dijkstra's algorithm
In a graph with cycles and NONNEGATIVE weights
Maintain a set 𝑆 of explored nodes.
Initialize 𝑆 = {𝑠}, 𝑠.𝑑 = 0, 𝑣.𝑑 = ∞.
Assume we know, ∀𝑢 ∈ 𝑆, 𝑢.𝑑 = 𝛿(𝑠, 𝑢).
Key lemma: If all edges leaving 𝑆 were already relaxed, let 𝑣 be
the vertex in 𝑉 − 𝑆 with the minimum 𝑣.𝑑. Then 𝑣.𝑑 = 𝛿(𝑠, 𝑣),
 - This 𝑣 can then be added to 𝑆, and process repeated.
SIMILAR TO PRIM'S ALGORITHM, but we are looking for the minimum distance
to 𝑣, not the minimum weight of edge connecting 𝑆 and 𝑉 − 𝑆.
NOW, our recurrence is: if A -> B -> C is the shortest, A -> B is also
the shortest for B.
Why it doesn't work on negative weights?
    |------|  .d = 10
    |  S   | --------> []
    |      |        10 ↑↓ (1 OR -100)
    |      | --------> []
    |------|  .d = 9
    The above non-negative case will work, .d = 9 will be inserted
    if we've chosen .d = 10, then we have to further go down that 1,
    getting 11;
    However, if that is not 1 but -100, we should take this path instead
    of .d = 9 path.
ALSO, note that MST Prim is on UNDIRECTED graph

S-T shortest (NOT S -> ALL)
!!! Adding a heuristic to Dijkstra's algorithm gives us A* search
    We update the distance estimate to 𝑣 as 𝑣.𝑑 = 𝑢.𝑑 + 𝑤(𝑢, 𝑣) + ℎ(𝑣),
    where ℎ(𝑣) is a heuristic function that estimates the distance from 𝑣
    to the destination 𝑡.
    The heuristic function should be admissible,
    meaning it never overestimates the true distance to the destination.
!!! This reduces the number of nodes explored.
𝐴∗ can be used with any function ℎ provided that ℎ(𝑢, 𝑡) ≤ 𝛿(𝑢, 𝑡).
Faster than Dijkstra in practice, but asymptotically the same.

Bidirectional: start Dijkstra expansions from both 𝑠 and 𝑡 in parallel.
When you find a common node 𝑢 in both expansions, stop. The shortest
path has distance: 𝛿(𝑠, 𝑢) + 𝛿(𝑡, 𝑢).

All-Pairs Shortest Paths
Using previous algorithms
A) When there are no negative cost edges
    1. Run Dijkstra's algorithm from each vertex as the source.
       Time: O(V (V + E) log V) = O(V^2 log V + VE log V)
       If the graph is dense, this is O(V^3 log V)
B) When negative-weight edges are present
    1. Run Bellman-Ford algorithm from each vertex as the source.
       Time: O(V * VE)
       If the graph is dense, this is O(V^4)

Optimization:
A) Dynamic Programming: Solution 1
    Def: 𝑑𝑖𝑗^(𝑚) = length of the shortest path from 𝑖 to 𝑗 that
    contains AT MOST 𝑚 edges.
    Use 𝐷^(𝑚) to denote the matrix 𝑑𝑖𝑗^(𝑚)
    Recurrence:
        𝑑𝑖𝑗^(𝑚) = min 𝑘 {𝑑𝑖𝑘^(𝑚 − 1) + 𝑤(𝑘, 𝑗)} for 𝑘 in range [1, V] -> try every inter-connections
        𝑑𝑖𝑗^(1) = 𝑤(𝑖, 𝑗) if (𝑖, 𝑗) ∈ 𝐸, ∞ otherwise -> if there's an edge
    Time: O(V^3 * V) = O(V^4)
    Space: O(V^2 * V) = O(V^3) can be improved to O(V^2) by only keeping
        track of the previous iteration's matrix
B) Dynamic Programming: Solution 2 (skipping the middleman)
    Observation:
        To compute 𝑑𝑖𝑗^(𝑚), instead of looking at the last stop before 𝑗, we
        look at the middle point.
        This cuts down the problem size by half.
    New recurrence: (you use edge weight 𝑤(𝑘, 𝑗) NO MORE)
        𝑑𝑖𝑗^(2𝑚) = min 𝑘 {𝑑𝑖𝑘^(𝑚) + 𝑑𝑘𝑗^(𝑚)} for 𝑘 in range [1, V]
    Algorithm:
        1. Initialize 𝐷^(1) as before.
        2. For 𝑚 = 1, 2, 4, ..., while 𝑚 < 𝑉 − 1, compute 𝐷^(2𝑚) from 𝐷^(𝑚).
        We end at at the first iteration where 𝑚 >= 𝑉 − 1
        Overshoot doesn't matter, AT MOST 𝑚 edges, does not miss any shortest
        paths  at most 𝑉 − 1 edges.
        each matrix computation takes O(V^3) time, and we do O(log V) iterations,
        so total time is O(V^3 log V)
C) Solution 3: Floyd-Warshall
    Def:
        Def: 𝑑𝑖𝑗^(𝑘) = length of the shortest path from 𝑖 to 𝑗 that
        such that all INTERMEDIATE vertices on the path (if any) are
        in the set {1,2, ... , 𝑘}. (!!! NOT count of edges anymore)
    Initially: 𝑑𝑖𝑗^(0) = 𝑤(𝑖, 𝑗) [NO INTERMEDIATE VERTICES]
    Goal: 𝐷^(𝑛)
    Recurrence:
        𝑑𝑖𝑗^(𝑘) = min {𝑑𝑖𝑗^(𝑘 − 1), 𝑑𝑖𝑘^(𝑘 − 1) + 𝑑𝑘𝑗^(𝑘 − 1)}
                      OLD PATH      NEW PATH THROUGH 𝑘
    Analysis:
        We have 𝑉 iterations for 𝑘, and each iteration takes O(V^2) time to
        update the matrix, so total time is O(V^3).
        [inside the loop we don't try every vertex now, we just try the 𝑘-th]
        Space is O(V^3) for the matrix -> Improved to O(V^2) [modify matrix in-place].
    WHY we can modify in-place?
        When we compute 𝑑𝑖𝑗^(𝑘), we only need 𝑑𝑖𝑗^(𝑘 − 1), 𝑑𝑖𝑘^(𝑘 − 1) and
        𝑑𝑘𝑗^(𝑘 − 1). The first one is the current value in the matrix, and
        the values in the (k)-th row and (k)-th column remain exactly the
        same whether you read them from the previous matrix (k-1) or the
        current matrix (k).
        Since (column is similar):
            𝑑𝑘𝑗^(𝑘) = min {𝑑𝑘𝑗^(𝑘 − 1), 𝑑𝑘𝑘^(𝑘 − 1) + 𝑑𝑘𝑗^(𝑘 − 1)}
            where 𝑑𝑘𝑘^(𝑘 − 1) = 0 (no self-loop)
            -> 𝑑𝑘𝑗^(𝑘) = 𝑑𝑘𝑗^(𝑘 − 1)
    NEGATIVE CYCLE DETECTION:
        𝑑𝑖𝑖^(𝑘) = min {𝑑𝑖𝑖^(𝑘 - 1), 𝑑𝑖𝑘^(𝑘 - 1) + 𝑑𝑘𝑖^(𝑘 - 1)}
        The above formula should be 0 (no self-loop)
        PLUS, 𝑑𝑖𝑘^(𝑘 - 1) + 𝑑𝑘𝑖^(𝑘 - 1) should >= 0 in normal cases;
        IF 𝑑𝑖𝑘^(𝑘 - 1) + 𝑑𝑘𝑖^(𝑘 - 1) < 0, there's a negative loop
        As soon as we see 𝑑𝑖𝑖^(𝑘) < 0 at some 𝑘
        we can stop and report a negative cycle exists.
*/

// Include our graph engine
#include "graph_engine.h"

// Entry point
int alg_graph_shortest_path_main(int argc, char* argv[])
{
    // Set up a simple undirected graph (in adjacency list representation)
    int num_vertices = 8;
    Graph_list* graph = create_graph_list(num_vertices);
    add_edge_list(graph, 0, 1, 9);
    add_edge_list(graph, 0, 5, 14);
    add_edge_list(graph, 0, 6, 20);
    add_edge_list(graph, 1, 2, 23);
    add_edge_list(graph, 2, 4, 2);
    add_edge_list(graph, 2, 7, 19);
    add_edge_list(graph, 3, 2, 6);
    add_edge_list(graph, 3, 7, 6);
    add_edge_list(graph, 4, 3, 11);
    add_edge_list(graph, 4, 7, 16);
    add_edge_list(graph, 5, 2, 18);
    add_edge_list(graph, 5, 4, 31);
    add_edge_list(graph, 5, 6, 5);
    add_edge_list(graph, 6, 4, 20);
    add_edge_list(graph, 6, 7, 44);

    // Find shortest path using Bellman-Ford and Dijkstra's algorithms
    Graph_tree* bellmanford_tree = bellmanford_sp_list(graph, 0);
    Graph_tree* dijkstra_tree = dijkstra_sp_list(graph, 0);

    // Allocate buffers to store distance to every node for both algorithms
    int* bellmanford_dist = (int*)malloc(sizeof(int) * num_vertices);
    int* dijkstra_dist = (int*)malloc(sizeof(int) * num_vertices);

    // Get the distance to every node from the shortest path trees
    for (int v = 0; v < num_vertices; ++v)
    {
        bellmanford_dist[v] = collect_root_to_node_weight_list(bellmanford_tree, graph, v);
        dijkstra_dist[v] = collect_root_to_node_weight_list(dijkstra_tree, graph, v);
    }

    // Convert a matrix version of the graph for Floyd-Warshall
    Graph_matrix* graph_matrix = list_to_matrix(graph);
    // Run Floyd-Warshall for all-pairs shortest path matrix
    int ** floydwarshall_dist = floydwarshall_sp_matrix(graph_matrix);

    // Print results
    printf("\033[1;32m>>> Graph Shortest Path (Results) >>>\033[0m\n");
    printf("  ├── \033[1mBellman-Ford Distances:\033[0m ");
    for (int v = 0; v < num_vertices; ++v)
        printf("%d ", bellmanford_dist[v]);
    printf("\n");
    printf("  └── \033[1mDijkstra Distances    :\033[0m ");
    for (int v = 0; v < num_vertices; ++v)
        printf("%d ", dijkstra_dist[v]);
    printf("\n");
    // Print original graph
    printf("\033[1;34mOriginal Graph (Adjacency List):\033[0m\n");
    print_graph_list(graph);
    // Print the shortest path trees
    printf("\033[1;34mBellman-Ford Shortest Path Tree:\033[0m\n");
    print_graph_tree(bellmanford_tree);
    printf("\033[1;34mDijkstra Shortest Path Tree:\033[0m\n");
    print_graph_tree(dijkstra_tree);
    // Print Floyd-Warshall results
    printf("\033[1;34mFloyd-Warshall All-Pairs Shortest Path Matrix:\033[0m\n");
    // print header
    printf("FROM\\TO");
    for (int i = 0; i < num_vertices; ++i)
        printf("%2c\033[1m%2d\033[0m", ' ', i);
    printf("\n");
    for (int i = 0; i < num_vertices; ++i)
    {
        printf("%3c\033[1m%2d\033[0m%2c", ' ', i, ' ');
        for (int j = 0; j < num_vertices; ++j)
        {
            if (floydwarshall_dist[i][j] == VALUE_ERROR) printf("%c%3s", ' ', "INF");
            else printf("%c%3d", ' ', floydwarshall_dist[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    free(bellmanford_dist);
    free(dijkstra_dist);
    free_graph_tree(bellmanford_tree);
    free_graph_tree(dijkstra_tree);
    free_graph_list(graph);
    free_graph_matrix(graph_matrix);
    for (int i = 0; i < num_vertices; ++i) free(floydwarshall_dist[i]);
    free(floydwarshall_dist);

    return 0;
}