#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestCycleInGraph(vector<vector<int>>& adjMatrix) {
        int n = adjMatrix.size();
        vector<int> visited(n, 0);
        vector<int> path_visited(n, 0);
        vector<int> entry_time(n, 0);
        int time = 0;
        int ans = -1;

        function<void(int)> dfs = [&](int node) {
            visited[node] = 1;
            path_visited[node] = 1;
            entry_time[node] = ++time;

            for (int v = 0; v < n; ++v) {
                if (adjMatrix[node][v] == 1) { // There's an edge
                    if (!visited[v]) {
                        dfs(v);
                    } else if (path_visited[v]) {
                        // Found a cycle
                        ans = max(ans, entry_time[node] - entry_time[v] + 1);
                    }
                }
            }

            path_visited[node] = 0; // Backtrack
        };

        // Loop over all nodes to find the longest cycle
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs(i);
            }
        }

        return ans;
    }


    pair<int, int> longestCycle(vector<vector<int>>& adjMatrix1, vector<vector<int>>& adjMatrix2) {
        // Find the longest cycle in both adjacency matrices
        int cycleLength1 = longestCycleInGraph(adjMatrix1);
        int cycleLength2 = longestCycleInGraph(adjMatrix2);

        return {cycleLength1, cycleLength2};
    }

};

vector<vector<int>> readAdjMatrix(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    int n;
    file >> n;
    if (file.fail()) {
        cerr << "Error reading size from file: " << filename << endl;
        exit(1);
    }
    vector<vector<int>> adjMatrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> adjMatrix[i][j];
            if (file.fail()) {
                cerr << "Error reading matrix element from file: " << filename << endl;
                exit(1);
            }
        }
    }
    return adjMatrix;
}
// Main function to test the code
int main() {
    // Example input: two adjacency matrices
       vector<vector<int>> adjMatrix1 = readAdjMatrix("input.txt");
    vector<vector<int>> adjMatrix2 = readAdjMatrix("input2.txt");
     
    Solution solution;
    pair<int, int> result = solution.longestCycle(adjMatrix1, adjMatrix2);
    
    cout << "Length of the longest cycle in Graph 1: " << result.first << endl;
    cout << "Length of the longest cycle in Graph 2: " << result.second << endl;

    return 0;
}
