#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include<algorithm>
#include <unordered_set>
#include <unordered_map>
#include <set>

using namespace std;

string label_tree(int node, map<int, vector<int>>& rootedAdjList) {
    // If the node has no children (leaf), assign base label "0"
    if (rootedAdjList[node].empty()) {
        return "0";
    }
    
    // Vector to store labels of children
    vector<string> childLabels;
    
    // Recursively get the labels of children
    for (int child : rootedAdjList[node]) {
        childLabels.push_back(label_tree(child, rootedAdjList));
    }
    
    // Sort the children's labels lexicographically
    sort(childLabels.begin(), childLabels.end());
    
    // Combine the sorted labels to form a label for the current node
    string combinedLabel = "(";
    for (const string& label : childLabels) {
        combinedLabel += label + ",";
    }
    
    // Remove the last comma and close the parentheses
    if (!childLabels.empty()) {
        combinedLabel.pop_back(); // Remove the last comma
    }
    combinedLabel += ")";
    
    // Return the label for the current node
    return combinedLabel;
}

map<int, vector<int>> root_tree(map<int, vector<int>>& adjList, int root ) {
    // Select the first center (you could modify this for multiple centers)
    
    
    // Map to store the rooted adjacency list
    map<int, vector<int>> rootedAdjList;
    
    // Set to track visited nodes
    set<int> visited;
    
    // Use a queue for BFS or DFS
    queue<int> q;
    q.push(root);
    visited.insert(root);
    
    // Perform BFS to create a rooted tree
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        // Traverse all neighbors (children)
        for (int neighbor : adjList[node]) {
            if (visited.find(neighbor) == visited.end()) {
                // If the neighbor is not visited, it becomes a child of the current node
                rootedAdjList[node].push_back(neighbor);
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
            if (rootedAdjList.find(node) == rootedAdjList.end()) {
            rootedAdjList[node] = {};  // Add node with empty children list
        }
    
    }
    
    return rootedAdjList;
}
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
        if (pair.second.empty()) {
            cout << "None";  // If the node has no children, print "None"
        } else {
            for (size_t i = 0; i < pair.second.size(); ++i) {
                cout << pair.second[i];
                if (i < pair.second.size() - 1) {
                    cout << " -> ";
                }
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

    map<int, vector<int>> rootedTree1, rootedTree2, rootedTree3, rootedTree4;
    string treeLabel1, treeLabel2 , treeLabel3,treeLabel4;
    if(centers1.size()==1 && centers2.size()==1 ){
          rootedTree1 = root_tree(adjList1, centers1[0]);
          rootedTree2 = root_tree(adjList2, centers2[0]);
          treeLabel1 = label_tree(centers1[0], rootedTree1);
          treeLabel2 = label_tree(centers2[0], rootedTree2);
          





    }
      else if(centers1.size()==2 && centers2.size()==2 ){
             rootedTree1 = root_tree(adjList1, centers1[0]);
             rootedTree2 = root_tree(adjList2, centers2[0]);
             rootedTree3 = root_tree(adjList1, centers1[1]);
             rootedTree4 = root_tree(adjList2, centers2[1]);
             printAdjacencyList(rootedTree2, "3");
             printAdjacencyList(rootedTree3, "3");
             printAdjacencyList(rootedTree4, "3");
            treeLabel1 = label_tree(centers1[0], rootedTree1);
            treeLabel2 = label_tree(centers2[0], rootedTree2);
            treeLabel3 = label_tree(centers1[1], rootedTree1);
            treeLabel4 = label_tree(centers2[1], rootedTree2);
   cout << "Labeled Tree: " << treeLabel1 << endl;
     cout << "Labeled Tree: " << treeLabel2 << endl;
        cout << "Labeled Tree: " << treeLabel3 << endl;
     cout << "Labeled Tree: " << treeLabel4 << endl;

    }
        else if(centers1.size()==2 && centers2.size()==1 ){
             rootedTree1 = root_tree(adjList1, centers1[0]);
             rootedTree2 = root_tree(adjList2, centers2[0]);
             rootedTree3 = root_tree(adjList1, centers1[1]);
              treeLabel1 = label_tree(centers1[0], rootedTree1);
            treeLabel2 = label_tree(centers2[0], rootedTree2);
            treeLabel3 = label_tree(centers1[1], rootedTree1);
           
    


    }
        else if(centers1.size()==1 && centers2.size()==2 ){
             rootedTree1 = root_tree(adjList1, centers1[0]);
             rootedTree2 = root_tree(adjList2, centers2[0]);
             rootedTree4 = root_tree(adjList2, centers2[1]);
              treeLabel1 = label_tree(centers1[0], rootedTree1);
            treeLabel2 = label_tree(centers2[0], rootedTree2);
            treeLabel4 = label_tree(centers2[1], rootedTree2);


    }
    printAdjacencyList(rootedTree1, "3");

     cout << "Labeled Tree: " << treeLabel1 << endl;
     cout << "Labeled Tree: " << treeLabel2 << endl;
     
     if((treeLabel1==treeLabel2)||(treeLabel1==treeLabel4) || (treeLabel3==treeLabel2) ){
     cout<<"TREES ARE ISOMORPHIC";
     }
     else{
        cout<<"NOT ISOMORHIC";
     }
     

     
     


    return 0;
}
