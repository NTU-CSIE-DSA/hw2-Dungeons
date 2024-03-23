#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>

#define MAXN 1000006

typedef long long ll;

typedef struct Vector {
    unsigned int len, size;
    ll *vec;
} Vector;

typedef struct List_node {
    int ord;
    ll v;
    struct List_node *prev, *nxt;
} List_node;

int N, M, Q;
int l[MAXN];
int parent[MAXN];
Vector chi[MAXN];
int chi_cur[MAXN];
List_node *llenh[MAXN], *llent[MAXN]; // longest length
ll len_sum[MAXN];
int path[MAXN];

bool treasure[MAXN];
int n_ts, waiting;
List_node *tsh, *tst;

int p_now;
int d_now;

void vec_init(Vector *new_vec) {
    new_vec->len = 1;
    new_vec->size = 0;
    new_vec->vec = (ll *) calloc(1, sizeof(ll));
}

void vec_pb(Vector *vec, ll val) {
    if (vec->size == vec->len) {
        vec->len *= 2;
        vec->vec = (ll *) realloc(vec->vec, vec->len * sizeof(ll));
    }
    vec->vec[vec->size] = val;
    vec->size++;
}

void llen_pb(int ind, int ord, ll dis) {
    List_node *new_node = (List_node *)calloc(1, sizeof(List_node));
    new_node->ord = ord;
    new_node->v = dis;
    while (llent[ind] && llent[ind]->v <= new_node->v) {
        List_node *tmp = llent[ind];
        llent[ind] = llent[ind]->prev;
        free(tmp);
    }
    if (llent[ind] == NULL) {
        llenh[ind] = new_node;
        llent[ind] = new_node;
        return;
    }
    llent[ind]->nxt = new_node;
    new_node->prev = llent[ind];
    assert(llent[ind]->ord < new_node->ord);
    llent[ind] = new_node;
}

ll dfs(int ind) {
    if (chi[ind].size == 0) return 0;
    for (int i = 0; i < (int)chi[ind].size; i++)
        llen_pb(ind, i, l[chi[ind].vec[i]] + dfs((int)chi[ind].vec[i]));
    return llenh[ind]->v;
}


signed main() {
    for (int i = 0; i < MAXN; i++) vec_init(&chi[i]);

    scanf("%d%d%d", &N, &M, &Q);

    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        scanf("%d", &l[v]);
        vec_pb(&chi[u], v);
        parent[v] = u;
    }
    dfs(0);
    while (Q--) {
        int instruction;
        scanf("%d", &instruction);
        if (instruction == 1) {
            if (chi_cur[p_now] == (int)chi[p_now].size) {
                printf("-1\n");
                continue;
            }
            int p_new = (int)chi[p_now].vec[chi_cur[p_now]];
            chi_cur[p_now]++;
            p_now = p_new;
            printf("%d\n", p_now);
            d_now++;
            len_sum[d_now] = len_sum[d_now-1] + l[p_now];
            path[d_now] = p_now;
        } else if (instruction == 2) {
            if (p_now == 0) {
                assert(waiting == 0 && n_ts == 0);
                assert(tsh == NULL && tst == NULL);
                printf("-1\n");
                continue;
            }
            if (treasure[p_now]) {
                assert(tsh && tst);
                List_node *tmp = tst;
                tst = tst->prev;
                if (tst == NULL) tsh = NULL;
                else tst->nxt = NULL;
                free(tmp);
                n_ts--;
            }

            p_now = parent[p_now];
            printf("%d\n", p_now);
            d_now--;

            if (p_now == 0) assert(waiting == 0);

            if (waiting > 0 && !treasure[p_now]) {
                assert(tsh && tst);
                treasure[p_now] = 1;
                waiting--;
            }
        } else if (instruction == 3) {
            ll t;
            scanf("%lld", &t);
            int L = 0, R = d_now;
            while (L != R) {
                int m = (L+R)/2;
                if (t < len_sum[d_now] - len_sum[m]) L = m+1;
                else R = m;
            }
            // printf("%d: ", L);
            printf("%d\n", path[L]);
        } else if (instruction == 4) {
            while (llenh[p_now] && chi_cur[p_now] > llenh[p_now]->ord) {
                List_node *tmp = llenh[p_now];
                llenh[p_now] = llenh[p_now]->nxt;
                free(tmp);
            }
            if (llenh[p_now] == NULL) {
                printf("0\n");
                continue;
            }
            printf("%lld\n", llenh[p_now]->v);
        } else if (instruction == 5) {
            ll p;
            scanf("%lld", &p);
            if (p_now == 0) {
                printf("0 %lld\n", p);
                continue;
            }
            List_node *new_node = (List_node *)calloc(1, sizeof(List_node));
            new_node->ord = p_now;
            new_node->v = p - len_sum[d_now];
            if (tst == NULL) {
                tsh = new_node;
                tst = new_node;
            } else {
                tst->nxt = new_node;
                new_node->prev = tst;
                tst = new_node;
            }
            n_ts++;
            if (treasure[p_now]) waiting++;
            if (n_ts > d_now) {
                assert(treasure[p_now]);
                assert(tsh != NULL);
                printf("%d %lld\n", tsh->ord, tsh->v);
                List_node *tmp = tsh;
                tsh = tsh->nxt;
                if (tsh == NULL) tst = NULL;
                else tsh->prev = NULL;
                free(tmp);
                assert(waiting > 0);
                waiting--;
                n_ts--;
                assert(n_ts == d_now);
            }
            treasure[p_now] = 1;
        } else assert(false);
    }
    return 0;
}
