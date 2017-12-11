/*	Project: Calculator
 *	Author: Li Xue
 *	Student ID:
 */

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stack>

using namespace std;

class Calculator {
private:
#define MAX_INPUT 10000
#define EPS 1e-8
	const double pi = acos(-1);
	const double zero = 0.;
	const double inf = 1 / zero;
	const double nan = inf - inf;

	char inputString[MAX_INPUT];
	int stringLength;
	bool invalid;
	bool isEquation;
	
	stack <double> numStack;
	stack <int> optStack;

public:
	bool input();
	bool isDigit(int pos);
	bool isInteger(double num);
	double getNumber(int& pos);
	double frac(double num);
	int getOperator(int& pos);
	int priority(int opt);
	void process();
	double calcProcess(double x);
	void solveProcess();
	void calculate(double x);
	void clear();
} calc;

void Calculator::clear() {
	inputString[stringLength] = 0;
	while (stringLength)	inputString[--stringLength] = 0;
	invalid = isEquation = false;
}

bool Calculator::isDigit(int pos) {
	return (inputString[pos] >= '0' && inputString[pos] <= '9') 
		|| inputString[pos] == 'x';
}

bool Calculator::isInteger(double num) {
	return abs(num - (int)num) < EPS || 1 - abs(num - (int)num) < EPS;
}

double Calculator::getNumber(int& pos) {
	bool isFloat = false;
	int inte = 0, deci = 0, powe = 1;
	char cPos = inputString[pos];

	if (cPos == 'x') {
		if (isDigit(++pos))
			invalid = true;
		return nan;
	}

	while (isDigit(pos) || cPos == '.') {
		if (cPos == 'x')	
			invalid = true;

		if (cPos == '.') {
			if (!isFloat) {
				isFloat = true;
				cPos = inputString[++pos];
				continue;
			} else {
				invalid = true;
				return 0;
			}
		}
		if (!isFloat) {
			inte = 10 * inte + cPos - '0';
		} else {
			deci = max(10 * deci + cPos - '0', deci);
			powe = max(10 * powe, powe);
		}
		cPos = inputString[++pos];
	}
	return inte + (double)deci / powe;
}

int Calculator::getOperator(int& pos) {
	int prePos = pos;
	char opt[9];
	char cPos = inputString[pos];

	if (!isDigit(pos) && cPos != '.' && cPos) {
		if (cPos >= 'a' && cPos <= 'z')
			pos += 3;
		else
			++pos;
	}

	memset(opt, 0, sizeof opt);
	strncpy(opt, inputString + prePos, pos - prePos);

	if (strcmp(opt, "@") == 0)		return 10;
	if (strcmp(opt, "(") == 0)		return 20;
	if (strcmp(opt, ")") == 0)		return 21;

	if (strcmp(opt, "+") == 0)		return 30;
	if (strcmp(opt, "-") == 0)		return 31;
	if (strcmp(opt, "*") == 0)		return 40;
	if (strcmp(opt, "/") == 0)		return 41;
	if (strcmp(opt, "%") == 0)		return 42;
	if (strcmp(opt, "^") == 0)		return 50;

	if (strcmp(opt, "!") == 0)		return 60;

	if (strcmp(opt, "sin") == 0)	return 61;
	if (strcmp(opt, "cos") == 0)	return 62;
	if (strcmp(opt, "tan") == 0)	return 63;
	if (strcmp(opt, "cot") == 0)	return 64;
	if (strcmp(opt, "csc") == 0)	return 65;
	if (strcmp(opt, "sec") == 0)	return 66;
	if (strcmp(opt, "log") == 0)	return 67;
	
	invalid = true;
	return 0;
}

int Calculator::priority(int opt) {
	return opt / 10;
}

double Calculator::frac(double num) {
	num += .5;
	double res = 1;
	for (int i = 1; i < num; ++i) {
		res *= i;
	}
	return res;
}

void Calculator::calculate(double x) {
	int opt = optStack.top();
	optStack.pop();

	if (numStack.empty()) {
		invalid = true;
		return;
	}

	if (priority(opt) == 6) {
		double num = numStack.top();
		if (num != num)	num = x;
		numStack.pop();
		switch (opt % 10)
		{
		case 1:
			num = sin(pi * num / 180);		break;
		case 2:
			num = cos(pi * num / 180);		break;
		case 3:
			num = tan(pi * num / 180);		break;
		case 4:
			num = 1 / tan(pi * num / 180);	break;
		case 5:
			num = 1 / cos(pi * num / 180);	break;
		case 6:
			num = 1 / sin(pi * num / 180);	break;
		case 7:
			if (num > EPS)	num = log(num);
			else invalid = true;			break;
		}
		numStack.push(num);

	} else {
		double num1 = numStack.top();
		if (num1 != num1)	num1 = x;
		numStack.pop();

		if (numStack.empty()) {
			invalid = true;
			return;
		}

		double num0 = numStack.top();
		if (num0 != num0)	num0 = x;
		numStack.pop();
		switch (opt)
		{
		case 30:
			num0 = num0 + num1;		break;
		case 31:
			num0 = num0 - num1;		break;
		case 40:
			num0 = num0 * num1;		break;
		case 41:
			if (abs(num1) > EPS) num0 = num0 / num1;
			else invalid = true;	break;
		case 42:
			if (isInteger(num0) && isInteger(num1) && abs(num1) > EPS)	
				num0 = (int)(num0 + .5) % (int)(num1 + .5);
			else invalid = true;	break;
		case 50:
			num0 = pow(num0, num1);	break;
		}
		numStack.push(num0);
	}
}

