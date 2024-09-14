#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

vector<int> findTreeCenters(const map<int, vector<int>>& adjList) {
    unordered_map<int, int> degree;
    unordered_set<int> nodes;
    queue<int> leaves;
    int n = adjList.size();

    // Calculate initial degrees and collect all nodes
    for (const auto& pair : adjList) {
        int node = pair.first;
        degree[node] = pair.second.size();
        nodes.insert(node);
        if (degree[node] <= 1) {
            leaves.push(node);
        }
    }

    int totalNodes = nodes.size();
    int processedCount = 0;

    while (processedCount < totalNodes - 2 && !leaves.empty()) {
        int leafCount = leaves.size();
        processedCount += leafCount;

        for (int i = 0; i < leafCount; ++i) {
            int leaf = leaves.front();
            leaves.pop();
            nodes.erase(leaf);

            // Decrease degree of the leaf's neighbors
            for (int neighbor : adjList.at(leaf)) {
                if (nodes.count(neighbor) > 0) {
                    degree[neighbor]--;
                    if (degree[neighbor] == 1) {
                        leaves.push(neighbor);
                    }
                }
            }
        }
    }

    // The remaining nodes are the centers
    return vector<int>(nodes.begin(), nodes.end());
}

map<int, vector<int>> readAdjacencyList(const string& filename) {
    map<int, vector<int>> adjList;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return adjList;
    }

    while (getline(file, line)) {
        istringstream iss(line);
        int node;
        iss >> node;
        
        int neighbor;
        while (iss >> neighbor) {
            adjList[node].push_back(neighbor);
        }
    }

    file.close();
    return adjList;
}

void printAdjacencyList(const map<int, vector<int>>& adjList, const string& listName) {
    cout << "Adjacency List " << listName << ":" << endl;
    for (const auto& pair : adjList) {
        cout << pair.first << ": ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            cout << pair.second[i];
            if (i < pair.second.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    map<int, vector<int>> adjList1 = readAdjacencyList("input.txt");
    map<int, vector<int>> adjList2 = readAdjacencyList("input2.txt");

    printAdjacencyList(adjList1, "1");
    printAdjacencyList(adjList2, "3");
     vector<int> centers1 = findTreeCenters(adjList1);
    vector<int> centers2 = findTreeCenters(adjList2);

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


    return 0;
}
