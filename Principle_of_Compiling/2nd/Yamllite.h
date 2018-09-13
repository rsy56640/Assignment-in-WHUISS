#pragma once
#ifndef _YAMLLITE_H
#define _YAMLLITE_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <tuple>
#include <memory>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#ifndef _STD 
#define _STD ::std::
#endif
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace YAMLlite
{

	/*
	 * kv       : 8, "key: value" key: string(value ""), number, scientific_notation, bool
	 * key      : 4, "key:"       kv (but value is array or kv)
	 * value    : 2, "- value"    trivial value type of the array
	 * new_line : 1, "-"          to start a new line, indicating the value is "kv" or "array"
	**/
	enum class line_t { kv = 8, key = 4, value = 2, new_line = 1 };


	/*
	 * -parse :
	 * -json  :
	 * -find  :
	**/
	enum class option { parse, json, find };


	/*
	 * 1 indent means 2 spaces
	 * str2 maybe null
	**/
	typedef struct line
	{
		std::size_t _line_num;
		std::size_t _indent;
		line_t _line_type;
		std::string str1, str2;
		line() = default;
		line(std::size_t line_num, std::size_t indent, line_t line_type, const std::string& s1 = "", const std::string& s2 = "")
			:_line_num(line_num), _indent(indent), _line_type(line_type), str1(s1), str2(s2)
		{}
		line(const line&) = default;
		line(line&&) = default;
		line& operator=(const line&)& = default;
		line& operator=(line&&)& = default;
	};


	typedef struct Yaml_Base_Exception
	{
		void printException() const
		{
			this->printEx();
		}
		virtual void printEx() const = 0;
	};
	struct Yaml_Exception :Yaml_Base_Exception
	{
		std::string _msg;
		std::size_t _line, _position;
		Yaml_Exception(const std::string& msg, std::size_t line, std::size_t position)
			:_msg(msg), _line(line), _position(position) {}
		void printEx() const
		{
			std::cout << (*this) << std::endl;
		}
		friend _STD ostream& operator<<(_STD ostream& os, const Yaml_Exception& e)
		{
			os << e._msg << " in the line: " << e._line << ", at position: " << e._position;
			return os;
		}
	};
	using Ex = Yaml_Exception;
	struct Yaml_find_Exception :Yaml_Base_Exception
	{
		std::string _msg;
		Yaml_find_Exception(const std::string& msg)
			:_msg(msg) {}
		void printEx() const
		{
			std::cout << (*this) << std::endl;
		}
		friend _STD ostream& operator<<(_STD ostream& os, const Yaml_find_Exception& e)
		{
			os << e._msg;
			return os;
		}
	};


	/*
	 * return the table which stores the line object
	**/
	_STD shared_ptr<_STD vector<line>> table();


	/*
	 * Parameter list:
	 *     c points to one line to be preprocessed
	 * Return value:
	 *     std::size_t : line_num
	 *     const char* : string
	 *     std::size_t : the size of valid line   (  int: 233  #123  #the result is 10)
	 * Exception Handling:
	 *     if 2 space segment exist except the beginning (   xxx xx xx)
	**/
	extern _STD tuple<std::size_t, const char*, std::size_t>
		preprocess(std::size_t line_num, const char* c);


	/*
	 * Description:
	 *     process one line and put it into table
	 * Parameter list:
	 *     std::size_t : line_num
	 *     const char* : string
	 *     std::size_t : the size of valid line   (int: 233  #123  #the result is 8)
	 * Exception Handling:
	 *
	 *
	**/
	extern void seperate_one_line(std::size_t line_num, const char* c, std::size_t size);


	/*
	 * Parameter list:
	 *     option      : (-parse), (-json), (-find)
	 *     const char* : search_path if option is 2(-find), filename if option is 1(-json)
	**/
	extern void parsing(option, const char* search_path);


	//for test
	extern void display();


}//end namespace YAMLlite

#endif // !_YAMLLITE_H