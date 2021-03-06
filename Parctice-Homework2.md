## $p1$ $DFS$+树状数组 $O((n+m)logn)$

第一步先建树，求出$dfs$序，由于一棵子树对应的$dfs$序是连续的一段，因此可以归纳为一个区间求和问题。$vert$数组记录节点对应的$dfs$序，$rg$数组记录其子树最大的$vert$值。

第二步是简单的单点修改区间求和问题，用树状数组。

- $18568kb, 192ms$

```c++
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stack>
#include <queue>

using namespace std;

const int maxn = 1e5 + 9;
int n, m;

struct Edge {
	int v;
	Edge* nx;
} *e[maxn << 1];
Edge* hd[maxn];
int cnt;
void addEdge(int u, int v) {
	e[++cnt] = new Edge();
	e[cnt]->v = v;
	e[cnt]->nx = hd[u];
	hd[u] = e[cnt];
}

int vert[maxn], rg[maxn], id;
void dfs(int o, int fa) {
	vert[o] = ++id;
	for (Edge* i = hd[o]; i != NULL; i = i->nx)
		if (i->v != fa)
			dfs(i->v, o);
	rg[o] = id;
}

int t[maxn << 1];
void ins(int x, int y) {
	while (x <= n) {
		t[x] += y;
		x += x & (-x);
	}
}

int que(int x) {
	int s = 0;
	while (x) {
		s += t[x];
		x -= x & (-x);
	}
	return s;
}
int c[maxn];

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) {
		c[i] = 1;
		ins(i, 1);
	}

	for (int i = 1, u, v; i < n; ++i) {
		scanf("%d%d", &u, &v);
		addEdge(u, v);
		addEdge(v, u);
	}
	dfs(1, 1);

	scanf("%d", &m);
	char s[2];
	int x;
	while (m--) {
		scanf("%s %d", s, &x);
		if (s[0] == 'C') {
			if (c[x] == 0) {
				c[x] = 1;
				ins(vert[x], 1);
			}
			else{
				c[x] = 0;
				ins(vert[x], -1);
			}
		}
		else {
			printf("%d\n", que(rg[x]) - que(vert[x] - 1));
		}
	}
}
```
