#include"Catalan.h"
#include<Windows.h>

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


	DWORD start_time = GetTickCount();			//计时

	if (n == 1) {
		cout << "1" << endl;
		return 0;
	}

	MyString cat[num][1430];			//why not 4862 cannot be replaced by Catalan[num]???

	//MyString c0[1][1], c1[1][1], c2[1][2], c3[1][5], c4[1][14], c5[1][42],
	//	c6[1][132], c7[1][429], c8[1][1430], c9[1][4862], **cat[10];
	//map<MyString**, MyString**>mymap;
	//mymap(cat[1]) = c0;

	cat[0][0] = "";
	cat[1][0] = "1 ";
	for (int i = 2; i < n; ++i) {
		int cur = 0;
		for (int j = 0; j < i;++j) {
			_offset = j + 1;
			for (int k = 0; k < Catalan[j]; ++k) {
				for (int l = 0; l < Catalan[i - 1 - j]; ++l) {
					cat[i][cur++] = cat[j][k] + cat[i - 1 - j][l];
				}
			}
		}
	}

	for (int j = 0; j < n; ++j) {
		_offset = j + 1;
		for (int k = 0; k < Catalan[j]; ++k) {
			for (int l = 0; l < Catalan[n - 1 - j]; ++l) {
				cout << cat[j][k] + cat[n - 1 - j][l];
				cout << endl;
			}
		}
	}
	/*for (int j = 0; j < n; ++j) {
		_offset = j + 1;
		for (int k = 0; k < Catalan[j]; ++k) {
			for (int l = 0; l < Catalan[n - 1 - j]; ++l) {
				MyString temp = cat[j][k] + cat[n - 1 - j][l];
				temp.print();
			}
		}
	}
*/
	DWORD end_time = GetTickCount();				//计时结束
	cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;


	system("pause");
	return 0;
}
