#include <cstdio>

using namespace std;

const int maxn = 8009;
int n, m, fa[maxn];

int find(int o) {
	return fa[o] == o ? o : fa[o] = find(fa[o]);
}

int main()
{
	int totKase;
	scanf("%d", &totKase);
	for (int kase = 1; kase <= totKase; ++kase) {
		printf("Scenario #%d:\n", kase);
		scanf("%d%d", &n, &m);
		// initialization
		for (int i = 1; i <= n + n; ++i)
			fa[i] = i;
		bool flag = true;
		// union (x,y') (x',y)
		for (int i = 0, x, y, a, b, c, d; i < m; ++i) {
			scanf("%d%d", &x, &y);
			a = find(x);
			b = find(y);
			c = find(x + n);
			d = find(y + n);
			fa[a] = d;
			fa[find(b)] = find(c);
		}
		// check if there is something wrong (IMPORTANT)
		for (int i = 1; i <= n; ++i)
			if (find(i) == find(i + n)) {
				flag = false;
				break;
			}
		if (flag) puts("No suspicious bugs found!");
		else puts("Suspicious bugs found!");
		puts("");
	}
}
