#include <cstdio>
#include <algorithm>

using namespace std;

const int maxn = 2009;
int n, m, element[maxn], loser[maxn];
// siz: the size of the second deepest layer
int siz;
// extraGame: some players play extra games to get the total number of players in the form of 2^t
int extraGame;

inline int gameWinner(int a, int b) {
	return element[a] < element[b] ? a : b;
}
inline int gameLoser(int a, int b) {
	return element[a] > element[b] ? a : b;
}

void playGame(int father, int leftChild, int rightChild) {
	int tmp1, tmp2;
	loser[father] = gameLoser(leftChild, rightChild);
	tmp1 = gameWinner(leftChild, rightChild);
	// bubble up along the right branches
	while (father > 1 && father % 2 == 1) {
		tmp2 = gameWinner(tmp1, loser[father >> 1]);
		loser[father >> 1] = gameLoser(tmp1, loser[father >> 1]);
		tmp1 = tmp2;
		father >>= 1;
	}
	// below is the current winner
	loser[father >> 1] = tmp1;
}

void replayGame(int i) {
	int father, tmp;
	if (i <= (extraGame << 1))
		father = i + (siz << 1) - 1 >> 1;
	else
		father = i - extraGame + siz - 1 >> 1;
	// loser[0] records current winner
	tmp = gameWinner(i, loser[father]);
	loser[father] = gameLoser(i, loser[father]);
	while (father >> 1) {
		int tmp;
		tmp = gameWinner(loser[father >> 1], loser[0]);
		loser[father >> 1] = gameLoser(loser[father >> 1], loser[0]);
		loser[0] = tmp;
		father >>= 1;
	}
}

void buildTree() {
	for (siz = 1; (siz << 1) <= n - 1; siz <<= 1);
	extraGame = n - siz;
	// below are extra games
	for (int i = 1; i <= extraGame; ++i)
		playGame(siz - 1 + i, (i << 1) - 1, i << 1);
	// if the number is odd, one player needs to play with a winner at first
	if (n % 2) 
		playGame(n >> 1, loser[n - 1 >> 1], extraGame << 1 | 1);
	// below are games remain
	for (int i = (extraGame << 1) + (n % 2) + 1; i <= n; i += 2)
		playGame(i - extraGame + siz >> 1, i, i + 1);
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i)
		scanf("%d", element + i);
	buildTree();

	for (int i = 0; i < n; ++i)
		printf("%d ", element[loser[i]]);
	puts("");
	while (m--) {
		int x, y;
		scanf("%d%d", &x, &y);
		element[x + 1] = y;
		replayGame(x + 1);
		for (int i = 0; i < n; ++i)
			printf("%d ", element[loser[i]]);
		puts("");
	}
}
