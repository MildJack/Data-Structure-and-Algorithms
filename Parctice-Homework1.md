## $p1$ 线段树 $O(nlogn)$

区间修改区间求和，线段树模板裸题，唯一需要注意的是$long$ $long$的输入和输出，容易$PE$。

线段树的细节还是挺多的，老年选手要多多注意。

- $19024kb, 1231ms$

```c++
#include <cstdio>
#include <iostream>

using namespace std;

typedef long long LL;
const int maxn = 1e5 + 9;
int n, m;

struct Node {
	int l, r;
	LL s, tg;
} t[maxn << 2];

void pushup(int o) {
	t[o].s = t[o << 1].s + t[o << 1 | 1].s;
}

void pushdown(int o) {
	if (t[o].l == t[o].r) return;
	if (t[o].tg) {
		t[o << 1].tg += t[o].tg;
		t[o << 1 | 1].tg += t[o].tg;
		t[o << 1].s += t[o].tg * (t[o << 1].r - t[o << 1].l + 1);
		t[o << 1 | 1].s += t[o].tg * (t[o << 1 | 1].r - t[o << 1 | 1].l + 1);
		t[o].tg = 0;
	}
}

void build(int o, int l, int r) {
	t[o].l = l;
	t[o].r = r;
	if (l == r) {
		cin >> t[o].s;
		return;
	}
	int m = l + r >> 1;
	build(o << 1, l, m);
	build(o << 1 | 1, m + 1, r);
	pushup(o);
}

void add(int o, int l, int r, int v) {
	pushdown(o);
	int oL = t[o].l, oR = t[o].r, s = oR - oL + 1;
	if (oL == l && oR == r) {
		t[o].tg += v;
		t[o].s += (LL)s * v;
		return;
	}
	int m = oL + oR >> 1;
	if (r <= m) add(o << 1, l, r, v);
	else if (l > m) add(o << 1 | 1, l, r, v);
	else {
		add(o << 1, l, m, v);
		add(o << 1 | 1, m + 1, r, v);
	}
	pushup(o);
}

LL query(int o, int l, int r) {
	pushdown(o);
	int oL = t[o].l, oR = t[o].r;
	if (oL == l && oR == r)
		return t[o].s;
	int m = oL + oR >> 1;
	if (r <= m) return query(o << 1, l, r);
	else if (l > m) return query(o << 1 | 1, l, r);
	else return query(o << 1, l, m) + query(o << 1 | 1, m + 1, r);
}

int main() {
	scanf("%d%d", &n, &m);
	build(1, 1, n);
	while (m--) {
		int x, y, z;
		char c[9];
		scanf("%s", c);
		if (c[0] == 'C') {
			scanf("%d%d%d", &x, &y, &z);
			add(1, x, y, z);
		}
		else {
			scanf("%d%d", &x, &y);
			cout << query(1, x, y) << endl;
		}
	}
}
```

## $p2$ 主席树 $O((n+m)lognlogn)$

求区间$K$大，模板裸题。很多数据结构都可以解决这个问题，我这里使用了主席树。

- $32812kb, 950ms$

```c++
#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

const int maxn = 1e5 + 9, maxs = 8e7 + 9;
int n, m, sz;
int rt[maxn], a[maxn], ls[maxs], rs[maxs], s[maxs];

int tot, hashh[maxn], num[maxn];
void discret() {
	for (int i = 1; i <= n; ++i)
		num[i] = a[i];
	sort(num + 1, num + n + 1);
	hashh[++tot] = num[1];
	for (int i = 2; i <= n; ++i)
		if (num[i] != num[i - 1])
			hashh[++tot] = num[i];
}
int find(int x) {
	int l = 1, r = tot, mid;
	while (l < r)
	{
		int mid = (l + r) >> 1;
		if (hashh[mid]<x)l = mid + 1;
		else r = mid;
	}
	return r;
}

void insert(int l, int r, int x, int &y, int v) {
	y = ++sz;
	s[y] = s[x] + 1;
	if (l == r)return;
	ls[y] = ls[x];
	rs[y] = rs[x];
	int m = l + r >> 1;
	if (v <= m) insert(l, m, ls[x], ls[y], v);
	else insert(m + 1, r, rs[x], rs[y], v);
}

int rankk(int k, int l, int r, int x) {
	if (r == x) return s[k];
	int m = l + r >> 1;
	if (x <= m) return rankk(ls[k], l, m, x);
	else return s[ls[k]] + rankk(rs[k], m + 1, r, x);
}

int query(int L, int R, int v) {
	int x = rt[L - 1], y = rt[R], l = 1, r = tot;
	while (l <= r) {
		int m = l + r >> 1;
		int t = rankk(y, 1, tot, m) - rankk(x, 1, tot, m);
		if (v > t) l = m + 1;
		else r = m - 1;
	}
	return l;
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) 
		scanf("%d", a + i);
	discret();
	for (int i = 1; i <= n; ++i)
		insert(1, tot, rt[i - 1], rt[i], find(a[i]));
	while (m--) {
		int l, r, x;
		scanf("%d%d%d", &l, &r, &x);
		printf("%d\n", hashh[query(l, r, x)]);
	}
}
```

## $p3$

### $A$ 暴力 $O(n^2)$ 

本题数据范围暴力完全可过。

- $432kb, 211ms$

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int maxn = 8e3 + 9;
int n, a[maxn];

int main()
{
	scanf("%d", &n);
	for (int i = 1; i < n; ++i) {
		scanf("%d", a + i);
		for (int j = 0; j < i; ++j)
			if (a[j] >= a[i])
				++a[j];
	}
	for (int i = 0; i < n; ++i)
		printf("%d\n", a[i] + 1);
}
```

###$B$ 树状数组+二分 $O(nlog^2n)$

倒序处理，一个位置的值等于它前面比它小的数的个数$k$和它后面比它小的数的个数$Query$加1。

用树状数组记录填过的数，则可二分当前位置的值，并用读入的$k$加上树状数组的$Query$之和来判断。

- $432kb, 11ms$

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int maxn = 8e3 + 9;
int n, a[maxn], t[maxn];

int query(int pos) {
	int sum = 0;
	while (pos) {
		sum += t[pos];
		pos -= pos & (-pos);
	}
	return sum;
}

void insert(int pos) {
	while (pos <= n) {
		++t[pos];
		pos += pos & (-pos);
	}
}

int binarySearch(int k) {
	int l = 1, r = n;
	while (l < r) {
		int m = l + r >> 1;
		int num = query(m);
		if (m <= num + k) l = m + 1;
		else r = m;
	}
	return r;
}

int main()
{
	scanf("%d", &n);
	for (int i = 2; i <= n; ++i)
		scanf("%d", a + i);
	for (int i = n; i; --i) {
		int k = binarySearch(a[i]);
		insert(k);
		a[i] = k;
	}
	for (int i = 1; i <= n; ++i)
		printf("%d\n", a[i]);
}
```

## 总结

老年选手真的已经不会这些数据结构了，感觉这门课很吃力，得把这些板子好好背了。
