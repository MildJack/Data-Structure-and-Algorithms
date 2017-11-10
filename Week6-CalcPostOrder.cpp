// notes referred to Week5-CalcPreOrder.cpp

#include <cstdio>

using namespace std;

const int maxn = 99;
int n;
char in[maxn], pre[maxn];

void search(int pl, int pr, int il, int ir) {
	if (pl > pr) return;

	int m;
	for (int i = il; i <= ir; ++i)
		if (in[i] == pre[pl]) {
			m = i;
			break;
		}

	if (pl == pr) {
		printf("%c", in[m]);
		return;
	}

	search(pl + 1, pl + m - il, il, m - 1);
	search(pr - ir + m + 1, pr, m + 1, ir);
	printf("%c", in[m]);
}

int main()
{
	while (~scanf("%s %s", pre, in)) {
		n = strlen(pre);

		search(0, n - 1, 0, n - 1);
		puts("");
	}
}
