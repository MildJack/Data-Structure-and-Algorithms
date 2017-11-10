#include <cstdio>

using namespace std;

const int maxn = 2e5;
int n, in[maxn], post[maxn];

// p: post order, i: in order
void search(int pl, int pr, int il, int ir) {
	if (pl > pr) return;

	// m: current node
	int m;
	for (int i = il; i <= ir; ++i)
		if (in[i] == post[pr]) {
			m = i;
			break;
		}
	printf("%d ", in[m]);

	if (pl == pr) return;

	search(pl, pl + m - il - 1, il, m - 1);
	search(pr - ir + m, pr - 1, m + 1, ir);
}

int main()
{
	while (~scanf("%d", &in[n])) ++n;
	n /= 2;
	for (int i = 0; i < n; ++i)
		post[i] = in[n + i];
	search(0, n - 1, 0, n - 1);
}