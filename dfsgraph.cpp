#include<iostream>
#include<vector>
#include<stack>
#include<omp.h>

using namespace std;

const int MAX = 100000;

// Adjacency List
vector<int> graph[MAX];

// Visited Array
bool visited[MAX];

//================ DFS FUNCTION =================
void dfs(int start_node)
{
    stack<int> s;

    s.push(start_node);

    cout << "\n\nDFS Traversal of Graph:\n";

    while(!s.empty())
    {
        int curr_node;

        // Pop node from stack
        curr_node = s.top();
        s.pop();

        // If node not visited
        if(!visited[curr_node])
        {
            visited[curr_node] = true;

            // Print traversal
            cout << curr_node << " ";

            // Parallel processing of adjacent nodes
#pragma omp parallel for
            for(int i = graph[curr_node].size()-1; i >= 0; i--)
            {
                int adj_node = graph[curr_node][i];

                if(!visited[adj_node])
                {
                    // Critical section for stack
#pragma omp critical
                    {
                        s.push(adj_node);
                    }
                }
            }
        }
    }

    cout << endl;
}

//================ DISPLAY GRAPH =================
void displayGraph(int n)
{
    cout << "\n\nGraph Representation (Adjacency List):\n";

    for(int i = 0; i < n; i++)
    {
        cout << i << " -> ";

        for(int j = 0; j < graph[i].size(); j++)
        {
            cout << graph[i][j] << " ";
        }

        cout << endl;
    }
}

//================ MAIN FUNCTION =================
int main()
{
    int n, m, start_node;

    cout << "Enter Number of Nodes, Edges and Start Node:\n";
    cin >> n >> m >> start_node;

    cout << "\nEnter Pair of Edges:\n";

    // Input edges
    for(int i = 0; i < m; i++)
    {
        int u, v;

        cin >> u >> v;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Initialize visited array
#pragma omp parallel for
    for(int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    // Display graph
    displayGraph(n);

    // DFS Traversal
    dfs(start_node);

    return 0;
}
