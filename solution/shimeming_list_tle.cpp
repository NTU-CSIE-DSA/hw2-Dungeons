#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>

#define MAXN 1000006

typedef long long ll;

typedef struct List_node {
  int ord;
  ll v;
  struct List_node *prev, *nxt;
} List_node;

int N, M, Q;
int l[MAXN];
int parent[MAXN];
List_node *chih[MAXN], *chit[MAXN];
List_node *chi_cur[MAXN];
List_node *llenh[MAXN], *llent[MAXN]; // longest length
ll len_sum[MAXN];
int path[MAXN];

bool treasure[MAXN];
int n_ts, waiting[MAXN];
List_node *tsh, *tst;

int p_now;
int d_now;

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
  if (chih[ind] == NULL) return 0;
  List_node *it = chih[ind];
  while (it) {
    llen_pb(ind, it->ord, l[it->v] + dfs((int)it->v));
    it = it->nxt;
  }
  return llenh[ind]->v;
}

void update(int ind) {
  if (ind == 0) return;
  while (llent[ind] && llent[ind]->prev && llent[ind]->v >= llent[ind]->prev->v) {
    List_node *tmp = llent[ind]->prev;
    llent[ind] = llent[ind]->prev;
    free(tmp);
  }
  update(parent[ind]);
}


signed main() {
  scanf("%d%d%d", &N, &M, &Q);

  for (int i = 0; i < M; i++) {
    int u, v;
    scanf("%d%d", &u, &v);
    scanf("%d", &l[v]);
    List_node *new_node = (List_node*)calloc(1, sizeof(List_node));
    new_node->v = v;
    if (chit[u]) {
      new_node->ord = chit[u]->ord+1;
      chit[u]->nxt = new_node;
      new_node->prev = chit[u];
    } else  {
      new_node->ord = 0;
      chih[u] = new_node;
    }
    chit[u] = new_node;
    parent[v] = u;
  }
  dfs(0);
  while (Q--) {
    int instruction;
    scanf("%d", &instruction);
    if (instruction == 1) {
      if (!chih[p_now] || chi_cur[p_now] == chit[p_now]) {
        printf("-1\n");
        continue;
      }
      if (chi_cur[p_now]) chi_cur[p_now] = chi_cur[p_now]->nxt;
      else chi_cur[p_now] = chih[p_now];
      p_now = (int)chi_cur[p_now]->v;
      printf("%d\n", p_now);
      d_now++;
      len_sum[d_now] = len_sum[d_now-1] + l[p_now];
      path[d_now] = p_now;
      assert(!treasure[p_now]);
    } else if (instruction == 2) {
      if (p_now == 0) {
        assert(n_ts == 0);
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

      int p_old = p_now;
      p_now = parent[p_now];
      printf("%d\n", p_now);
      d_now--;

      if (waiting[p_old] > 0 && !treasure[p_now]) {
        assert(tsh && tst);
        treasure[p_now] = 1;
        waiting[p_old]--;
      }
      waiting[p_now] += waiting[p_old];
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
      while (chi_cur[p_now] && llenh[p_now] && chi_cur[p_now]->ord >= llenh[p_now]->ord) {
        List_node *tmp = llenh[p_now];
        llenh[p_now] = llenh[p_now]->nxt;
        free(tmp);
      }
      if (llenh[p_now] == NULL) {
        llent[p_now] = NULL;
        printf("0\n");
        continue;
      } else {
        llenh[p_now]->prev = NULL;
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
      // new_node->ord = p_now;
      int L = 0, R = d_now;
      while (L != R) {
        int m = (L+R)/2;
        if (p < len_sum[d_now] - len_sum[m]) L = m+1;
        else R = m;
      }
      new_node->ord = parent[path[L]];
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
      if (treasure[p_now]) waiting[p_now]++;
      if (n_ts > d_now) {
        assert(treasure[p_now]);
        assert(tsh != NULL);
        if (tsh->v >= 0) printf("value remaining is %lld\n", tsh->v);
        else printf("value lost at %d\n", tsh->ord);
        // printf("%d %lld\n", tsh->ord, tsh->v);
        List_node *tmp = tsh;
        tsh = tsh->nxt;
        if (tsh == NULL) tst = NULL;
        else tsh->prev = NULL;
        free(tmp);
        waiting[p_now]--;
        n_ts--;
        assert(n_ts == d_now);
      }
      treasure[p_now] = 1;
    } else if (instruction == 6) {
      int v;
      scanf("%d", &v);
      scanf("%d", &l[v]);

      List_node *new_node = (List_node*)calloc(1, sizeof(List_node));
      new_node->v = v;
      if (chit[p_now]) {
        new_node->ord = chit[p_now]->ord+1;
        chit[p_now]->nxt = new_node;
        new_node->prev = chit[p_now];
      } else  {
        new_node->ord = 0;
        chih[p_now] = new_node;
      }
      chit[p_now] = new_node;
      parent[v] = p_now;

      llen_pb(p_now, chit[p_now]->ord, l[v] + dfs(v));
      update(p_now);
    }
  }
  return 0;
}
