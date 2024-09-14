#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

// Function to read an adjacency list from a file
void readAdjList(const string& filename, map<int, vector<int>>& adjList) {
    ifstream infile(filename);
    int u;
    while (infile >> u) {
        string line;
        getline(infile, line);
        stringstream ss(line);
        int v;
        while (ss >> v) {
            adjList[u].push_back(v);
            adjList[v].push_back(u);  // Since this is an undirected tree
        }
    }
}

// Function to find the center of a tree using a BFS approach
vector<int> findTreeCenter(const map<int, vector<int>>& adjList) {
    map<int, int> degree;
    queue<int> leaves;
    int n = adjList.size();

    // Calculate the degree of each node
    for (auto& p : adjList) {
        degree[p.first] = p.second.size();
        if (degree[p.first] == 1) {
            leaves.push(p.first);
        }
    }

    int processed = 0;
    while (processed < n - 2) {
        int leafCount = leaves.size();
        processed += leafCount;

        for (int i = 0; i < leafCount; ++i) {
            int leaf = leaves.front();
            leaves.pop();

            for (int neighbor : adjList.at(leaf)) {
                degree[neighbor]--;
                if (degree[neighbor] == 1) {
                    leaves.push(neighbor);
                }
            }
        }
    }

    vector<int> centers;
    while (!leaves.empty()) {
        centers.push_back(leaves.front());
        leaves.pop();
    }
    return centers;
}

// Function to root the tree at a specific node
void rootTreeAt(int root, const map<int, vector<int>>& adjList, map<int, vector<int>>& rootedTree) {
    queue<int> q;
    map<int, bool> visited;
    
    q.push(root);
    visited[root] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : adjList.at(node)) {
            if (!visited[neighbor]) {
                rootedTree[node].push_back(neighbor);
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// Utility function to print a tree in a structured manner
void printTree(const map<int, vector<int>>& tree) {
    for (auto& node : tree) {
        cout << node.first << ": ";
        for (int neighbor : node.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

int main() {
    map<int, vector<int>> adjList1, adjList2;

    // Read adjacency list from both files
    readAdjList("input1.txt", adjList1);
    readAdjList("input2.txt", adjList2);

    // Find the center(s) of both trees
    vector<int> centers1 = findTreeCenter(adjList1);
    vector<int> centers2 = findTreeCenter(adjList2);

    // Print centers
    cout << "Centers of Tree 1: ";
    for (int center : centers1) {
        cout << center << " ";
    }
    cout << endl;

    cout << "Centers of Tree 2: ";
    for (int center : centers2) {
        cout << center << " ";
    }
    cout << endl;

    // Root the trees at the found centers
    map<int, vector<int>> rootedTree1, rootedTree2, rootedTree3, rootedTree4;

    if (centers1.size() == 1) {
        rootTreeAt(centers1[0], adjList1, rootedTree1);
        cout << "Rooted Tree 1 (Center at " << centers1[0] << "):" << endl;
        printTree(rootedTree1);
    } else if (centers1.size() == 2) {
        rootTreeAt(centers1[0], adjList1, rootedTree1);
        rootTreeAt(centers1[1], adjList1, rootedTree2);
        cout << "Rooted Tree 1 (Center at " << centers1[0] << "):" << endl;
        printTree(rootedTree1);
        cout << "Rooted Tree 2 (Center at " << centers1[1] << "):" << endl;
        printTree(rootedTree2);
    }

    if (centers2.size() == 1) {
        rootTreeAt(centers2[0], adjList2, rootedTree3);
        cout << "Rooted Tree 3 (Center at " << centers2[0] << "):" << endl;
        printTree(rootedTree3);
    } else if (centers2.size() == 2) {
        rootTreeAt(centers2[0], adjList2, rootedTree3);
        rootTreeAt(centers2[1], adjList2, rootedTree4);
        cout << "Rooted Tree 3 (Center at " << centers2[0] << "):" << endl;
        printTree(rootedTree3);
        cout << "Rooted Tree 4 (Center at " << centers2[1] << "):" << endl;
        printTree(rootedTree4);
    }

    return 0;
}
