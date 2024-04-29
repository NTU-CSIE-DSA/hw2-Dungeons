#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef long long ll;
#define maxn (int)(1e6+10)

struct child_queue_node{
    struct child_queue_node* nxt;
    int c, d;
}*head[maxn], *tail[maxn];
struct child_queue_node* newChild(int c, int d){
    struct child_queue_node* t = (struct child_queue_node*) malloc(sizeof(struct child_queue_node));
    t->nxt = NULL;
    t->c = c;
    t->d = d;
    return t;
};

struct treasure_node{
    struct treasure_node *prv, *nxt;
    int neg_d;
    ll v;
};
struct treasure_node* newQueueNode(struct treasure_node* oriHead, int neg_d, ll v){
    struct treasure_node* t = (struct treasure_node*) malloc(sizeof(struct treasure_node));
    t->nxt = oriHead;
    t->neg_d = neg_d;
    t->v = v;
    return t;
}

struct queue_of_treasure{
    struct treasure_node *head, *tail;
    int top, bot;
}*cur_queue[maxn];
struct queue_of_treasure* newQueue(int neg_d, ll v, int dep){
    struct queue_of_treasure* t = (struct queue_of_treasure*) malloc(sizeof(struct queue_of_treasure));
    t->bot = t->top = dep;
    t->head = t->tail = newQueueNode(NULL, neg_d, v);
    t->head->nxt = t->head->prv = NULL;
    return t;
}

int stack[maxn];
ll dep[maxn];

void pop_front(struct queue_of_treasure* q){
    if (q->head == q->tail){
        free(q->head);
        cur_queue[q->bot] = NULL;
        free(q);
    }
    else{
        q->head = q->head->nxt;
        free(q->head->prv);
        q->head->prv = NULL;
        
        cur_queue[q->bot--] = NULL;
        cur_queue[q->bot] = q;
    }
}
void pop_back(struct queue_of_treasure* q){
    if (q->head == q->tail){
        free(q->head);
        cur_queue[q->bot] = NULL;
        free(q);
    }
    else{
        q->tail = q->tail->prv;
        free(q->tail->nxt);
        q->tail->nxt = NULL;
    }
}
void push_front(struct queue_of_treasure* q, int neg_d, ll v){
    --q->top;
    q->head = newQueueNode(q->head, neg_d, v);
    q->head->nxt->prv = q->head;
    
    if (q->top == 0){
        if (q->tail->v >= 0) printf("value remaining is %lld\n", q->tail->v);
        else printf("value lost at %d\n", q->tail->neg_d);
        pop_back(q);
        ++q->top;
    }
    else if (cur_queue[q->top] != NULL){
        push_front(cur_queue[q->top], q->tail->neg_d, q->tail->v);
        pop_back(q);
        q->tail->nxt = cur_queue[q->top]->head;
        cur_queue[q->top]->head->prv = q->tail;
        q->tail = cur_queue[q->top]->tail;
        q->top = cur_queue[q->top]->top;
        cur_queue[q->top] = q;
    }
    else cur_queue[q->top] = q;
}

struct max_child_deque_node{
    int c;
    ll mxv;
    struct max_child_deque_node *prv, *nxt;
}*mxc_head[maxn], *mxc_tail[maxn];
struct max_child_deque_node* newMaxChildNode(int c, ll v, struct max_child_deque_node *prv){
    struct max_child_deque_node* t = (struct max_child_deque_node*) malloc(sizeof(struct max_child_deque_node));
    t->c = c;
    t->mxv = v;
    t->prv = prv;
    t->nxt = NULL;
    return t;
}
void updatemx(int i, int c, ll v){
    while (mxc_tail[i] != NULL && v > mxc_tail[i]->mxv){
        struct max_child_deque_node* tmp = mxc_tail[i];
        mxc_tail[i] = mxc_tail[i]->prv;
        free(tmp);
    }

    if (mxc_tail[i] == NULL){
        mxc_head[i] = mxc_tail[i] = newMaxChildNode(c, v, NULL);
    }
    else{
        mxc_tail[i]->nxt = newMaxChildNode(c, v, mxc_tail[i]);
        mxc_tail[i] = mxc_tail[i]->nxt;
    }
}
ll dfs(int x){
    for (struct child_queue_node* c = head[x];c!=NULL;c=c->nxt){
        updatemx(x, c->c, c->d + dfs(c->c));
    }

    if (mxc_head[x] == NULL) return 0;
    else return mxc_head[x]->mxv;
}

int bin_search(ll x, int cur_dep){
    int l = 0, r = cur_dep;
    while (l != r){
        int m = (l + r) / 2;
        if (dep[m] >= x) r = m;
        else l = m + 1;
    }
    return l;
}

int main(){
    int n, m, q;

    scanf("%d%d%d", &n, &m, &q);
    for (int i=0;i<n;++i){
        cur_queue[i] = NULL;
        head[i] = tail[i] = NULL;
        mxc_head[i] = mxc_tail[i] = NULL;
    }
    while (m--){
        int u, v, w;

        scanf("%d%d%d", &u, &v, &w);
        if (head[u] == NULL){
            head[u] = tail[u] = newChild(v, w);
        }
        else{
            tail[u]->nxt = newChild(v, w);
            tail[u] = tail[u]->nxt;
        }
    }
    dfs(0);
    int cur = 0, cur_dep = 0;
    while (q--){
        int op;

        scanf("%d", &op);
        if (op == 1){
            if (head[cur] != NULL){
                printf("%d\n", head[cur]->c);
                dep[cur_dep+1] = dep[cur_dep] + head[cur]->d;
                cur = head[cur]->c;
                stack[++cur_dep] = cur;
            }
            else printf("-1\n");
        }
        else if (op == 2){
            if (cur){
                if (cur_queue[cur_dep] != NULL){
                    pop_front(cur_queue[cur_dep]);
                }

                cur = stack[--cur_dep];
                if (mxc_head[cur]->c == head[cur]->c){
                    struct max_child_deque_node* tmp = mxc_head[cur];
                    mxc_head[cur] = mxc_head[cur]->nxt;
                    free(tmp);

                    if (mxc_head[cur] == NULL) mxc_tail[cur] = NULL;
                    else mxc_head[cur]->prv = NULL;
                }
                struct child_queue_node* tmp = head[cur];
                head[cur] = head[cur]->nxt;
                free(tmp);
                printf("%d\n", cur);
            }
            else printf("-1\n");
        }
        else if (op == 3){
            ll t;

            scanf("%lld", &t);
            printf("%d\n", stack[bin_search(dep[cur_dep] - t, cur_dep)]);
        }
        else if (op == 4){
            if (head[cur] == NULL) printf("0\n");
            else {printf("%lld\n", mxc_head[cur]->mxv);}
        }
        else if (op == 5){
            ll p;

            scanf("%lld", &p);
            int neg_id = bin_search(dep[cur_dep] - p, cur_dep);
            p -= dep[cur_dep];
            if (cur_queue[cur_dep] == NULL){
                cur_queue[cur_dep] = newQueue(neg_id ? stack[neg_id - 1] : -1, p, cur_dep);
            }
            else{
                push_front(cur_queue[cur_dep], neg_id ? stack[neg_id - 1] : -1, p);
            }
        }
        else{
            int v, l;

            scanf("%d%d", &v, &l);
            if (head[cur] == NULL){
                head[cur] = tail[cur] = newChild(v, l);
            }
            else{
                tail[cur]->nxt = newChild(v, l);
                tail[cur] = tail[cur]->nxt;
            }
            updatemx(cur, v, l + dfs(v));
        }
    }
}