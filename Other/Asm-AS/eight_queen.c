#include<iostream>
using namespace std;

int _count = 0;
bool judge(int* p, int pos, int m) {
	for (int i = 0; i <= pos; ++i)
		if (p[i] == m || pos + 1 - i == abs(m - p[i]))return false;
	return true;
}

void EightQueens(int* p, int pos) {
	if (pos == 7) {
		_count++;
		printf("%d: %d %d %d %d %d %d %d %d\n", _count,
			p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		return;
	}
	for (int i = 0; i < 8; ++i) {
		if (judge(p, pos, i)) {
			int _pos = pos;
			p[++_pos] = i;
			EightQueens(p, _pos);
		}
	}
}

int main()
{
	int queen[8];
	EightQueens(queen, -1);
    return 0;
}
