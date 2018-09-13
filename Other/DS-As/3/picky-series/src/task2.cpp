#include <iostream> 
#include <iterator>
using namespace std;
ostream_iterator<int> _oit(cout, " ");
//is supposed to place the num
void dfs(int* p, int num, int n) {
	//print if all num are placed
	if (num > n) {
		copy(p, p + 2 * n, _oit);
		cout << endl;
		return;
	}
	//place the num
	int left = 0, right = left + num + 1;
	for (; right < 2 * n; ++left, ++right) {
		//judge if the position can be placed with num
		if (p[left] == 0 && p[right] == 0) {
			p[left] = p[right] = num;
			//depth first search
			dfs(p, num + 1, n);
			//exit this branchs
			p[left] = p[right] = 0;
		}
	}
}
int main() {
	int n;
	cin >> n;
	if (n % 4 == 1 || n % 4 == 2) {
		cout << "There is no such sequence." << endl;
		system("pause");
		return 0;
	}
	int *p = new int[2 * n];
	memset(p, 0, sizeof(int) * 2 * n);
	dfs(p, 1, n);
	system("pause");
	return 0;
}
