## $p1:BlackBox$  $Treap$ $O((n+m)logn)$

只要求支持添加操作与查值操作。Treap模板题。

- $2772kb, 70ms$

```c++
#include<cstdio>
#include<cstdlib>

using namespace std;

const int maxn = 3e4 + 9;

struct Node {
	int leftChild, rightChild, value, size, cnt, prio;
} t[maxn];
int root, size;

void update(int o) {
	t[o].size = t[t[o].leftChild].size + t[t[o].rightChild].size + t[o].cnt;
}
void leftRotate(int& o) {
	int s = t[o].rightChild;
	t[o].rightChild = t[s].leftChild;
	t[s].leftChild = o;
	t[s].size = t[o].size;
	update(o);
	o = s;
}
void rightRotate(int &o) {
	int s = t[o].leftChild;
	t[o].leftChild = t[s].rightChild;
	t[s].rightChild = o;
	t[s].size = t[o].size;
	update(o);
	o = s;
}
void insert(int &o, int v) {
	if (o == 0) {
		o = ++size;
		t[o].size = t[o].cnt = 1;
		t[o].value = v;
		t[o].prio = rand();
		return;
	}
	++t[o].size;
	if (t[o].value == v)
		++t[o].cnt;
	else if (v < t[o].value) {
		insert(t[o].leftChild, v);
		if (t[t[o].leftChild].prio < t[o].prio)
			rightRotate(o);
	}
	else {
		insert(t[o].rightChild, v);
		if (t[t[o].rightChild].prio < t[o].prio)
			leftRotate(o);
	}
}
int queryValue(int o, int rank) {
	if (o == 0)
		return 0;
	if (rank <= t[t[o].leftChild].size)
		return queryValue(t[o].leftChild, rank);
	else if (rank > t[t[o].leftChild].size + t[o].cnt)
		return queryValue(t[o].rightChild, rank - (t[t[o].leftChild].size + t[o].cnt));
	else
		return t[o].value;
}

int n, m, r, a[maxn], b[maxn];

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i)
		scanf("%d", a + i);
	for (int i = 1; i <= m; ++i)
		scanf("%d", b + i);

	for (int i = 1, j = 1; j <= m; ++j) {
		while (i <= b[j])
			insert(root, a[i++]);
		printf("%d\n", queryValue(root, ++r));
	}
}
```

## $p2:Tree$  点分治 $O(n(logn)^2)$

先将无根树转化成有根树，为避免退化，以树的重心为根。

满足条件的点对有两种情况：

1. 位于同一子树中。
2. 位于不同子树中。

预先处理好每个点到根的距离，先排序，再计算所有这样的点对，后减去位于同一子树的那一部分重复计算的即可。即$Sum_u=Calc_u-\Sigma Sum_v.$（其中v是u的儿子）

- $2944kb, 116ms$

```c++
#include <cstdio>
#include <algorithm>

using namespace std;

const int maxn = 1e4 + 9;
struct Edge {
	int des, val, nxt;
} e[maxn << 1];
int cnt, head[maxn];
void addEdge(int u, int v, int w) {
	e[++cnt].des = v;
	e[cnt].val = w;
	e[cnt].nxt = head[u];
	head[u] = cnt;
}

int n, k, sum, ans, root;
int siz[maxn], rootSize[maxn], record[maxn], dis[maxn];
bool vis[maxn];

void getRoot(int o, int fa) {
	siz[o] = 1;
	rootSize[o] = 0;
	for (int i = head[o]; i; i = e[i].nxt) {
		if (e[i].des == fa || vis[e[i].des])
			continue;
		getRoot(e[i].des, o);
		siz[o] += siz[e[i].des];
		rootSize[o] = max(rootSize[o], siz[e[i].des]);
	}
	rootSize[o] = max(rootSize[o], sum - siz[o]);
	if (rootSize[o] < rootSize[root])
		root = o;
}

void getDis(int o, int fa) {
	record[++record[0]] = dis[o];
	for (int i = head[o]; i; i = e[i].nxt) {
		if (e[i].des == fa || vis[e[i].des])
			continue;
		dis[e[i].des] = dis[o] + e[i].val;
		getDis(e[i].des, o);
	}
}

int calc(int o, int curDis) {
	dis[o] = curDis;
	record[0] = 0;
	getDis(o, 0);
	sort(record+1, record + record[0]+1);
	int total = 0;
	for (int l = 1, r = record[0]; l < r; )
		if (record[l] + record[r] <= k) {
			total += r - l;
			++l;
		}
		else
			--r;
	return total;
}

void work(int o) {
	ans += calc(o, 0);
	vis[o] = true;
	for (int i = head[o]; i; i = e[i].nxt) {
		if (vis[e[i].des])
			continue;
		ans -= calc(e[i].des, e[i].val);
		sum = siz[e[i].des];
		root = 0;
		getRoot(e[i].des, root);
		work(root);
	}
}

int main() {
	rootSize[0] = 0x7fffffff;
	while (scanf("%d%d", &n, &k), n && k) {
		ans = root = cnt = 0;
		for (int i = 1; i <= n; ++i) {
			vis[i] = false;
			head[i] = 0;
		}

		for (int i = 1, u, v, w; i < n; ++i) {
			scanf("%d%d%d", &u, &v, &w);
			addEdge(u, v, w);
			addEdge(v, u, w);
		}
		sum = n;
		getRoot(1, 0);
		work(root);
		printf("%d\n", ans);
	}
}
```

