#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "Parallel BFS: ";
        while (!q.empty()) {
            int sz = q.size();
            vector<int> nodes(sz);

            #pragma omp parallel for
            for (int i = 0; i < sz; ++i) {
                #pragma omp critical
                {
                    nodes[i] = q.front(); q.pop();
                    cout << nodes[i] << " ";
                }
            }

            for (int node : nodes) {
                for (int nbr : adj[node]) {
                    if (!visited[nbr]) {
                        #pragma omp critical
                        {
                            if (!visited[nbr]) {
                                visited[nbr] = true;
                                q.push(nbr);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        cout << "Parallel DFS: ";
        while (!s.empty()) {
            int sz = s.size();
            vector<int> nodes(sz);

            #pragma omp parallel for
            for (int i = 0; i < sz; ++i) {
                #pragma omp critical
                {
                    nodes[i] = s.top(); s.pop();
                    cout << nodes[i] << " ";
                }
            }

            for (int node : nodes) {
                for (int nbr : adj[node]) {
                    if (!visited[nbr]) {
                        #pragma omp critical
                        {
                            if (!visited[nbr]) {
                                visited[nbr] = true;
                                s.push(nbr);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    Graph g(V);
    cout << "Enter edges (u v):\n";
    for (int i = 0, u, v; i < E; ++i) {
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter start node for traversal: ";
    cin >> start;

    g.parallelBFS(start);
    g.parallelDFS(start);

    return 0;
}

//g++ -fopenmp L1.cpp -o L1
//./L1
