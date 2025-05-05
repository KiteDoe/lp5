#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

class Graph {
private:
    int vertices;
    vector<vector<int>> adjList;

public:
    Graph(int v){
        this->vertices=v;
        adjList.resize(v);
    }

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // Sequential BFS
    void BFS(int start) {
        vector<bool> visited(vertices, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front(); 
            q.pop();
            cout << node << " ";

            for (int neighbor : adjList[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    // Parallel BFS
    void BFS_Parallel(int start) {
        vector<bool> visited(vertices, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front(); 
            q.pop();
            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < adjList[node].size(); ++i) {
                int neighbor = adjList[node][i];
                #pragma omp critical
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    // Sequential DFS
    void DFS(int start) {
        vector<bool> visited(vertices, false);
        stack<int> s;
        s.push(start);

        while (!s.empty()) {
            int node = s.top(); s.pop();
            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";

                for (int neighbor : adjList[node]) {
                    if (!visited[neighbor]) {
                        s.push(neighbor);
                    }
                }
            }
        }
        cout << endl;
    }

    // Parallel DFS
    void DFS_Parallel(int start) {
        vector<bool> visited(vertices, false);
        stack<int> s;
        s.push(start);

        #pragma omp parallel
        while (!s.empty()) {
            int node;
            #pragma omp critical
            if (!s.empty()) {
                node = s.top();
                s.pop();
            }

            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";

                #pragma omp parallel for
                for (int neighbor : adjList[node]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        s.push(neighbor);
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // BFS
    auto start = high_resolution_clock::now();
    cout << "Sequential BFS: "; g.BFS(0);
    auto end = high_resolution_clock::now();
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    start = high_resolution_clock::now();
    cout << "Parallel BFS: "; g.BFS_Parallel(0);
    end = high_resolution_clock::now();
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    // DFS
    start = high_resolution_clock::now();
    cout << "Sequential DFS: "; g.DFS(0);
    end = high_resolution_clock::now();
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    start = high_resolution_clock::now();
    cout << "Parallel DFS: "; g.DFS_Parallel(0);
    end = high_resolution_clock::now();
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    return 0;
}
