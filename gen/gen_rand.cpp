// #include "testlib.h"
#include "jngen.h"
#include <bits/stdc++.h>
using namespace std;
using namespace jngen;

bool inside(int x, TArray<int> a){
    for (auto &t:a) if (x == t) return true;
    return false;
}

int main(int argc, char* argv[]){
    registerGen(argc, argv, 1);

    int N = atoi(argv[1]), M = atoi(argv[2]), Q = atoi(argv[3]);
    bool toMax = atoi(argv[4]), trvEnt = atoi(argv[12]);
    int adv_prob = atoi(argv[11]);
    TArray<int> opc;
    for (int i=1;i<=6;++i){
        for (int _=0;_<atoi(argv[i+4]);++_) opc.push_back(i);
    }

    int n = rnd.next(2, N);
    int m = rnd.next(0, min(M, n-1)), q = rnd.next(1, Q);
    if (toMax){
        n = N;
        m = M;
        q = Q;
    }
    cout << n << ' ' << m << ' ' << q << '\n';

    auto a = TArray<int>::randomUnique(m, n);
    cout << a.shuffle() << '\n';
    auto edges = TArray<tuple<int, int, int>>(n-1);
    for (int i=0;i<n-1;++i){
        get<1>(edges[i]) = i+1;
        get<0>(edges[i]) = rnd.next(0, i);
        get<2>(edges[i]) = rnd.next(1, (int)1e9);
    }
    edges = edges.shuffle();
    vector<queue<pair<int,int>, list<pair<int,int>>>> adj(n);
    for (int i=0;i<n-1;++i){
        cout << get<0>(edges[i]) << ' ' << get<1>(edges[i]) << ' ' << get<2>(edges[i]) << '\n';
        adj[get<0>(edges[i])].push(make_pair(get<1>(edges[i]), get<2>(edges[i])));
    }
    int cur = 0;
    long long cur_dep = 0;
    stack <int, vector<int>> sk;
    sk.push(0);
    set <int> anc;
    while (q--){
        int op = rnd.choice(opc);
        while ((op == 6 && anc.empty()) || (op == 5 && cur == 0) || (op == 2 && !adj[cur].empty())) op = rnd.choice(opc);
        // if (((op == 1 && adj[cur].empty()) || (op == 2 && cur == 0)) && rnd.next(100) > 0) op = rnd.next(1, 6);
        // while (!ok[subtask-1][op-1]) op = rnd.next(1, 6);
        
        cout << op;

        if (op == 1){
            if (!adj[cur].empty()){
                cur_dep += adj[cur].front().second;
                cur = adj[cur].front().first;
                sk.push(cur);
                if (inside(cur, a)) anc.insert(cur);
            }
        }
        else if (op == 2){
            if (cur){
                sk.pop();
                cur = sk.top();
                cur_dep -= adj[cur].front().second;
                adj[cur].pop();
                anc.erase(cur);
            }
        }
        else if (op == 3){
            cout << ' ' << rnd.next(0ll, cur_dep * 11 / 10);
        }
        else if (op == 4){}
        else if (op == 5){
            int d = cur;

            if (!anc.empty() && rnd.next(100) < adv_prob) d = rnd.choice(a);

            cout << ' ' << d << ' ' << rnd.next(1ll, (long long)1e18);
        }
        else{
            int d = rnd.choice(a);

            cout << ' ' << d << '\n';
        }

        cout << '\n';
    }
}