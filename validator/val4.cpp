#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(){
    registerValidation();
    int n = inf.readInt(2, 1e6, "n");
    inf.readSpace();
    int m = inf.readInt(n-1, n-1, "m");
    inf.readSpace();
    int q = inf.readInt(1, 1e6, "q");
    inf.readEoln();
    set <int> v_list;
    vector<queue<int, list<int>>> adj(n);
    while (m--){
        int u = inf.readInt(0, n-2, "u_i");
        inf.readSpace();
        int v = inf.readInt(u+1, n-1, "v_i");
        inf.readSpace();
        ensure(v_list.find(v) == v_list.end());
        v_list.insert(v);
        adj[u].push(v);
        int w = inf.readInt(1, 1e9, "w_i");
        inf.readEoln();
    }

    stack <int, vector<int>> sk;
    int cur = 0;
    sk.push(0);
    while (q--){
        int op = inf.readInt(1, 5, "op_i");

        if (op == 1){
            if (!adj[cur].empty()){
                cur = adj[cur].front();
                sk.push(cur);
            }
        }
        else if (op == 2){
            if (cur){
                sk.pop();
                cur = sk.top();
                adj[cur].pop();
            }
        }
        else if (op == 5){
            inf.readSpace();
            long long p = inf.readLong(0, (long long)1e18, "p_i");
        }
        else ensure(false);
        inf.readEoln();
    }
    inf.readEof();

    return 0;
}