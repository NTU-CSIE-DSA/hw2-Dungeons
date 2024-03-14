## Problem Description
**Dungeons of Sacred Abyss** is a brand new game developed by the software company Nextgen Techonology Universal Company of Software, Innovation and Engineering (NTUCSIE) in order to fill the video game market after **Dragon's Song: Awakening** have failed due to power inflation. This new game is designed to give player an unique immersive experience while discovering treasures through exploring the magnificent dungeons and the beauty of the sacred abyss.

In each game, the map consists of $N$ dungeons where $M$ of them are **Advanced** Dungeons and player is intially located in dungeon $0$. After the game starts, $n-1$ pathways between dungeons $u_i, v_i$ where $u_i < v_i$ with length $l_i$ will appear one by one. Interestingly, for each dungeon $j$ except dungeon $0$, exactly one pathway with $v_i = j$ exists. Another interesting property about these pathways is that for each dungeon $j$, at most one pathway connecting to a dungeon with greater number is activated at the same time. If a pathway appears when there are no activated pathway connecting dungeon $u_i$ to a dungeon with greater number, the pathway is immediately activated. Once a pathway from $u_i$ to $v_i$ is deactivated, the first pathway connecting dungeon $u_i$ to a dungeon with greater number that have not been activated before is activated immediately.

During your traversal, you might find some treasures with a large value. Since the dungeons are buried deep underground, the Advanced Dungeons Ascending (ADA) system is applied to transport the treasures. The system store treasures in the dungeons and each dungeon may store at most one treasure. When a treasure is being tried to store in a dungeon already with a treasure, the original one will be transported via an activated pathway to a dungeon with less number. If that dungeon already has a treasure too, this procedure will be applied recursively. When a treasure reaches dungeon $0$ though, it has been sucessfully escorted out of the system. You should report and remove it from the system. The advanced dungeons operate as a checkpoint for the treasures, treasures in them are monitered and sometimes new treasures are stored in them. However, this system operates at a cost. By transporting a treasure through a pathway, the value of a treasure is decreased by the length of such pathway. To help calculate these costs, you are sometimes required to search for the dungeon satsifying some criteria.

The game will give player $Q$ instructions on what to do and you, as the player, should follow them exactly. Otherwise, the head game developer Honey Toast Lin (HT Lin) will be mad and deduct your scores on DSA HW2 P4. There are six kinds of instructions that may be given to you:

1. Go to a dungeon with greater number than the current dungeon via an activated pathway and report which dungeon that is. If no such pathway exists, do not move and report $-1$.
2. Go to a dungeon with less number than the current dungeon via an activated pathway and report which dungeon that is. Then, deactivate the pathway that have just been gone through. If no such pathway exists, do not move and report $-1$.
3. Find and report the furthest dungeon that the ADA system can transport a treasure with value $t_i$ from the current dungeon to without the value dropping below $0$. It is still considered *transported* even if the treasure never leaves the current dungeon. Note that *no treasure is actually stored into the system* and we assume there is a consistent treasure flow pushing the treasure in this instruction.
4. Find and report the length of the shortest pathway from the current dungeon to a dungeon with greater number that have not been deactivated yet i.e. the pathways that are currently activated or still waiting to be activated. If no such pathway exists, report $-1$.
5. A treasure with value $p_i$ is discovered and stored into dungeon $d_i$. It is guaranteed that $d_i$ is either the current dungeon or an advanced dungeon located in the direct route from current dungeon to dungeon $0$.
6. Report the index and current value of the treasure stored at dungeon $d_i$. It is guaranteeed that $d_i$ is an advanced dungeon located in the direct route from current dungeon to dungeon $0$.


### Input
The first line consists of three space-seperated integers, $N, M, Q$.

The second line consists of $M$ space-seperated unique integers, $a_1, a_2, \dots, a_M$

The next $N-1$ lines each consists of three space-seperated integers $u_i, v_i, l_i$.

The final $Q$ lines each consists of space-seperated integers depending on the instruction given, they may be:
+ $1$, indicating instruction 1 is given.
+ $2$, indicating instruction 2 is given.
+ $3$ $t_i$, indicating instruction 3 is given.
+ $4$, indicating instruction 4 is given.
+ $5$ $d_i$ $p_i$, indicating instruction 5 is given.
+ $6$ $d_i$, indicating instruction 6 is given.

### Output
For instructions 1, 2, 3 and 4, print a line with an integer representing the report to such instruction.

For instruction 5, if a treasure is transported out of the system, print a line with two integers representing the index and the current value of such treasure. Do not print anything if it does not happen. 

For instruction 6, print a line with two integers representing report to such instruction or a single integer $-1$ if no treasure is at $d_i$.

### Constraints
+ $2 \leq N \leq 2 \times 10^5$
+ $0 \leq M \leq 20$
+ $1 \leq Q \leq 2 \times 10^5$
+ $0 < a_i < N$
+ $0 \leq u_i < v_i < N$, all the $v_i$ should form a permutaion of $1$ to $N-1$.
+ $0 < l_i \leq 10^9$
+ $0 \leq t_i \leq 10^{18}$
+ $0 \leq p_i \leq 10^{18}$
+ $0 < d_i < N$
+ $d_i$ is either the current node or an advanced dungeon located in the direct route from current dungeon to dungeon $0$ in instruction 5.
+ $d_i$ is an advanced dungeon located in the direct route from current dungeon to dungeon $0$ in instruction 6.

## Subtasks
### Subtask 1 (5 pts)
+ Only instructions 1 and 2 are given.

### Subtask 2 (15 pts)
+ Only instructions 1, 2 and 3 are given.

### Subtask 3 (10 pts)
+ Only instructions 1, 2 and 4 are given.

### Subtask 4 (20 pts)
+ Only instructions 1, 2 and 5 are given.
+ $M = 0$

### Subtask 5 (20 pts)
+ Only instructions 1, 2, 5 and 6 are given.

### Subtask 6 (30 pts)
+ No other constraints.

## Sample Testcases

### Sample Input 1
```
4 0 8
0 1 2
0 2 5
1 3 2
1
3 3
1
1
3 4
3 0
2
2
```

### Sample Output 1
```
1
0
3
-1
0
3
1
0
```

### Sample Input 2
```
4 2 11
3 1
0 2 3
0 1 7
0 3 5
4
1
2
4
1
2
4
1
2
4
1
```


### Sample Output 2
```
3
2
0
5
1
0
5
3
0
-1
-1
```

### Sample Input 3
```
6 0 11

0 1 5
1 2 4
2 3 3
3 4 2
4 5 1
1
5 1 10
1
5 2 20
1
1
5 4 30
5 4 25
5 4 50
2
5 3 15
```

### Sample Output 3
```
1
2
3
4
1 5
3
3 11
```

### Sample Input 4
```
5 1 12
1
0 1 11
0 2 4
1 3 5
1 4 14
1
5 1 12
1
5 3 20
5 1 14
2
1
5 4 25
5 4 30
6 1
2
2
```

### Sample Output 4
```
1
3
1 1
1
4
4 3
7 1
1
0
```

## Hints
+ Don't be afraid with the large number of operations. Some of them are relatively simple ~~and my original problem idea consists of 9 operations. Be grateful :)~~
+ The pathways of the map seems to form something familiar?
+ To handle the value depreciation of the treasures, maybe the values stored to deal with instruction 3 will help?