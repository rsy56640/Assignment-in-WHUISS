#include "token.h"

namespace lexical
{
	_STD unordered_map<_STD string, _STD pair<double, bool> > symbol_table = {};

	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//*c points to one sentence including the last semicolon
	_STD vector<Token> seperate_one_sentence(const char* c, const _STD size_t size)
	{
		sentence_num++;
		const static _STD vector<double> digit_after_dot =
		{
			1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001,
			0.00000001, 0.000000001, 0.0000000001, 0.00000000001,
			0.000000000001, 0.0000000000001, 0.00000000000001,
			0.000000000000001, 0.0000000000000001, 0.00000000000000001,
			0.000000000000000001, 0.0000000000000000001, 0.00000000000000000001
		};
		const static _STD unordered_set<int> digit =
		{
			1,2,3,4,5,6,7,8,9,0
		};
		const static _STD unordered_set<char> op =
		{
			'+','-','*','/','=','(',')'
		};
		const static _STD unordered_set<char> token =
		{
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
			'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
			'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '$', '_'
		};

		_STD vector<Token> token_list;
		const int length = size;
		int cur_pos = 0;

		while (cur_pos < length)
		{

			//ignore the space
			if (c[cur_pos] == ' ')
			{
				cur_pos++;
				continue;
			}


			//a const
			bool is_const = false;
			bool has_dot = false;
			bool is_minus = false;
			double num = 0.0;
			int carry_after_dot = 0;

			//such as  +1,  a + -2
			if (c[cur_pos] == '+' || c[cur_pos] == '-')
			{

				//valid situation
				//there is already an op or equal before +, -
				if (token_list.empty()
					|| static_cast<int>(token_list.back()._ty) == 2		//op
					|| static_cast<int>(token_list.back()._ty) == 4		//equal
					|| static_cast<int>(token_list.back()._ty) == 5		//left parenthesis
					)
				{
					//lest the case that	x ++2;
					if (cur_pos > 0 && c[cur_pos] == c[cur_pos - 1])
						throw Lexical_Exception(
							_STD string("unexpected operator")
							.append(" '")
							.append(_STD string(&c[cur_pos], 1))
							.append("'"),
							sentence_num, cur_pos);

					is_minus = (c[cur_pos] == '-');
					cur_pos++;
					//invalid successed content
					while (c[cur_pos] == ' ')cur_pos++;
					if (digit.find(char_to_int(c[cur_pos])) == digit.end()
						&& c[cur_pos] != '.')
						throw Lexical_Exception(
							_STD string("unexpected operator")
							.append(" '")
							.append(_STD string(&c[cur_pos], 1))
							.append("'"),
							sentence_num, cur_pos);
				}

			}

			//then scan the number
			while (digit.find(char_to_int(c[cur_pos])) != digit.end()
				|| c[cur_pos] == '.')
			{
				is_const = true;
				if (c[cur_pos] == '.')
				{
					if (has_dot)
						throw Lexical_Exception(
							_STD string("unexpected operator")
							.append(" '.'"),
							sentence_num, cur_pos);
					has_dot = true;
					carry_after_dot = 0;
					cur_pos++;
					continue;
				}
				if (!has_dot)
				{
					num *= 10.0;
					num += static_cast<double>(char_to_int(c[cur_pos]));
					cur_pos++;
				}
				else
				{
					//too many after dot
					if (carry_after_dot >= 20)
					{
						cur_pos++;
						continue;
					}
					carry_after_dot++;
					num += static_cast<double>(char_to_int(c[cur_pos]))
						* digit_after_dot[carry_after_dot];
					cur_pos++;
				}
			}
			if (is_const)
			{

				if (is_minus)
					num *= -1.0;

				if (has_dot)
					token_list.emplace_back(
						static_cast<type>(1), static_cast<_STD string>(""), num);
				else
				{
					token_list.emplace_back(
						static_cast<type>(1), static_cast<_STD string>(""),
						static_cast<int>(num), false);
				}

				continue;
			}


			//an operand
			if (op.find(c[cur_pos]) != op.end())
			{
				// op "="	type 4
				if (c[cur_pos] == '=')
				{
					token_list.emplace_back(
						static_cast<type>(4), _STD string(&c[cur_pos], 1));
					cur_pos++;
					continue;
				}

				// op "("	type 5
				if (c[cur_pos] == '(')
				{
					token_list.emplace_back(
						static_cast<type>(5), _STD string(&c[cur_pos], 1));
					cur_pos++;
					continue;
				}

				// op ")"	type 6
				if (c[cur_pos] == ')')
				{
					token_list.emplace_back(
						static_cast<type>(6), _STD string(&c[cur_pos], 1));
					cur_pos++;
					continue;
				}

				//other op
				token_list.emplace_back(
					static_cast<type>(2), _STD string(&c[cur_pos], 1));
				cur_pos++;
				continue;
			}


			//a word
			bool is_word = false;
			int word_count = 0;
			_STD string tmp;
			while (token.find(c[cur_pos]) != token.end())
			{
				is_word = true;
				cur_pos++;
				word_count++;
			}
			if (is_word)
			{
				tmp = _STD forward<_STD string>
					(_STD string(&c[cur_pos - word_count], word_count));
				//Token "print"
				if (tmp.compare("print") == 0)
				{
					token_list.emplace_back(
						static_cast<type>(3), tmp);
					continue;
				}
				token_list.emplace_back(
					static_cast<type>(0), tmp);
				continue;
			}


			//other than the const/op/word
			//throw exception
			throw Lexical_Exception(
				_STD string("unexpected operator")
				.append(" '")
				.append(_STD string(&c[cur_pos], 1))
				.append("'"),
				sentence_num, cur_pos);

		}//end while

		return token_list;

	}//end function seperate_one_sentence(char*, const _STD size_t);



	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//execute one sentence
	void execution_one_sentence(const _STD vector<Token>& token_list)
	{

		//allow 3 operations:
		//1. do declaration		ex: a;				//initialize with 0.0
		//2. assignment			ex: a = 4.0 + -.72;		b = a;		c = b = a *   +.777;
		//3. print				ex: print(a);	print(a + b * 4);

		const int size = token_list.size();
		if (0 == size)return;


		//1. do declaration
		if (size == 1)
		{
			auto& token = token_list[0];
			if (static_cast<int>(token._ty) == 0)
			{
				symbol_table[token._symbol] = _STD make_pair(0.0, true);
				return;
			}
			throw Token_list_Exception("unecpected token ", sentence_num, token_list[0]);
		}



		//2. assignment
		//for such statement:
		//c = b = a * 2; 
		//first calculate the expression a*2
		//then push the b,c into symbol table
		if (static_cast<int>(token_list[0]._ty) == 0)
		{

			//assignment has right associativity
			//so find the last '='
			int cur = size - 1;
			for (; cur >= 0; cur--)
			{
				if (static_cast<int>(token_list[cur]._ty) == 4)break;
			}

			//no '='
			if (cur == -1)
				throw Token_list_Exception("unecpected token ", sentence_num, token_list[1]);

			//the last is '='
			if (cur == size - 1)
				throw Token_list_Exception("unecpected token ", sentence_num, token_list[cur]);

			//verify whether the sequence satisfy "id = id = ... = expr"
			for (int i = 1; i < cur; ++i)
			{
				if (static_cast<int>(token_list[i]._ty) != ((i & 1) ? 4 : 0))
					throw Token_list_Exception("unecpected token", sentence_num, token_list[i]);
			}


			//put expr into the calculation function
			//then process from right to left

			//fork the expr
			_STD vector<Token> expr(size - cur - 1);
			for (int i = cur + 1, j = 0; i < size; ++i, ++j)
				expr[j] = token_list[i];
			auto number = calculate(expr, cur + 1);

			//process from right to left
			int i = cur - 1;
			for (; i > 0; i -= 2)
			{
				auto& token = token_list[i];
				auto search = symbol_table.find(token._symbol);
				if (search == symbol_table.end())
					throw Token_list_Exception
					("There is no such identifier: ", sentence_num, token);
				symbol_table[token._symbol] = number;
			}

			//now i = 0
			symbol_table[token_list[0]._symbol] = number;
			return;
		}



		//3. print
		//only one form:
		// 'print'    '('    'expr'    ')'
		//put expr into the calculate function
		if (static_cast<int>(token_list[0]._ty) == 3)
		{

			//verify the form
			if (size < 4
				|| static_cast<int>(token_list[1]._ty) != 5
				|| static_cast<int>(token_list[size - 1]._ty) != 6)
				throw Token_list_Exception
				("unexpected ", sentence_num, token_list[size - 1]);

			//calculate the expr
			_STD vector<Token> expr(size - 3);
			for (int i = 2, j = 0; i < size - 1; ++i, ++j)
				expr[j] = token_list[i];
			auto number = calculate(expr, 3);
			//if (!number.second)
			std::cout << static_cast<double>(number.first) << std::endl;
			// always treat as double
			//else std::cout << static_cast<double>(number.first) << std::endl;

			return;
		}


		//no id/print match
		throw Token_list_Exception("unecpected token ", sentence_num, token_list[0]);

	}//end fucntion execution_one_sentence(_STD vector<Token>);



	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//calcuate the expression
	_STD pair<double, bool>
		calculate(const _STD vector<Token>& expr_list, const _STD size_t pos)
	{
		//[[maybe_unused]]bool is_double = false;	//judge whether the result is double
		int size = expr_list.size();
		//if (size == 0)return NULL;
		int cur_pos = 0;
		bool next_identifer = true;	//only identifier/constant/left parenthesis permitted
		int left_parenthesis = 0, right_parenthesis = 0;

		//verify whether the expr_list is legal
		while (cur_pos < size)
		{

			//left parenthesis
			if (static_cast<int>(expr_list[cur_pos]._ty) == 5)
			{
				cur_pos++;
				left_parenthesis++;
				next_identifer = true;
				continue;
			}

			//right parenthesis
			if (static_cast<int>(expr_list[cur_pos]._ty) == 6)
			{
				if (next_identifer)
					throw Expr_Exception("There is something wrong near",
						sentence_num, cur_pos + pos, expr_list[cur_pos]);
				cur_pos++;
				right_parenthesis++;
				next_identifer = false;
				continue;
			}

			//identifier / constant
			if (static_cast<int>(expr_list[cur_pos]._ty) < 2)
			{
				if (!next_identifer)
					throw Expr_Exception("There is something wrong near",
						sentence_num, cur_pos + pos, expr_list[cur_pos]);
				cur_pos++;
				next_identifer = false;
				continue;
			}

			//operator
			if (static_cast<int>(expr_list[cur_pos]._ty) == 2)
			{
				if (next_identifer)
					throw Lexical_Exception("There is something wrong near",
						sentence_num, cur_pos + pos);
				cur_pos++;
				next_identifer = true;
				continue;
			}

		}//end verify
		if (next_identifer)
			throw Expr_Exception("unexpected end of expr",
				sentence_num, cur_pos + pos, expr_list[cur_pos - 1]);
		if (left_parenthesis != right_parenthesis)
			throw Expr_Exception("Parentheses does not match",
				sentence_num, cur_pos + pos, expr_list[cur_pos - 1]);



		//using two stacks to calculate the expression
		//priority: +, -: 1		*, /: 2		(: 3		):0
		//meet number, push into stack.
		//meet operator, compare the current priority with the previous,
		//	if previous are higher or equal, then do the previous operation,
		//	if current are higher, then push the current operator into the stack.

		struct Operator
		{
			char _c;
			int _priority;
			Operator(char c, int priority)
				:_c(c),
				_priority(priority)
			{}
		};


		struct Number
		{
			double _num;
			bool _is_double;
			Number(double num, bool is_double)
				:_num(num),
				_is_double(is_double)
			{}
		};


		_STD stack<Operator> operator_stack;
		_STD stack<Number> number_stack;

		cur_pos = 0;

		while (cur_pos < size)
		{

			auto& token = expr_list[cur_pos];

			//idnetifier
			if (static_cast<int>(token._ty) == 0)
			{
				auto search = symbol_table.find(token._symbol);
				if (search != symbol_table.end())
				{
					cur_pos++;
					number_stack.emplace((*search).second.first, (*search).second.second);
				}
				else
				{
					throw Id_Exception("There is no such identifier: ",
						token._symbol, sentence_num, cur_pos + pos);
				}
				continue;
			}


			//constant
			if (static_cast<int>(token._ty) == 1)
			{
				cur_pos++;
				number_stack.emplace(token._num, token._is_double);
				continue;
			}


			//operator	+, -, *, /
			if (static_cast<int>(token._ty) == 2)
			{
				const char c = token._symbol[0];
				cur_pos++;

				//no operator in the stack
				if (operator_stack.empty())
				{
					if (c == '+' || c == '-')
						operator_stack.emplace(c, 1);
					else operator_stack.emplace(c, 2);
					continue;
				}

				//	if previous are higher or equal, then do the previous operation,
				//	if current are higher, then push the current operator into the stack.
				auto& op = operator_stack.top();
				int _priority = op._priority;

				int cur_priority = 1;
				if (c == '*' || c == '/')cur_priority = 2;

				//left parenthesis
				if (_priority == 3)
				{
					operator_stack.emplace(c, cur_priority);
					continue;
				}

				//push current
				if (cur_priority > _priority)
					operator_stack.emplace(c, cur_priority);

				//do previous operation
				else
				{
					auto& num2 = number_stack.top(); number_stack.pop();
					auto& num1 = number_stack.top(); number_stack.pop();
					operator_stack.pop();
					double result = 0.0;
					bool is_double = false;
					//do previous operation
					switch (op._c)
					{
					case '+':
						result = num1._num + num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '-':
						result = num1._num - num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '*':
						result = num1._num * num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '/':
						if (abs(num2._num) < 1e-6)
							throw Lexical_Exception("Divided by zero", sentence_num, pos + cur_pos);
						result = num1._num / num2._num;
						is_double = num1._is_double || num2._is_double;
						if (!is_double)
							result = static_cast<double>(result); // always treat as double
						break;
					}


					//push current op, and push the new constant
					operator_stack.emplace(c, cur_priority);
					number_stack.emplace(result, is_double);

				}

				continue;

			}


			//left parenthesis
			if (static_cast<int>(token._ty) == 5)
			{
				cur_pos++;
				operator_stack.emplace('(', 3);
				continue;
			}


			//right parenthesis
			if (static_cast<int>(token._ty) == 6)
			{

				//pop the operator stack until meet '('
				while (!operator_stack.empty() && operator_stack.top()._c != '(')
				{
					auto& op = operator_stack.top(); operator_stack.pop();
					auto& num2 = number_stack.top(); number_stack.pop();
					auto& num1 = number_stack.top(); number_stack.pop();
					double result = 0.0;
					bool is_double = false;
					//do operation
					switch (op._c)
					{
					case '+':
						result = num1._num + num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '-':
						result = num1._num - num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '*':
						result = num1._num * num2._num;
						is_double = num1._is_double || num2._is_double;
						break;
					case '/':
						if (abs(num2._num) < 1e-6)
							throw Lexical_Exception("Divided by zero", sentence_num, pos + cur_pos);
						result = num1._num / num2._num;
						is_double = num1._is_double || num2._is_double;
						if (!is_double)
							result = static_cast<double>(result); // always treat as double
						break;
					}
					number_stack.emplace(result, is_double);
				}//end while

				//if empty
				if (operator_stack.empty())
					throw Lexical_Exception
					("unexpected right parenthesis ')'", sentence_num, cur_pos + pos);

				//if top = '( '
				else
				{
					operator_stack.pop();
					cur_pos++;
					continue;
				}

			}// end if right perenthesis

		}//end traverse of the expression


		//do the final calculation
		while (!operator_stack.empty())
		{
			auto& op = operator_stack.top(); operator_stack.pop();
			auto& num2 = number_stack.top(); number_stack.pop();
			auto& num1 = number_stack.top(); number_stack.pop();
			double result = 0.0;
			bool is_double = false;
			//do operation
			switch (op._c)
			{
			case '+':
				result = num1._num + num2._num;
				is_double = num1._is_double || num2._is_double;
				break;
			case '-':
				result = num1._num - num2._num;
				is_double = num1._is_double || num2._is_double;
				break;
			case '*':
				result = num1._num * num2._num;
				is_double = num1._is_double || num2._is_double;
				break;
			case '/':
				if (abs(num2._num) < 1e-6)
					throw Lexical_Exception("Divided by zero", sentence_num, pos + cur_pos);
				result = num1._num / num2._num;
				is_double = num1._is_double || num2._is_double;
				if (!is_double)
					result = static_cast<double>(result); // always treat as double
				break;
			}
			number_stack.emplace(result, is_double);
		}//end while

		auto& result = number_stack.top();
		return _STD make_pair(result._num, result._is_double);

	}//end function calculate(_STD vector<Token>, const _STD size_t)

	void addLine() { sentence_num++ ; }

}//end namespace