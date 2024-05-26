# DSA 2024 Homework 2 Programming Problem 2 (Dungeons)

# tutorial

## Basic Part

The structure of the dungeons is actually a tree rooted in $0$. The children of each node are given as a queue. Implement $n$ queues to store them. (Also doable with offline and allocating exact size arrays)

This can solve the first two operations.

While traversing, maintain a stack to record the current ancestry. Also record the depth of each node in the ancestry, which can be done by appending $stack.top() + edge\_length$ to the stack during operation 1, and removing $stack.top()$ during operation 2. Use an array to implement so that we can quickly find the value with an index. Use binary search on the depth stack to solve operation 3. Assume the current node is $n$, root is $r$, $x$ is the node on the path from $r$ to $n$. If $depth[n] - depth[x] < treasure\_value$, then the treasure will lose its value from $r$ to $x$, otherwise, it will lose its value from $x$ to $n$.

To solve operation 4, we can first find the furthest leaf from each node. All the leaf nodes have a value of $0$. Then, for all the non-leaf nodes, the answer is $\displaystyle\max_{c \in children} edge\_length(c) + furthest\_leaf(c)$. Finally, maintain a suffix maximum on the children queue implemented in the previous part. That is, assume the children of node $n$ are $c_1, c_2, \dots, c_k$ in order, then we can record:

$$
\begin{cases}
dis[k + 1] &= 0\\
dis[k] &= edge\_length(n, c_k) + furthest\_leaf(c_k)\\
dis[i] &= \max\{dis[i + 1], edge\_length(n, c_i) + furthest\_leaf(c_i)\}\ , \forall 1\leq i\leq k - 1
\end{cases}
$$

If the edge from $n$ to $c_1, c_2, \dots, c_i$ is removed, then return $dis[i + 1]$ when queried. Also, remember to update $furthest\_leaf(n)$ to $dis[i + 1]$ when operation 2 moves from $c_i$ to node $n$.

For operation 5, whether a treasure will lose complete value is similar to operation 3. Treasures in a consecutive range of dungeons will be moved together. Use linked-list-based queues to maintain a consecutive range of treasures. Also record the start and end node of each queue. When two queues meet, merge them. This can solve when the treasure is escorted out. If a treasure is escorted out when pushing a treasure to node $n$, then the queue starting from $n$ must end at the root. Pop the last element in the linked list and output it.

## Bonus Part

Now, the tree will be updated during the traversal, which will only affect operation 4. Since a new edge is always pushed into the end of the current node’s queue, we are actually pushing a new value into the end of a suffix-maximum problem. This is a classic sliding window maximum problem. Maintain a deque to solve the problem.

When appending a child after a node, we can ignore the children with less depth in the previous, as the new child will be removed after the previous children. Thus, we can maintain a queue with child order along with decreasing order of depth. When inserting a new child behind, remove the nodes with depth less than the new child in the queue (the last few elements). When removing a child, check whether the first element in the queue has the same index as the removed child. If so, remove the first element in the queue. When querying, output the depth of the first element.

The time complexity of initialize is $O(n)$, the time complexity for each operation $1, 2, 4, 6$ is $O(1)$, and for each operation $3, 5$ is $O(log(n))$, so the total time complexity is $O(n + q\log n)$

