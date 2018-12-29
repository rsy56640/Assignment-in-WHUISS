#pragma once
#ifndef _TOKEN_H
#define _TOKEN_H
#ifndef _STD 
#define _STD ::std::
#endif
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <utility>
#include <cassert>


namespace lexical
{

	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	enum class type { identifier, constant, opcode, print, equal, lp, rp };
	//such as:	a = 3.14 + 7;
	//			b = a / 2.0;
	//			print(b);
	//			print(a - 1);


	//class Token
	class  Token
	{

	public:

		//default constructor for vector
		Token()
			:_ty(type(0)),
			_symbol(""),
			_num(0.0),
			_is_double(true)
		{}

		Token(type ty, _STD string symbol = "", double num = 0.0, bool is_double = true)
			:_ty(ty),
			_symbol(symbol),
			_num(num),
			_is_double(is_double)
		{}

		type _ty;

		_STD string _symbol;	//id, op, lp, rp, semicolon

		double _num;			//const

		bool _is_double;

	};



	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//*c points to one sentence including the last semicolon
	//@ parameter list:
	//@ char* c: one sentence that is seperated by semicolon.
	//@ const _STD size_t size: sentence size that does not include semicolon.
	//@ return value: token_list.
	extern _STD vector<Token> seperate_one_sentence(const char* c, const _STD size_t size);


	//Base class for Exception
	class Exception_base
	{

	public:

		void print_exception()
		{
			this->print();
		}

	private:

		virtual void print() = 0;

	};


	//class Lexical_Exception
	class Lexical_Exception :public Exception_base
	{

	public:
		Lexical_Exception(_STD string msg, _STD size_t sentence_num, _STD size_t pos)
			:_msg(msg),
			_sentence_num(sentence_num),
			_pos(pos)
		{}

		friend _STD ostream& operator<<(_STD ostream& os, Lexical_Exception& e)
		{
			os << e._msg << " in the sentence: " << e._sentence_num << " at position: " << e._pos;
			return os;
		}

	private:

		void print()
		{
			std::cout << (*this) << std::endl;
		}

		_STD string _msg;
		_STD size_t _sentence_num;
		_STD size_t _pos;

	};


	//convert char to int
	inline static int char_to_int(char c)
	{
		return static_cast<int>(c) - 48;
	}


	//record the sentence num according to the semicolon.
	static _STD size_t sentence_num = 0;
	void addLine();


	//class Id_Exception
	class Id_Exception :public Exception_base
	{

	public:

		Id_Exception(_STD string msg, _STD string identifier,
			_STD size_t sentence_num, _STD size_t pos)
			:_msg(msg),
			_identifier(identifier),
			_sentence_num(sentence_num),
			_pos(pos)
		{}

		friend _STD ostream& operator<<(_STD ostream& os, Id_Exception& e)
		{
			os << e._msg << e._identifier << " in the sentence "
				<< e._sentence_num << " at " << e._pos;
			return os;
		}

	private:

		void print()
		{
			std::cout << (*this) << std::endl;
		}

		_STD string _msg;
		_STD string _identifier;
		_STD size_t _sentence_num;
		_STD size_t _pos;

	};


	//symbol table
	extern _STD unordered_map<_STD string, _STD pair<double, bool> > symbol_table;



	//class Token_list exception
	class Token_list_Exception :public Exception_base
	{

	public:
		Token_list_Exception(_STD string msg, _STD size_t sentence_num, Token token)
			:_msg(msg),
			_sentence_num(sentence_num),
			_token(token)
		{}

		friend _STD ostream& operator<<(_STD ostream& os, Token_list_Exception& e)
		{
			os << e._msg;
			if (static_cast<int>(e._token._ty) == 1)
				os << " " << e._token._num;
			else os << " " << e._token._symbol;
			os << " in the sentence " << e._sentence_num;
			return os;
		}

	private:

		void print()
		{
			std::cout << (*this) << std::endl;
		}

		_STD string _msg;
		_STD size_t _sentence_num;
		Token _token;

	};



	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//execute one sentence
	//@ parameter list:
	//@ cnost _STD vector<Token>& token_list: 
	//		token list which contains the seperating tokens
	extern void execution_one_sentence(const _STD vector<Token>& token_list);



	//class expression exception
	class Expr_Exception :public Exception_base
	{

	public:
		Expr_Exception(_STD string msg, _STD size_t sentence_num, _STD size_t pos, Token token)
			:_msg(msg),
			_sentence_num(sentence_num),
			_pos(pos),
			_token(token)
		{}

		friend _STD ostream& operator<<(_STD ostream& os, Expr_Exception& e)
		{
			os << e._msg;
			if (static_cast<int>(e._token._ty) == 0)
				os << " the identifier: " << e._token._symbol;
			if (static_cast<int>(e._token._ty) == 1)
				os << " the number: " << e._token._num;
			if (static_cast<int>(e._token._ty) >= 2)
				os << " the operator: " << e._token._symbol;
			os << " in the sentence " << e._sentence_num << " at " << e._pos;
			return os;
		}

	private:

		void print()
		{
			std::cout << (*this) << std::endl;
		}

		_STD string _msg;
		_STD size_t _sentence_num;
		_STD size_t _pos;
		Token _token;

	};


	//id:0		const:1		op:2	print:3		equal:4		lp:5	rp:6
	//calcuate the expression
	//@ parameter list:
	//@ const _STD vector<Token>& expr_list: an expression list.
	//@ const _STD size_t pos: the relative position to the sentence.
	//@ return value: the result of the expression.
	extern _STD pair<double, bool>
		calculate(const _STD vector<Token>& expr_list, const _STD size_t pos);



}
#endif // !_TOKEN_H