double Calculator::calcProcess(double x) {

#define CHECK {if (invalid) {if (!isEquation) puts("WRONG"); return nan;}}

	optStack.push(0);

	for (int pos = 0; pos <= stringLength; ) {
		if (isDigit(pos)) {
			CHECK;
			numStack.push(getNumber(pos));
		} else {
			int opt = getOperator(pos);
			CHECK;
			if (opt == 21) {
				while (optStack.top() != 20) {
					if (optStack.top() == 0) invalid = true;
					calculate(x);
					CHECK;
				}
				optStack.pop();
			} 
			else if (opt == 60) {
				double num = numStack.top();
				numStack.pop();
				if (!isInteger(num) || num < -EPS)	invalid = true;
				CHECK;
				numStack.push(frac(num));
			}
			else {
				if (opt != 20)
					while (priority(optStack.top()) >= priority(opt)) {
						calculate(x);
						CHECK;
					}
				optStack.push(opt);
			}
		}
	}
	double num = numStack.top();
	numStack.pop();
	optStack.pop();

	if (! numStack.empty()) invalid = true;
	if (optStack.top() != 0) invalid = true;
	CHECK;
	while (!numStack.empty())	numStack.pop();
	while (!optStack.empty())	optStack.pop();

	if (!isEquation)	printf("%g\n", num);
	return num;
}

void Calculator::solveProcess() {
#define RANGE 1e2
#define STEP 1e-2
#define PRECISION 1e-6
#define CLEAN {invalid = false;while(!numStack.empty())numStack.pop();while(!optStack.empty())optStack.pop();}

	int rootCount = 0;
	double root[MAX_INPUT];
	memset(root, 0, sizeof root);

	for (double left = -RANGE; left < RANGE; left += STEP) {

		double right = left + STEP;
		CLEAN;
		double fLeft = calcProcess(left);
		CLEAN;
		double fRight = calcProcess(right);

		if (fLeft != fLeft || fRight != fRight)
			continue;

		if (abs(fLeft) < EPS) {
			root[rootCount++] = left;
			continue;
		}

		if (abs(fRight) < EPS) {
			root[rootCount++] = right;
			continue;
		}

		if (fLeft * fRight < -EPS) {
			double lef = left, rig = right;
			while (rig - lef > PRECISION) {

				double mid = (lef + rig) / 2;
				CLEAN;
				double fMid = calcProcess(mid);
				
				if (fMid != fMid)
					break;

				if (fLeft * fMid < -EPS) {
					rig = mid;
					fRight = fMid;
				} else {
					lef = mid;
					fLeft = fMid;
				}
			}

			root[rootCount++] = (lef + rig) / 2;
		}
	}

	if (!rootCount) {
		puts("NO SOLUTION");
		return;
	}

	for (int i = 1; i < rootCount; ++i)
		if (abs(root[i]) < EPS)
			root[i] = 0;

	sort(root, root + rootCount);
	printf("%g", root[0]);

	for (int i = 1; i < rootCount; ++i)
		if (abs(root[i] - root[i - 1]) > PRECISION)
			printf(",%g", root[i]);
	puts("");
}

void Calculator::process() {
	if (isEquation && stringLength < 3) {
		puts("WRONG");
		return;
	}

	if (!isEquation)	calcProcess(0);
	else	solveProcess();
}

bool Calculator::input() {
	clear();
	scanf("%s", inputString);
	stringLength = strlen(inputString);

	char tmp[MAX_INPUT];
	memset(tmp, 0, sizeof tmp);

	int pt = 0;
	if (inputString[0] == '-')
		tmp[pt++] = '0';
	for (int ps = 0; ps < stringLength; ++ps) {
		tmp[pt++] = inputString[ps];
		if (inputString[ps] == '(' && inputString[ps + 1] == '-')
			tmp[pt++] = '0';
	}
	stringLength = pt;
	for (int i = 0; i < pt; ++i) {
		inputString[i] = tmp[i];
		if (tmp[i] == 'x')
			isEquation = true;
	}
	if (isEquation && stringLength > 2) {
		stringLength -= 2;
		inputString[stringLength + 1] = 0;
	}
	inputString[stringLength] = '@';

	return stringLength > 0;
}

int main() {
	for (printf("> "); calc.input(); printf("> "))
		calc.process();
	return 0;
}
