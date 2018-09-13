#include "token.h"
using namespace lexical;



int main()
{

	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	const int num = 6;
	char* c[num];
	c[0] = "a=(3+4.8)* -.3 + 9/4.5;";
	c[1] = "  d;";
	c[2] = " b =a= (a + +  7.1 ) -.722*  (-3.770224058066800548405486 - a);";
	c[3] = "c= b    =.32*  a+-.7 + 3/2;";	// 3/2 is to be calculated as 1;
	c[4] = "print(a );";
	c[5] = "print(b -3);";




	using lexical::symbol_table;
	_STD vector<int> index(num);
	for (int i = 0; i < num; ++i)
		for (; c[i][index[i]] != ';'; ++index[i]);
	_STD vector<Token> v[num];
	[[maybe_unused]]_STD pair<double, bool> result;

	try
	{

		for (int i = 0; i < num; ++i)
		{
			v[i] = seperate_one_sentence(c[i], index[i]);
			execution_one_sentence(v[i]);
		}

	}
	catch (Exception_base& e)
	{
		e.print_exception();
	}

	/*
	symbol_table["rsy"] = _STD make_pair(0.0, true);
	_STD cout << static_cast<double>(symbol_table["a"].first)
		<< "\t" << symbol_table["b"].first << _STD endl;
		*/


	system("pause");
	return 0;
}