/*Design and implement Parallel Breadth First Search and Depth First Search based on existing 
algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS .*/


#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

struct Graph {
    int V;
    vector<vector<int>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (!visited[v]) {
                    #pragma omp critical
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        cout << endl;
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        #pragma omp parallel     //creates a team of threads.
        {
            #pragma omp single nowait
            // ensures that only one thread (without waiting for others) invokes the recursive DFSUtil() function. 
            // This is important so that the recursion starts only once.
            {
                DFSUtil(start, visited);
            }
        }
        cout << endl;
    }

    void DFSUtil(int u, vector<bool>& visited) {
        #pragma omp critical   //Ensures only one thread updates visited[u] at a time to avoid race conditions.
        {
            if (visited[u]) return;
            visited[u] = true;
            cout << u << " ";
        }

        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            #pragma omp task  //Allows parallel recursive calls for each unvisited neighbor
            DFSUtil(v, visited);
        }

        #pragma omp taskwait  //Ensures all child tasks finish before the parent continues (to maintain DFS correctness).
    }
};

int main() {
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V);

    int edgeCount;
    cout << "Enter the number of edges: ";
    cin >> edgeCount;

    cout << "Enter the edges (in format 'source destination'): \n";
    for (int i = 0; i < edgeCount; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "BFS traversal starting from node 0: ";
    g.BFS(0);

    cout << "DFS traversal starting from node 0: ";
    g.DFS(0);

    return 0;
}


/*
What You Should Say to the Examiner
This code attempts to parallelize DFS using OpenMP.

DFS is inherently a recursive, depth-first process, which is difficult to parallelize efficiently.

The use of #pragma omp parallel and #pragma omp single ensures only one thread starts the recursion.

#pragma omp parallel for is used to explore multiple neighbors in parallel, but it can lead to race conditions unless proper synchronization (like critical sections or atomic operations) is used.

A better parallel strategy for DFS is using task-based parallelism (with #pragma omp task and #pragma omp taskwait), which avoids such race conditions.
*/