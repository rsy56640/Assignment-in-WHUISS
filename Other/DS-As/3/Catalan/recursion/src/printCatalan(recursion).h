#pragma once
#include<iostream>
using namespace std;

const int num = 10;
bool counter[2 * num];

class myStk {
private:
	int stk[num], cur;
public:
	myStk():cur(-1) {}
	void push(const int n) { stk[++cur] = n; }
	void pop() { cout << stk[cur--] << " "; }
};

void print_out_stack(int n, int in, int out, int pos) {
	if (in < n) {
		counter[pos] = true;
		print_out_stack(n, in + 1, out, pos + 1);
	}
	if (in > out) {
		counter[pos] = false;
		print_out_stack(n, in, out + 1, pos + 1);
	}
	if (out == n) {
		myStk stk;
		int m = 1;
		for (int i = 0; i < 2 * n; ++i) {
			if (counter[i])stk.push(m++);
			else stk.pop();
		}
		cout << endl;
	}
}