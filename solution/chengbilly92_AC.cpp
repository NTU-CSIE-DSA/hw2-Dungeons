#include <stdio.h>
#include <stdlib.h>

typedef struct Listnode {
  long long val;
  struct Listnode *next;
  struct Listnode *prev;
}listnode;

typedef struct Linkedlist {
  listnode *head;
  listnode *tail;
  int size;
}linkedlist;

typedef struct Treenode {
  int parent;
  linkedlist childs, length, depth, depth_id, treasure_lost_pos, treasure_val, treasure_source;
}treenode;

treenode vertex[1000001];
int ancestors[1000001] = {};
long long node_depth[1000001] = {};
int node_level[1000001] = {};
long long total_length[1000001] = {};
int cur_vertex = 0;
int cur_level = 0;
long long cur_depth = 0;

void linkedlist_init(linkedlist *l) {
  l -> head = NULL;
  l -> tail = NULL;
  l -> size = 0;
}

void push_back(linkedlist *l, long long data) {
  listnode *new_node = (listnode*) malloc(sizeof(listnode));
  new_node -> val = data;
  new_node -> next = NULL;
  new_node -> prev = l -> tail;
  if(l -> head == NULL) {
    l -> head = new_node;
    l -> tail = new_node;
  }
  else {
    l -> tail -> next = new_node;
    l -> tail = new_node;
  }
  l -> size ++;
}

void push_front(linkedlist *l, long long data) {
  listnode *new_node = (listnode*) malloc(sizeof(listnode));
  new_node -> val = data;
  new_node -> next = l -> head;
  new_node -> prev = NULL;
  if(l -> head == NULL) {
    l -> head = new_node;
    l -> tail = new_node;
  }
  else {
    l -> head -> prev = new_node;
    l -> head = new_node;
  }
  l -> size ++;
}

void pop_front(linkedlist *l) {
  if(l -> head != NULL) {
    listnode *ori_head = l -> head;
    l -> head = l -> head -> next;
    if(l -> head == NULL) {
      l -> tail = NULL;
    }
    else {
      l -> head -> prev = NULL;
    }
    l -> size --;
    free(ori_head);
  }
}

void pop_back(linkedlist *l) {
  if(l -> tail != NULL) {
    listnode *ori_tail = l -> tail;
    l -> tail = l -> tail -> prev;
    if(l -> tail == NULL) {
      l -> head = NULL;
    }
    else {
      l -> tail -> next = NULL;
    }
    l -> size --;
    free(ori_tail);
  }
}

void merge(linkedlist *l1, linkedlist *l2) {
  if(l1 -> size == 0) {
    l1 -> head = l2 -> head;
    l1 -> tail = l2 -> tail;
    l1 -> size = l2 -> size;
    l2 -> size = 0;
    l2 -> head = NULL;
    l2 -> tail = NULL;
    return;
  }
  else if(l2 -> size == 0) {
    return;
  }
  l1 -> size += l2 -> size;
  l2 -> size = 0;
  l1 -> tail -> next = l2 -> head;
  l2 -> head -> prev = l1 -> tail;
  l1 -> tail = l2 -> tail;
  l2 -> head = NULL;
  l2 -> tail = NULL;
}

long long get_max_depth(int idx) {
  if(vertex[idx].depth.size > 0) {
    return vertex[idx].depth.head -> val;
  }
  return 0;
}

void dfs(int s) {
  listnode *cur = vertex[s].childs.head;
  listnode *len = vertex[s].length.head;
  while(cur != NULL) {
    dfs(cur -> val);
    long long max_depth = get_max_depth(cur -> val) + len -> val;
    while(vertex[s].depth.size > 0 && max_depth >= vertex[s].depth.tail -> val) {
      pop_back(&vertex[s].depth);
      pop_back(&vertex[s].depth_id);
    }
    push_back(&vertex[s].depth, max_depth);
    push_back(&vertex[s].depth_id, cur -> val);
    cur = cur -> next;
    len = len -> next;
  }
}

void add_edge(int u, int v, int l) {
  push_back(&vertex[u].childs, v);
  push_back(&vertex[u].length, l);
  vertex[v].parent = u;
  dfs(v);
  while(vertex[u].depth.size > 0 && (l + get_max_depth(v)) >= vertex[u].depth.tail -> val) {
    pop_back(&vertex[u].depth);
    pop_back(&vertex[u].depth_id);
  }
  push_back(&vertex[u].depth, l + get_max_depth(v));
  push_back(&vertex[u].depth_id, v);
}


