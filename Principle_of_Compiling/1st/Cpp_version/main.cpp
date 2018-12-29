#include "token.h"
#include <iomanip>
using namespace lexical;



int main()
{

	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	extern _STD unordered_map<_STD string, _STD pair<double, bool> > symbol_table;

	std::cout << "input format:" << std::endl;
	std::cout << "\t1. expr        -> like \"a=(3+4.8)* -.3 + 9/4.5\" or  \"b =a= (a + +  7.1 ) -.722\"" << std::endl;
	std::cout << "\t2. print(expr) -> output the value of the expression." << std::endl;
	std::cout << "\t3. dict        -> output the dictionary table." << std::endl;
	std::cout << std::endl << "----------------------------------------------------------" << std::endl << std::endl;
	std::size_t line_unm = 0;
	while (true)
	{
		if (line_unm == 100) break;
		std::cout << ++line_unm << " >>> ";
		std::string input;
		std::getline(std::cin, input);
		if (input.compare("dict") == 0)
		{
			addLine();
			for (auto it : symbol_table)
				std::cout << it.first << std::setw(16 - it.first.size())
				<< it.second.first << std::endl;
			continue;
		}
		try
		{
			std::vector<Token> tokens = seperate_one_sentence(input.c_str(), input.size());
			execution_one_sentence(tokens);
		}
		catch (Exception_base& e)
		{
			e.print_exception();
		}
	}

	/*
	symbol_table["rsy"] = _STD make_pair(0.0, true);
	_STD cout << static_cast<double>(symbol_table["a"].first)
		<< "\t" << symbol_table["b"].first << _STD endl;
		*/


	std::getchar();
	return 0;
}