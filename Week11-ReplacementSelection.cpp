#include <cstdio>
#include <algorithm>

using namespace std;

const int maxn = 1009;
int m, siz, str[maxn], A[maxn];

class MinHeap {
public:
	int *heapArray;
	int currentSiz, maxSize;
	void buildHeap();
	void siftDown(int pos);
	MinHeap(int *array, const int n);
};

void MinHeap::siftDown(int pos) {
	int i = pos;
	// j: the left child of i, j + 1: the right child of i
	// tmp: save the value of heapArray[i], which later will be put in the last position
	int j = i << 1 | 1, tmp = heapArray[i];
	while (j < currentSiz) {
		if (j < currentSiz - 1 && heapArray[j] > heapArray[j + 1])
			++j;
		if (tmp > heapArray[j]) {
			heapArray[i] = heapArray[j];                                             
			i = j;
			j = i << 1 | 1;
		}
		else break;
	}
	heapArray[i] = tmp;
}

void MinHeap::buildHeap() {
	for (int i = currentSiz / 2 - 1; i >= 0; --i)
		siftDown(i);
}

MinHeap::MinHeap(int *array, const int n) {
	currentSiz = maxSize = n;
	heapArray = new int[maxSize];
	for (int i = 0; i < n; ++i)
		heapArray[i] = array[i];
	buildHeap();
}

void replacementSelection(int *A) {
	MinHeap H(A, siz);
	int last = siz - 1;

	// difficult to understand, why siz-1?
	int len = m < last ? m : siz;

	for (int i = 0; i < len; ++i) {
		printf("%d ", H.heapArray[0]);
		// should be in this sequence
		if (str[i] >= H.heapArray[0])
			H.heapArray[0] = str[i];
		// should be in another sequence
		else {
			H.heapArray[0] = H.heapArray[last];
			H.heapArray[last] = str[i];
			--H.currentSiz;
			--last;
		}
		if (last != 0)
			H.siftDown(0);
	}
}

int main() {
	scanf("%d%d", &m, &siz);
	for (int i = 0; i < m; ++i)
		scanf("%d", str + i);
	for (int i = 0; i < siz; ++i)
		scanf("%d", A + i);
	replacementSelection(A);
}
