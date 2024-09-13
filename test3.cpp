#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <cmath>
#include <map>
#include <random>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <limits.h>
 
using namespace std;
const int MOD = 1e9 + 9;
 
class Tree {
public:
    vector<vector<int>> adj;
    vector<int> centroid;
    vector<int> sub;
    vector<int> id;
    vector<int64_t> powr;
 
    void dfs(int curNode, int prevNode) {
        sub[curNode] = 1;
        bool is_centroid = true;
        vector<pair<int, int>> nodes;
        for (int v: adj[curNode]) {
            if (v != prevNode) {
                dfs(v, curNode);
                sub[curNode] += sub[v];
                if (sub[v] > (int) adj.size() / 2) {
                    is_centroid = false;
                }
                nodes.emplace_back(id[v], v);
            }
        }
        sort(nodes.begin(), nodes.end());
        id[curNode] = 1;
        for (auto& p: nodes) {
            id[curNode] = ((powr[sub[p.second] + 1] * id[curNode]) % MOD + id[p.second]) % MOD;
        }
        id[curNode] *= 2;
        id[curNode] %= MOD;
        if ((int) adj.size() - sub[curNode] > (int) adj.size() / 2) {
            is_centroid = false;
        }
        if (is_centroid) {
            centroid.push_back(curNode);
        }
    }
 
    vector<int> Centroid() {
        dfs(0, -1);
        return centroid;
    }
 
    bool isIsomorphic(int root1, Tree t2, int root2) {
        dfs(root1, root1);
        t2.dfs(root2, root2);
        sort(sub.begin(), sub.end());
        sort(t2.sub.begin(), t2.sub.end());
        return (id[root1] == t2.id[root2]);
    }
 
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    Tree(int n) {
        adj.resize(n);
        sub.resize(n);
        id.resize(n);
        powr.resize(n + 1);
        powr[0] = 1;
        for (int i = 1; i <= n; i++) {
            powr[i] = 2 * powr[i - 1];
            powr[i] %= MOD;
        }
    }
};
 
int main() {
    // Example input
    vector<pair<int, vector<pair<int, int>>>> test_cases = {
        {5, {{1, 2}, {1, 3}, {2, 4}, {2, 5}}},  // First tree
        {5, {{1, 2}, {2, 3}, {3, 4}, {4, 5}}},  // Second tree (not isomorphic to first)
        {5, {{1, 2}, {1, 3}, {3, 4}, {3, 5}}},  // Third tree (isomorphic to first)
    };

    for (int t = 0; t < test_cases.size(); t += 2) {
        int N = test_cases[t].first;
        Tree t1(N), t2(N);
        
        // Build first tree
        for (auto& edge : test_cases[t].second) {
            t1.add_edge(edge.first - 1, edge.second - 1);
        }
        
        // Build second tree
        for (auto& edge : test_cases[t+1].second) {
            t2.add_edge(edge.first - 1, edge.second - 1);
        }

        vector<int> c1 = t1.Centroid();
        vector<int> c2 = t2.Centroid();
        bool isomorphic = false;
        
        for (int i : c1) {
            for (int j : c2) {
                if (t1.isIsomorphic(i, t2, j)) {
                    isomorphic = true;
                    break;
                }
            }
            if (isomorphic) break;
        }

        cout << "Trees " << t/2 + 1 << " and " << t/2 + 2 << " are " 
             << (isomorphic ? "isomorphic" : "not isomorphic") << endl;
    }

    return 0;
}