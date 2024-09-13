#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

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
        }
    }

    file.close();
}

int main() {
    unordered_map<int, vector<int>> adjList1;
    unordered_map<int, vector<int>> adjList2;


    // Read from input.txt
    readFileToAdjList("input.txt", adjList1);

    // Read from input2.txt
    readFileToAdjList("input2.txt", adjList2);

    // Print the adjacency list
    for (const auto& pair : adjList1) {
        cout << pair.first << ": ";
        for (int neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }

    return 0;
}