#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(){
    registerValidation();
    int n = inf.readInt(2, 1e6, "n");
    inf.readSpace();
    int m = inf.readInt(0, 20, "m");
    inf.readSpace();
    int q = inf.readInt(1, 1e6, "q");
    inf.readEoln();

    set <int> a_list;
    while (m--){
        int a = inf.readInt(1, n-1, "a_i");
        ensure(a_list.find(a) == a_list.end());
        a_list.insert(a);
        if (m > 0) inf.readSpace();
    }
    inf.readEoln();
    set <int> v_list;
    vector<queue<int, list<int>>> adj(n);
    for (int i=1;i<n;++i){
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
    ensure(*v_list.begin() == 1);
    ensure(*prev(v_list.end()) == n-1);

    set <int> anc;
    stack <int, vector<int>> sk;
    int cur = 0;
    anc.insert(0);
    sk.push(0);
    while (q--){
        int op = inf.readInt(1, 6, "op_i");

        if (op == 1){
            if (!adj[cur].empty()){
                cur = adj[cur].front();
                anc.insert(cur);
                sk.push(cur);
            }
        }
        else if (op == 2){
            if (cur){
                sk.pop();
                anc.erase(cur);
                cur = sk.top();
                adj[cur].pop();
            }
        }
        else if (op == 3){
            inf.readSpace();
            long long t = inf.readLong(0, (long long)1e18, "t_i");
        }
        else if (op == 4){}
        else if (op == 5){
            inf.readSpace();
            int d = inf.readInt(1, cur, "d_i");
            ensure(anc.find(d) != anc.end());
            ensure(d == cur || a_list.find(d) != a_list.end());

            inf.readSpace();
            long long p = inf.readLong(0, (long long)1e18, "p_i");
        }
        else if (op == 6){
            inf.readSpace();
            int d = inf.readInt(1, cur, "d_i");
            ensure(anc.find(d) != anc.end());
            ensure(a_list.find(d) != a_list.end());
        }
        inf.readEoln();
    }
    inf.readEof();

    return 0;
}