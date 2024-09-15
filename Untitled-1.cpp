#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

string labelTree(int node, int parent, const map<int, vector<int>>& graph) {
    vector<string> childLabels;
    
    for (int child : graph.at(node)) {
        if (child != parent) {
            childLabels.push_back(labelTree(child, node, graph));
        }
    }
    
    sort(childLabels.begin(), childLabels.end());
    
    string label = "(";
    for (const string& childLabel : childLabels) {
        label += childLabel;
    }
    label += ")";
    
    return label;
}

int main() {
    map<int, vector<int>> graph = {
        {1, {5, 3}},
        {2, {4}},
        {3, {4, 1, 7}},
        {4, {2, 3}},
        {5, {8, 1}},
        {6, {7}},
        {7, {3, 6}},
        {8, {5}}
    };

    int root = 3;  // Choosing 3 as the root
    string result = labelTree(root, -1, graph);
    cout << "Labeled tree: " << result << endl;

    return 0;
}