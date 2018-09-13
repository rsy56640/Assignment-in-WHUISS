#include"printCatalan(recursion).h"
#include <Windows.h>  

int main()
{
	int n;
	cout << "please enter the size of the stack, which is allowed from 1-9." << endl;
	cin >> n;
	if (n <= 0 || n > 9) {
		cerr << "Error: n <= 0 or n > 9. I do not want to allow n > 9 because that is too large." << endl;
		system("pause");
		return 0;
	}
	if (n == 1) {
		cout << "1 " << endl;
		system("pause");
		return 0;
	}

	DWORD start_time = GetTickCount();
	print_out_stack(n,0,0,0);
	DWORD end_time = GetTickCount();
	cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;

	system("pause");
	return 0;
}