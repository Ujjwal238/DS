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

unordered_map<string, int> labelMap;  
int labelCounter = 1;                 

string hash_child_labels(const vector<int>& childLabels) {
    long long hashValue = 0;
    for (int label : childLabels) {
        hashValue += label * 31;  // Use a prime number multiplier for hashing
    }
    return to_string(hashValue);  // Return hash as string (or keep as int)
}

int label_tree(int node, const map<int, vector<int>>& rootedAdjList) {
    if (rootedAdjList.at(node).empty()) {
        return 0;  // Base case: return a fixed label for leaf nodes
    }

    vector<int> childLabels;
    
    // Recursively label the children
    for (int child : rootedAdjList.at(node)) {
        childLabels.push_back(label_tree(child, rootedAdjList));
    }
    
    // Hash the child labels instead of sorting
    string labelKey = hash_child_labels(childLabels);

    // Assign a unique label if we haven't seen this structure before
    if (labelMap.find(labelKey) == labelMap.end()) {
        labelMap[labelKey] = labelCounter++;
    }

    // Return the unique label for this node
    return labelMap[labelKey];
}


map<int, vector<int>> root_tree(map<int, vector<int>>& adjList, int root ) {

    map<int, vector<int>> rootedAdjList;
    
    set<int> visited;
    
   
    queue<int> q;
    q.push(root);
    visited.insert(root);
    
  
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        
        for (int neighbor : adjList[node]) {
            if (visited.find(neighbor) == visited.end()) {
                
                rootedAdjList[node].push_back(neighbor);
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
            if (rootedAdjList.find(node) == rootedAdjList.end()) {
            rootedAdjList[node] = {}; 
        }
    
    }
    
    return rootedAdjList;
}
vector<int> findTreeCenters(const map<int, vector<int>>& adjList) {
    unordered_map<int, int> degree;
    unordered_set<int> nodes;
    queue<int> leaves;
    int n = adjList.size();

   
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
            cout << "None";  
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
    int treeLabel1, treeLabel2 , treeLabel3,treeLabel4;
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
             printAdjacencyList(rootedTree1, "1");
             treeLabel1 = label_tree(centers1[0], rootedTree1);
              cout << "Labeled Tree1: " << treeLabel1 << endl;
             printAdjacencyList(rootedTree3, "3");
             treeLabel3 = label_tree(centers1[1], rootedTree1);
             cout << "Labeled Tree3: " << treeLabel3 << endl;
             printAdjacencyList(rootedTree2, "2");
            treeLabel2 = label_tree(centers2[0], rootedTree2);
            cout << "Labeled Tree2: " << treeLabel2 << endl;
             printAdjacencyList(rootedTree4, "4");
            treeLabel4 = label_tree(centers2[1], rootedTree2);
           
            cout << "Labeled Tree4: " << treeLabel4 << endl;

            
  
     
        
     
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
    printAdjacencyList(rootedTree1, "1");

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