int main (int argc, char** argv) {

  int n, m, q;
  scanf("%d%d%d", &n, &m, &q);
  for(int i = 0; i <= n; i++) {
    linkedlist_init(&vertex[i].childs);
    linkedlist_init(&vertex[i].length);
    linkedlist_init(&vertex[i].depth);
    linkedlist_init(&vertex[i].depth_id);
    linkedlist_init(&vertex[i].treasure_val);
    linkedlist_init(&vertex[i].treasure_source);
    linkedlist_init(&vertex[i].treasure_lost_pos);
    vertex[i].parent = -1;
  }
  for(int i = 0; i < m; i++) {
    int u, v, l;
    scanf("%d%d%d", &u, &v, &l);
    push_back(&vertex[u].childs, v);
    push_back(&vertex[u].length, l);
    vertex[v].parent = u;
  }
  dfs(0);
  for(int i = 0; i < q; i++) {
    int op;
    scanf("%d", &op);
    if(op == 1) {
      if(vertex[cur_vertex].childs.size > 0) {
        long long len = vertex[cur_vertex].length.head -> val;
        cur_vertex = vertex[cur_vertex].childs.head -> val;
        cur_level++;
        cur_depth += len;
        total_length[cur_level] = cur_depth;
        ancestors[cur_level] = cur_vertex;
        node_depth[cur_vertex] = cur_depth;
        node_level[cur_vertex] = cur_level;
        printf("%d\n", cur_vertex);
      }
      else {
        printf("-1\n");
      }
    }
    else if(op == 2) {
      int prev_idx = cur_vertex;
      if(vertex[cur_vertex].parent != -1) {
        cur_vertex = vertex[cur_vertex].parent;
        long long len = vertex[cur_vertex].length.head -> val;
        if(vertex[cur_vertex].depth_id.head -> val == prev_idx) {
          pop_front(&vertex[cur_vertex].depth);
          pop_front(&vertex[cur_vertex].depth_id);
        }
        pop_front(&vertex[cur_vertex].childs);
        pop_front(&vertex[cur_vertex].length);
        cur_level--;
        cur_depth -= len;
        if(vertex[prev_idx].treasure_lost_pos.size > 0) {
          pop_front(&vertex[prev_idx].treasure_lost_pos);
          pop_front(&vertex[prev_idx].treasure_val);
          pop_front(&vertex[prev_idx].treasure_source);
          merge(&vertex[cur_vertex].treasure_lost_pos, &vertex[prev_idx].treasure_lost_pos);
          merge(&vertex[cur_vertex].treasure_val, &vertex[prev_idx].treasure_val);
          merge(&vertex[cur_vertex].treasure_source, &vertex[prev_idx].treasure_source);
        }
        printf("%d\n", cur_vertex);
      }
      else{
        printf("-1\n");
      }
    }
    else if(op == 3) {
      long long t;
      scanf("%lld", &t);
      long long target = cur_depth - t;
      int l = -1, r = cur_level;
      while(l < r - 1) {
        int m = (l + r + 1) >> 1;
        if(total_length[m] >= target) {
          r = m;
        }
        else {
          l = m;
        }
      }
      printf("%d\n", ancestors[r]);
    }
    else if(op == 4) {
      printf("%lld\n", get_max_depth(cur_vertex));
    }
    else if(op == 5) {
      long long p;
      scanf("%lld", &p);
      int treasure_level = node_level[cur_vertex];
      int l = 0, r = treasure_level;
      while(l < r - 1) {
        int m = (l + r + 1) >> 1;
        if(total_length[treasure_level] - total_length[m] > p) {
          l = m;
        }
        else {
          r = m;
        }
      }
      push_front(&vertex[cur_vertex].treasure_lost_pos, ancestors[l]);
      push_front(&vertex[cur_vertex].treasure_val, p);
      push_front(&vertex[cur_vertex].treasure_source, cur_vertex);
      int idx = cur_level - vertex[cur_vertex].treasure_lost_pos.size;
      while(idx >= 0) {
        if(vertex[ancestors[idx]].treasure_lost_pos.size == 0) {
          break;
        }
        merge(&vertex[cur_vertex].treasure_lost_pos, &vertex[ancestors[idx]].treasure_lost_pos);
        merge(&vertex[cur_vertex].treasure_val, &vertex[ancestors[idx]].treasure_val);
        merge(&vertex[cur_vertex].treasure_source, &vertex[ancestors[idx]].treasure_source);
        idx = cur_level - vertex[cur_vertex].treasure_lost_pos.size;
      }
      if(idx == -1) {
        int source = vertex[cur_vertex].treasure_source.tail -> val;
        long long value = vertex[cur_vertex].treasure_val.tail -> val;
        if(value >= node_depth[source]) {
          printf("value remaining is %lld\n", value - node_depth[source]);
        }
        else {
          printf("value lost at %lld\n", vertex[cur_vertex].treasure_lost_pos.tail -> val);
        }
        pop_back(&vertex[cur_vertex].treasure_lost_pos);
        pop_back(&vertex[cur_vertex].treasure_val);
        pop_back(&vertex[cur_vertex].treasure_source);
      }
    }
    else {
      int v, l;
      scanf("%d%d", &v, &l);
      add_edge(cur_vertex, v, l);
    }
  }
  return 0;
}
