#include<cstdio>
#include<stack>

using namespace std;

char s[999];
// stack of numbers
stack <int> num;
// stack of operations
stack <char> opt;

// get an interger
int gint(int& p) {
	int t = 0;
	while (s[p] >= '0' && s[p] <= '9') {
		t = 10 * t + s[p++] - '0';
	}
	return t;
}

// calculate the priority of the operation
int prio(char c) {
	if (c == '*' || c == '/') return 1;
	if (c == '+' || c == '-') return 0;
	if (c == '(') return -1;
	// ~ is used to pop all elements in stack in the end
	if (c == '~') return -2;
	// ! is used to make sure the stack is not empty, avoiding that check
	if (c == '!') return -3;
}

// calculate
void calc() {
	int t = opt.top(); opt.pop();
	int y = num.top(); num.pop();
	int x = num.top(); num.pop();
	int z;
	switch (t) {
		case '+': z = x + y; break;
		case '-': z = x - y; break;
		case '*': z = x * y; break;
		case '/': z = x / y; break;
	}
	num.push(z);
}

int main()
{
	// number of test cases
	int kase;
	scanf("%d", &kase);
	opt.push('!');
	while (kase--) {
		scanf("%s", s);
		int len = strlen(s), p = 0;
		s[len] = '~';
		while (p <= len) {
			if (s[p] >= '0' && s[p] <= '9')
				num.push(gint(p));
			else {
				if (s[p] == ')') {
					while (opt.top() != '(')
						calc();
					// pop '('
					opt.pop();
				}
				else if (s[p] != '(') {
					while (prio(opt.top()) >= prio(s[p]))
						calc();
				}
				if (s[p] != ')')
					opt.push(s[p]);
				++p;
			}
		}
		// print the result
		printf("%d\n", num.top());
		// pop result
		num.pop();
		// pop '~'
		opt.pop();
	}
}
