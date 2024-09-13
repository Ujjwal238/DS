#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

using namespace std;

void readFileToAdjList(const string& filename, unordered_map<int, vector<int>>& adjList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int node;
        iss >> node;

        int neighbor;
        while (iss >> neighbor) {
            adjList[node].push_back(neighbor);
            adjList[neighbor].push_back(node);  // Ensure undirected edges
        }
    }

    file.close();
}

vector<int> findTreeCenters(const unordered_map<int, vector<int>>& tree) {
    int n = tree.size();
    vector<int> degree(n + 1, 0);
    queue<int> leaves;

    for (const auto& pair : tree) {
        int node = pair.first;
        degree[node] = pair.second.size();
        if (degree[node] <= 1) {
            leaves.push(node);
        }
    }

    int count = leaves.size();
    while (count < n - 2) {
        int leafCount = leaves.size();
        count += leafCount;

        for (int i = 0; i < leafCount; ++i) {
            int leaf = leaves.front();
            leaves.pop();

            for (int neighbor : tree.at(leaf)) {
                if (--degree[neighbor] == 1) {
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

map<int, vector<int>> constructRootedTree(const unordered_map<int, vector<int>>& tree, int root) {
    map<int, vector<int>> rootedTree;
    queue<int> q;
    unordered_set<int> visited;

    q.push(root);
    visited.insert(root);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : tree.at(node)) {
            if (visited.find(neighbor) == visited.end()) {
                rootedTree[node].push_back(neighbor);
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return rootedTree;
}

void printRootedTree(const map<int, vector<int>>& rootedTree, int node, int depth = 0) {
    cout << string(depth * 2, ' ') << node << endl;
    if (rootedTree.find(node) != rootedTree.end()) {
        for (int child : rootedTree.at(node)) {
            printRootedTree(rootedTree, child, depth + 1);
        }
    }
}

vector<unordered_map<int, vector<int>>> findForestComponents(const unordered_map<int, vector<int>>& adjList) {
    vector<unordered_map<int, vector<int>>> forest;
    unordered_set<int> visited;

    for (const auto& pair : adjList) {
        int start = pair.first;
        if (visited.find(start) == visited.end()) {
            unordered_map<int, vector<int>> component;
            queue<int> q;
            q.push(start);
            visited.insert(start);

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                for (int neighbor : adjList.at(node)) {
                    if (visited.find(neighbor) == visited.end()) {
                        component[node].push_back(neighbor);
                        component[neighbor].push_back(node);
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }

            forest.push_back(component);
        }
    }

    return forest;
}

int main() {
    unordered_map<int, vector<int>> adjList;

    // Read from input.txt and input2.txt
    readFileToAdjList("input.txt", adjList);
    readFileToAdjList("input2.txt", adjList);

    // Find forest components
    vector<unordered_map<int, vector<int>>> forest = findForestComponents(adjList);

    cout << "Forest contains " << forest.size() << " tree(s)." << endl << endl;

    // Process each tree in the forest
    for (int i = 0; i < forest.size(); ++i) {
        cout << "Tree " << i + 1 << ":" << endl;

        // Find the centers of the tree
        vector<int> centers = findTreeCenters(forest[i]);

        cout << "  Centers: ";
        for (int center : centers) {
            cout << center << " ";
        }
        cout << endl << endl;

        // Construct and print rooted tree for each center
        for (int center : centers) {
            cout << "  Rooted tree with center " << center << ":" << endl;
            map<int, vector<int>> rootedTree = constructRootedTree(forest[i], center);
            printRootedTree(rootedTree, center);
            cout << endl;
        }

        cout << "----------------------" << endl;
    }

    return 0;
}