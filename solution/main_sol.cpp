#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef long long ll;
#define maxn (int)(1e6+10)

struct child_node{
    struct child_node* nxt;
    int c, d, mn;
}*head[maxn], *tail[maxn], *tmp[maxn];
struct child_node* newChild(int c, int d){
    struct child_node* t = (struct child_node*) malloc(sizeof(struct child_node));
    t->nxt = NULL;
    t->c = c;
    t->d = d;
    t->mn = d;
    return t;
};

struct queue_node{
    struct queue_node *prv, *nxt;
    int id;
    ll v;
};
struct queue_node* newQueueNode(struct queue_node* oriHead, int id, ll v){
    struct queue_node* t = (struct queue_node*) malloc(sizeof(struct queue_node));
    t->nxt = oriHead;
    t->id = id;
    t->v = v;
    return t;
}

struct que{
    struct queue_node *head, *tail;
    int top, bot;
}*cur_queue[maxn];
struct que* newQueue(int id, ll v, int dep){
    struct que* t = (struct que*) malloc(sizeof(struct que));
    t->bot = t->top = dep;
    t->head = t->tail = newQueueNode(NULL, id, v);
    t->head->nxt = t->head->prv = NULL;
    return t;
}

bool isSpe[maxn], dep_isSpe[maxn];
int sk[maxn], dep_of[maxn];
ll dep[maxn];

void pop_front(struct que* q){
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
void pop_back(struct que* q){
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
void push_front(struct que* q, int id, ll v){
    --q->top;
    q->head = newQueueNode(q->head, id, v);
    q->head->nxt->prv = q->head;
    
    if (q->top == 0 || dep_isSpe[q->top]){
        if (q->top == 0){
            printf("%d %lld\n", q->tail->id, q->tail->v);
        }
        else if (dep_isSpe[q->top]){
            if (cur_queue[q->top] == NULL){
                cur_queue[q->top] = newQueue(q->tail->id, q->tail->v, q->top);
            }
            else{
                push_front(cur_queue[q->top], q->tail->id, q->tail->v);
            }
        }
        
        pop_back(q);
        ++q->top;
    }
    else if (cur_queue[q->top] != NULL){
        push_front(cur_queue[q->top], q->tail->id, q->tail->v);
        pop_back(q);
        q->tail->nxt = cur_queue[q->top]->head;
        cur_queue[q->top]->head->prv = q->tail;
        q->tail = cur_queue[q->top]->tail;
        q->top = cur_queue[q->top]->top;
        cur_queue[q->top] = q;
    }
    else cur_queue[q->top] = q;
}

int main(){
    int n, m, q;

    scanf("%d%d%d", &n, &m, &q);
    for (int i=0;i<n;++i){
        cur_queue[i] = NULL;
        head[i] = tail[i] = NULL;
    }
    while (m--){
        int s;

        scanf("%d", &s);
        isSpe[s] = true;
    }
    for (int _=n;--_;){
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
    for (int i=0;i<n;++i){
        int j = 0;
        struct child_node* cur = head[i];

        while (cur != NULL){
            tmp[j++] = cur;
            cur = cur->nxt;
        }
        --j;
        while ((--j) >= 0){
            tmp[j]->mn = tmp[j]->mn < tmp[j+1]->mn ? tmp[j]->mn : tmp[j+1]->mn;
        }
    }
    int cur = 0, cur_dep = 0;
    for (int t=0;t<q;++t){
        int op;

        scanf("%d", &op);
        if (op == 1){
            if (head[cur] != NULL){
                printf("%d\n", head[cur]->c);
                dep[cur_dep+1] = dep[cur_dep] + head[cur]->d;
                cur = head[cur]->c;
                sk[++cur_dep] = cur;
                dep_of[cur] = cur_dep;
                dep_isSpe[cur_dep] = isSpe[cur];
            }
            else printf("-1\n");
        }
        else if (op == 2){
            if (cur){
                if (cur_queue[cur_dep] != NULL){
                    pop_front(cur_queue[cur_dep]);
                }

                cur = sk[--cur_dep];
                head[cur] = head[cur]->nxt;
                printf("%d\n", cur);
            }
            else printf("-1\n");
        }
        else if (op == 3){
            ll t;

            scanf("%lld", &t);
            t = dep[cur_dep] - t;
            int l = 0, r = cur_dep;
            while (l != r){
                int m = (l + r) / 2;
                if (dep[m] >= t) r = m;
                else l = m + 1;
            }
            printf("%d\n", sk[l]);
        }
        else if (op == 4){
            if (head[cur] == NULL) printf("-1\n");
            else printf("%d\n", head[cur]->mn);
        }
        else if (op == 5){
            int d;
            ll p;

            scanf("%d%lld", &d, &p);
            p -= dep[dep_of[d]];
            if (cur_queue[dep_of[d]] == NULL){
                cur_queue[dep_of[d]] = newQueue(t, p, dep_of[d]);
            }
            else{
                push_front(cur_queue[dep_of[d]], t, p);
            }
        }
        else if (op == 6){
            int d;

            scanf("%d", &d);
            if (cur_queue[dep_of[d]] == NULL)  printf("-1\n");
            else{
                printf("%d %lld\n", cur_queue[dep_of[d]]->head->id, cur_queue[dep_of[d]]->head->v + dep_of[d]);
            }
        }
    }
}