Reference: [Introduction to Monotonic Queues](https://www.geeksforgeeks.org/introduction-to-monotonic-queues/)

# Common Mistakes

+ Didn't properly handle the queue merge during operation 5.
+ Didn't consider that $v$ itself still has subtrees when performing operation 6.
+ Various strange pointers are poorly written. e.g. get the value from NULL pointer.
+ Didn't initialize the data, meaning you should make all the treasure queues empty at first and set $furthest\_ leaf$ to 0, then do a DFS to update $furthest\_ leaf$.
+ $O(n\log n)$ space complexity, it may lead to MLE.

# Coding Tips

+ This problem involves a lot of pointer operations, so make sure to understand how the pointers should be directed before starting to write the code.
+ There are many variables, so naming the variables properly can also help with debugging.


# sample code 

The code below is the copy of `./solution/readable_main_sol.cpp`. They are identical.

```c
/*
This is the main solution (readability enhanced version, REV) that will receive 120 pts.
The problem is based on traversing through a tree and conducting some operations.
Note that operation 3, 4, 5 are basically three independent problems and operation 1, 2 are the basics of tree traversal.
(Operation 5 is kinda based on operation 3 but the core of operation 5 is not.)
The bonus operation 6 only really affects operation 4 and my implementation leads to operation 3, 5 being completely
unaffected. However, depending on the approach, you might need to modify some code to support operation 3, 5 in bonus.

Anyway, enjoy more than 200 lines of code!
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxn (int)(1e6+10)

// Declaration of some structs that are going to be used

// The stack is an array-based stack that allows random-access to the current dfs stack.
int stack[maxn];

// This array stores the depth of each layer.
// Each layer is a node visited and stored in the previous stack and depth is the distance to root.
long long depth[maxn];

// This is the node to store the children of each parent node
struct child_queue_node{
    struct child_queue_node* nxt; // The next child
    int c, d; // The index of node of this child, the distance to that node
} *head[maxn], *tail[maxn];

struct child_queue_node* newChild(int c, int d){
    struct child_queue_node* t = (struct child_queue_node*) malloc(sizeof(struct child_queue_node));
    t -> nxt = NULL; // Newest child is always the last in the queue so it is initialized as NULL
    t -> c = c;
    t -> d = d;
    return t;
};

// This is the node of queues that are created to deal with operation 5
struct treasure_node{
    struct treasure_node *prv, *nxt; // Doubly-linked list is required
    int neg_d; // The position that the value of the treasure becomes negative
    long long v; // The remaining value of the treasure
};

struct treasure_node* newQueueNode(struct treasure_node* oriHead, int neg_d, long long v){
    struct treasure_node* t = (struct treasure_node*) malloc(sizeof(struct treasure_node));
    t -> nxt = oriHead; // The new treasure is always pushed into the head of the queue
    t -> neg_d = neg_d;
    t -> v = v;
    return t;
}

// This is the queue of treasures since there may be multiple queues
// Note that it is actually a deque. However, I found it easier to understand by regarding it as a queue since the 
// push_front() here is releatively irrelevant. If you find it weird that a queue have pop_front(),
// then you are actually correct. :D
struct queue_of_treasure{
    struct treasure_node *head, *tail; // The head and tail nodes
    int top, bot; // As a queue of treasures 
} *cur_queue[maxn]; // This array of pointer stores which queue each node belongs to

struct queue_of_treasure* newQueue(int neg_d, long long v, int dep){
    struct queue_of_treasure* t = (struct queue_of_treasure*) malloc(sizeof(struct queue_of_treasure));
    t -> bot = t -> top = dep;
    t -> head = t -> tail = newQueueNode(NULL, neg_d, v);
    t -> head -> nxt = t -> head -> prv = NULL;
    return t;
}

// Popping from the head (front) of a queue
void pop_front(struct queue_of_treasure* q){
    // If the head is the same as tail  ->  queue only consists of one node  ->  delete the entire queue
    if (q -> head == q -> tail){
        free(q -> head);
        cur_queue[q -> bot] = NULL;
        free(q);
    }
    // Remove the head
    else{
        q -> head = q -> head -> nxt;
        free(q -> head -> prv);
        q -> head -> prv = NULL;
        
        cur_queue[q -> bot--] = NULL;
        cur_queue[q -> bot] = q;
    }
}

// Popping from the tail (back) of a queue
// Note that it is very similar to pop_front()
void pop_back(struct queue_of_treasure* q){
    if (q -> head == q -> tail){
        free(q -> head);
        cur_queue[q -> bot] = NULL;
        free(q);
    }
    else{
        q -> tail = q -> tail -> prv;
        free(q -> tail -> nxt);
        q -> tail -> nxt = NULL;
    }
}

// Pushing to the head (front) of the queue
// This function is complicated since we may result in merging queues.
void push_front(struct queue_of_treasure* q, int neg_d, long long v){
    // Extend the queue by moving the top upwards by one.
    --q -> top;
    q -> head = newQueueNode(q -> head, neg_d, v);
    q -> head -> nxt -> prv = q -> head;
    
    // If the top of queue reached root, print the treasure on top.
    if (q -> top == 0){
        if (q -> tail -> v >= 0) printf("value remaining is %lld\n", q -> tail -> v);
        else printf("value lost at %d\n", q -> tail -> neg_d);
        pop_back(q);
        ++q -> top;
    }
    // If the top of queue reached the bottom of another queue, merge the two queues.
    // The implementation of this part may have multiple different approaches.
    // The one provided below may not be the cleanest nor the simplest. It is just the one that I thought of when
    // writing the main solution.
    else if (cur_queue[q -> top] != NULL){
        // First, push the top of current queue to the upper queue
        push_front(cur_queue[q -> top], q -> tail -> neg_d, q -> tail -> v);
        // Then, pop that node out.
        pop_back(q);
        // Finally, some pointer operations to merge the queues.
        q -> tail -> nxt = cur_queue[q -> top] -> head;
        cur_queue[q -> top] -> head -> prv = q -> tail;
        q -> tail = cur_queue[q -> top] -> tail;
        q -> top = cur_queue[q -> top] -> top;
        cur_queue[q -> top] = q;
        // Note that cur_queue[] array is not properly modified since that wilong long require O(N) time.
        // The reason that it is not required is left as an exercise to the reader. :D
    }
    // The most easiest part! Hurray!!!
    else cur_queue[q -> top] = q;
}

// This part is the node of deques to solve the bonus version of operation 4
// The bonus part of operation 4 is based on a classic problem: sliding window maximum
// https://leetcode.com/problems/sliding-window-maximum
// The deque below is the sliding window maximum deque with additional metadata (t -> c) to determine the popping time.
// The rest is a normal implementation of the classic problem.
struct max_child_deque_node{
    int c;
    long long mxv;
    struct max_child_deque_node *prv, *nxt;
} *mxc_head[maxn], *mxc_tail[maxn];

struct max_child_deque_node* newMaxChildNode(int c, long long v, struct max_child_deque_node *prv){
    struct max_child_deque_node* t = (struct max_child_deque_node*) malloc(sizeof(struct max_child_deque_node));
    t -> c = c;
    t -> mxv = v;
    t -> prv = prv;
    t -> nxt = NULL;
    return t;
}

// Though named as updatemx(), it is simply popping along long the element less than v,
// and pushing the new element to the end of deque.
void updatemx(int i, int c, long long v){
    // The popping part. Although this may appears O(N), the total count of popping is bounded to the elements pushed
    // so the time required is O(N) throughout the entire lifespan of a deque and O(N)/N = O(1) per update.
    while (mxc_tail[i] != NULL && v > mxc_tail[i] -> mxv){
        struct max_child_deque_node* tmp = mxc_tail[i];
        mxc_tail[i] = mxc_tail[i] -> prv;
        free(tmp);
    }

    // Finally, push into the deque.
    if (mxc_tail[i] == NULL){
        mxc_head[i] = mxc_tail[i] = newMaxChildNode(c, v, NULL);
    }
    else{
        mxc_tail[i] -> nxt = newMaxChildNode(c, v, mxc_tail[i]);
        mxc_tail[i] = mxc_tail[i] -> nxt;
    }
}

// dfs() to find the furthest leaf.
long long dfs(int x){
    for (struct child_queue_node* c = head[x];c!=NULL;c=c -> nxt){
        // For each child, find the deepest leaf within the subtree and add that with the distance to the child.
        // Update the value into the deque.
        updatemx(x, c -> c, c -> d + dfs(c -> c));
    }

    // Return the furthest leaf in this subtree.
    if (mxc_head[x] == NULL) return 0; // The furthest leaf can be root and thus a distance of 0.
    else return mxc_head[x] -> mxv;
}

// A nice, simple, classic, elegant binary search on a stack.
int bin_search(long long x, int cur_layer){
    int l = 0, r = cur_layer;
    while (l != r){
        int m = (l + r) / 2;
        if (depth[m] >= x) r = m;
        else l = m + 1;
    }
    return l;
}

int main(){
    int n, m, q;

    scanf("%d%d%d", &n, &m, &q);

    // Initializtion
    for (int i=0;i<n;++i){
        cur_queue[i] = NULL;
        head[i] = tail[i] = NULL;
        mxc_head[i] = mxc_tail[i] = NULL;
    }

    // Input of the initial edges.
    while (m--){
        int u, v, w;

        scanf("%d%d%d", &u, &v, &w);
        if (head[u] == NULL){
            head[u] = tail[u] = newChild(v, w);
        }
        else{
            tail[u] -> nxt = newChild(v, w);
            tail[u] = tail[u] -> nxt;
        }
    }

    // Note that dfs() must be after the edges are constructed.
    dfs(0);

    // cur for current node, cur_layer for current layer
    int cur = 0, cur_layer = 0;
    while (q--){
        int op;

        scanf("%d", &op);
        if (op == 1){
            // Going downwards
            if (head[cur] != NULL){
                printf("%d\n", head[cur] -> c);
                // The depth array acts as an stack that stores depth of each layer in the tree.
                // It can help solve operation 3 and 5.
                depth[cur_layer+1] = depth[cur_layer] + head[cur] -> d;
                cur = head[cur] -> c;
                // A stack that allows random-access. We can know the node at each layer via this array-based stack
                stack[++cur_layer] = cur;
            }
            else printf("-1\n");
        }

        else if (op == 2){
            // Going upwards
            if (cur){ // If cur == 0, it is at root so no operations are required
                if (cur_queue[cur_layer] != NULL){
                    pop_front(cur_queue[cur_layer]);
                }

                cur = stack[--cur_layer]; // Moving cur upwards

                // If the subtree deleted is the where the current longest leaf belongs to,
                // we have to delete that node from the queue for operation 4
                if (mxc_head[cur] -> c == head[cur] -> c){
                    struct max_child_deque_node* tmp = mxc_head[cur];
                    mxc_head[cur] = mxc_head[cur] -> nxt;
                    free(tmp);

                    if (mxc_head[cur] == NULL) mxc_tail[cur] = NULL;
                    else mxc_head[cur] -> prv = NULL;
                }

                // Simply remove the first node (aka the child we just came from) in the child queue
                struct child_queue_node* tmp = head[cur];
                head[cur] = head[cur] -> nxt;
                free(tmp);
                printf("%d\n", cur);

                // Note that popping from an array-based stack is not necessary.
            }
            else printf("-1\n");
        }

        else if (op == 3){
            long long t;

            scanf("%lld", &t);
            // Perform binary search 
            printf("%d\n", stack[bin_search(depth[cur_layer] - t, cur_layer)]);
        }

        else if (op == 4){
            // Note that the bonus part only affects operation 4 and the implementation here is for the bonus version.
            // Basic: 
            // A dfs is done in the beginning so we already knew the furthest leaf within each subtree.
            // Then, this problem is simply solving suffix-maximum.
            // Bonus:
            // The bonus part of operation 4 is based on a classic problem: sliding window maximum
            // https://leetcode.com/problems/sliding-window-maximum
            // In the query part of a sliding window maximum problem, it is simply the head of the deque.
            if (head[cur] == NULL) printf("0\n");
            else {printf("%lld\n", mxc_head[cur] -> mxv);}
        }

        else if (op == 5){
            long long p;

            scanf("%lld", &p);
            // First, conduct a binary search similar to operation 3 to determine where the value dropped to below 0
            // or that it is never negative.
            int neg_id = bin_search(depth[cur_layer] - p, cur_layer);
            p -= depth[cur_layer];

            // Store the treasure into the treasure queue.
            // If no treasure in current layer, simply create one.
            if (cur_queue[cur_layer] == NULL){
                cur_queue[cur_layer] = newQueue(neg_id ? stack[neg_id - 1] : -1, p, cur_layer);
            }
            // Else, push it in the head of current queue. The rest of pushing treasures upwards is handled by the
            // function push_front().
            else{
                push_front(cur_queue[cur_layer], neg_id ? stack[neg_id - 1] : -1, p);
            }
        }

        else{
            // Bonus operation!
            int v, l;

            scanf("%d%d", &v, &l);
            // Here is relatively simple, we just push to the tail of child queue.
            if (head[cur] == NULL){
                head[cur] = tail[cur] = newChild(v, l);
            }
            else{
                tail[cur] -> nxt = newChild(v, l);
                tail[cur] = tail[cur] -> nxt;
            }
            // Update the sliding window maximum deque.
            // Note that since v is never visited, dfs(v) is required to properly calculate
            // the furthest leaf within the subtree of v
            updatemx(cur, v, l + dfs(v));
        }
    }
}
```
