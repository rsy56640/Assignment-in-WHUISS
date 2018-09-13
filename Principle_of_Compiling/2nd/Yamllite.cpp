#include "Yamllite.h"

namespace YAMLlite
{

	_STD shared_ptr<_STD vector<line>> table()
	{
		static _STD shared_ptr<_STD vector<line>>
			table{ _STD make_shared<_STD vector<line>>() };
		return table;
	}


	_STD tuple<std::size_t, const char*, std::size_t>
		preprocess(std::size_t line_num, const char* c)
	{
		std::size_t count = 0;
		const std::size_t length = strlen(c);
		if (0 == length)
			return _STD make_tuple(line_num, c, 0);
		//ignore comments
		for (; count < length; ++count)
			if (c[count] == '#')break;
		//ignore space at the end
		count--;
		while (count >= 0 && c[count] == ' ')
			count--;
		//Exception Handling
		//such as:  xxxx  xxx xxx
		{
			bool prev_space = false;
			int space_block_count = 0;
			if (c[0] == ' ')space_block_count--;//considering the begin indentation
			for (int i = 0; i < static_cast<int>(count); ++i)
			{//i is int instead of std::size_t because of Integer Overflow
				if (c[i] == ' ') {
					if (!prev_space)space_block_count++;
					prev_space = true;
				}
				else {
					prev_space = false;
					if (space_block_count > 1)
						throw Ex
					{
						string{ "Unexpected '" }
						.append(string(1, static_cast<char>(c[i])))
						.append("'"),
						line_num, static_cast<std::size_t>(i + 1)
					};
				}
			}
		}
		return _STD make_tuple(line_num, c, ++count);
	}//end function preprocess(std::size_t, const char*);


	/* line_t
	 * kv       : 8, "key: value" key: string(value ""), number, scientific_notation, bool
	 * key      : 4, "key:"       kv (but value is array or kv)
	 * value    : 2, "- value"    trivial value type of the array
	 * new_line : 1, "-"          to start a new line, indicating the value is "kv" or "array"
	**/
	void seperate_one_line(std::size_t line_num, const char* c, std::size_t size)
	{
		if (0 == size)return;
		//key starts with English letter, with following of letter, digit, '_', ends without '_'
		static const _STD  unordered_set<char> key_letter =
		{
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
			'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
			'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'
		};
		//support for int and float
		static const _STD unordered_set<int> digit =
		{
			'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '-', 'e', 'E'
		};
		//check out the indentation
		static std::size_t prev_indent = 0;//indicate the previous indentation
		std::size_t indent = 0;
		for (; indent < size; ++indent)
			if (c[indent] != ' ')break;
		//indentation Exception Handling - preliminary checking
		{
			if (indent > (prev_indent << 1) + 2)
				throw Ex{ "Unexpected extra space", line_num, (prev_indent << 1) + 3 };
			if (indent % 2)
				throw Ex{ "Expected space", line_num, indent + 1 };
		}
		prev_indent = (indent /= 2);
		//scan string and format
		std::size_t cur = indent << 1;

		//function value_scan() : scan (string, number, bool, sci-notation)
		static const _STD function<std::string(const char*, std::size_t, std::size_t)>
			value_scan = [&line_num, &indent](const char* c, std::size_t cur, std::size_t end_pos)->decltype(auto)
		{
			//string
			if (c[cur] == '"' && c[end_pos - 1] == '"')
			{
				if (cur == end_pos - 1)//eg.(   - ")
					throw Ex{ "Expected \"", line_num, end_pos + 1 };
				for (std::size_t i = cur + 1; i < end_pos - 1; i++)
					if (c[i] == '"')
						throw Ex{ "Unexpected \"", line_num, i + 1 };
				return std::string{ c + cur ,end_pos - cur };
			}
			//bool
			if (c[cur] == 't' || c[cur] == 'f')
			{
				if (end_pos - cur == 4 && strncmp(c + cur, "true", 4) == 0)
					return std::string{ c + cur ,end_pos - cur };
				if (end_pos - cur == 5 && strncmp(c + cur, "false", 5) == 0)
					return std::string{ c + cur ,end_pos - cur };
			}
			//number and sci-notation 1.23e-10
			if (digit.find(c[cur]) != digit.end())
			{
				_STD vector<std::size_t> e, dot, minus;
				for (std::size_t i = cur; i < end_pos; ++i)
				{
					if (digit.find(c[i]) == digit.end())
						throw Ex{
						string{ "Unexpected '" }
						.append(string(1, static_cast<char>(c[i])))
						.append("'"),
						line_num, i + 1
					};
					if (c[i] == 'e' || c[i] == 'E')
						e.push_back(i);
					if (c[i] == '.')
						dot.push_back(i);
					if (c[i] == '-')
						minus.push_back(i);
				}
				//attempt to parse int/float
				if (e.size() == 0)
				{
					if (minus.size() > 1)
						throw Ex{ "Unexpected '-'", line_num, minus[1] + 1 };
					if (dot.size() <= 1)
					{
						if (minus.empty() || (minus.size() == 1 && minus[0] == cur))
							return std::string{ c + cur ,end_pos - cur };
						throw Ex{ "Unexpected '-'", line_num, minus[0] + 1 };
					}
					throw Ex{ "Unexpected '.'", line_num, dot[1] + 1 };
				}
				//attempt to parse sci-notation
				if (e.size() == 1)
				{
					const std::size_t e_pos = e[0];
					if (e_pos == cur || e_pos == end_pos - 1)
						throw Ex{ "Unexpected 'e'", line_num, e_pos + 1 };
					if (dot.size() == 0)//1e-10, -23e-20
					{
						std::size_t minus_count = 0;
						if (c[cur] == '-')minus_count++;
						if (c[e_pos + 1] == '-')minus_count++;
						if (minus_count != minus.size())
						{
							for (std::size_t pos : minus)
								if (pos != cur && pos != e_pos + 1)
									throw Ex("Unexpected '-'", line_num, pos + 1);
						}
						/*************************\
						* for the case that "-e-" *  //TODO
						\*************************/
						return std::string{ c + cur ,end_pos - cur };
					}
					if (dot.size() == 1)//1.23e-10, -2.733e-12
					{
						const std::size_t dot_pos = dot[0];
						if (dot_pos > e_pos)
							throw Ex{ "Unexpected '.'", line_num, dot_pos + 1 };
						std::size_t minus_count = 0;
						if (c[cur] == '-')minus_count++;
						if (c[e_pos + 1] == '-')minus_count++;
						if (minus_count != minus.size())
						{
							for (std::size_t pos : minus)
								if (pos != cur && pos != e_pos + 1)
									throw Ex("Unexpected '-'", line_num, pos + 1);
						}
						/*************************\
						* for the case that "-e-" *  //TODO
						\*************************/
						return std::string{ c + cur ,end_pos - cur };
					}
					throw Ex{ "Unexpected '.'", line_num, dot[1] + 1 };
				}
				throw Ex{ "Unexpected 'e'", line_num, e[1] + 1 };
			}
			throw Ex{
				string{ "Unexpected '" }
				.append(string(1, static_cast<char>(c[cur])))
				.append("'"),
				line_num, cur + 1 };
		};//end std::function value_scan();

		//line_t::value(- value) or line_t::new_line(-)
		if (c[cur] == '-')
		{
			//line_t::new_line "-"
			if (cur + 1 == size)
			{
				table()->emplace_back(line_num, indent, line_t::new_line);
				return;
			}

			//line_t::value of the array
			if (c[++cur] == ' ')    //check out one necessary space
			{
				cur++;
				std::string value = value_scan(c, cur, size);
				table()->emplace_back(line_num, indent, line_t::value,
					_STD forward<decltype(value)>(value));
				return;
			}
			else throw Ex{ "Expected space", line_num, cur };
		}

		//line_t::kv or line_t::key
		{
			_STD vector<std::size_t> space;
			for (std::size_t i = cur; i < size; ++i)
				if (c[i] == ' ')
					space.push_back(i);

			//function key_scan(): scan (key:)
			static const _STD function<std::string(const char*, std::size_t, std::size_t)>
				key_scan = [&line_num](const char* c, std::size_t cur, std::size_t end_pos)->decltype(auto)
			{
				if (c[end_pos - 1] != ':')
					throw Ex{ "Expected ':'", line_num, end_pos };
				for (std::size_t i = cur; i < end_pos - 1; ++i)
					if (key_letter.find(c[i]) == key_letter.end())
						throw Ex{
						string{ "Unexpected '" }
						.append(string(1, static_cast<char>(c[i])))
						.append("'"),
						line_num, i + 1 };
				if ((c[cur] < '9' && c[cur] > '0') || c[cur] == '_')
					throw Ex{ "Expected English letter", line_num, cur + 1 };
				if (c[end_pos - 2] == '_')
					throw Ex{ "Unexpected '_'", line_num, end_pos - 1 };
				return std::string{ c + cur, end_pos - cur - 1 };//ignore ':' in "key:"
			};//end std::function key_scan();

			//line_t::key (  xxx:)
			if (space.size() == 0)
			{
				std::string key = key_scan(c, cur, size);
				table()->emplace_back(line_num, indent, line_t::key, _STD forward<decltype(key)>(key));
				return;
			}
			//line_t::kv  (  xxx: xxx,  xxx: "xxx")
			else if (space.size() == 1)
			{
				const std::size_t space_pos = space[0];
				//str1-key
				std::string key = key_scan(c, cur, space_pos);
				//str2-value
				cur = space_pos + 1;
				std::string value = value_scan(c, cur, size);
				table()->emplace_back(line_num, indent, line_t::kv,
					_STD forward<decltype(key)>(key), _STD forward<decltype(value)>(value));
				return;
			}
			else throw Ex{ "Unexpected space", line_num, space[1] };
		}

		throw Ex{
			string{ "Unexpected '" }
			.append(string(1, static_cast<char>(c[cur])))
			.append("'"),
			line_num, cur + 1 };

	}//end function seperate_one_line(std::size_t, const char*, std::size_t);


	 /*
	  * Parameter list:
	  *     option      : (-parse), (-json), (-find)
	  *     const char* : search_path if option is 2(-find), filename if option is 1(-json)
	 **/
	void parsing(option opt, const char* search_path)
	{
		auto tb = table();
		tb->emplace_back(0u, 0u, line_t::kv, "EOF", "\"EOF\"");//to help reduce
		//-parse
		//a trivial measure to check the semantics
		using _key = std::string;
		using _value = std::string;
		_STD unordered_map<_key, _value> find_table;
		_STD vector<_STD pair<_key, _value>> display_table;//only for display
		const std::function<void(std::string, int, std::string)> table_insert_ary =
			[&find_table, &display_table](std::string path, int array_num, std::string value)
		{
			find_table.emplace(std::string{ path }.append("[").append(_STD to_string(array_num)).append("]"), value);
			display_table.push_back(_STD make_pair(std::string{ path }.append("[").append(_STD to_string(array_num)).append("]"), value));
		};//end std::function table_insert_ary(std::string, int, std::string);
		const std::function<void(std::string, std::string, std::string)> table_insert_kv =
			[&find_table, &display_table](std::string path, std::string key, ::string value)
		{
			find_table.emplace(std::string{ path }.append(".").append(key), value);
			display_table.push_back(_STD make_pair(std::string{ path }.append(".").append(key), value));
		};//end std::function table_insert_v(std::string, std::string, std::string);

		typedef struct path {
			YAMLlite::line l;
			std::string cur_path;
			std::string parent_path;
			int array_level;// -1 if no - array, store array_level for reduce
			path(YAMLlite::line _l, const std::string& _cur_path,
				const std::string& _parent_path, int ary_level = -1)
				:l(_l), cur_path(_cur_path), parent_path(_parent_path), array_level(ary_level)
			{}
		};
		_STD stack<path> stk;
		const std::function<void(const line, std::string, std::string)> record_key_path =
			[&stk](const line l, std::string path, std::string key)->decltype(auto)
		{
			stk.emplace(l, string{ path }.append(".").append(key), path, -1);
		};//end std::function record_key_path(const line, std::string, std::string);
		const std::function<void(const line, std::string, int)> record_ary_path =
			[&stk](const line l, std::string path, int array_num)->decltype(auto)
		{
			stk.emplace(l,
				string{ path }.append("[").append(_STD to_string(array_num)).append("]"),
				path, array_num);
		};//end std::function record_ary_path(const line, std::string, int);

		//find_table records the meeting (kv, value).
		//stk records the path when meeting (key, new_line).//更正：存储所有行
		// value 就存储 父目录，

		/*
		 *  0和1指的是这一行与上一行缩进的差， 若 < 0 记为 -1, then reduce
		 *  左列是上一行的类型，值是这一行可以接受的类型。
		 *  ______________________________________________
		 *  |_________________|    0   |    1   |   -1   |
		 *  |  kv       : 8   |  0000  |  0000  |  1111  |   (kv, key) must have 0 indent!!!
		 *  |  key      : 4   |  0000  |  1111  |  0000  |
		 *  |  value    : 2   |  0011  |  0000  |  1111  |   (kv, key) must have 0 indent!!!
		 *  |  new_line : 1   |  0000  |  1111  |  0000  |
		 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		**/
		const _STD map<_STD pair<line_t, int>, int> type_map =
		{
			{ _STD make_pair(line_t::kv, 0), 0 }, { _STD make_pair(line_t::kv, 1), 0 }, { _STD make_pair(line_t::kv, -1), 15 },
			{ _STD make_pair(line_t::key, 0), 0 }, { _STD make_pair(line_t::key, 1), 15 }, { _STD make_pair(line_t::key, -1), 0 },
			{ _STD make_pair(line_t::value, 0), 3 }, { _STD make_pair(line_t::value, 1), 0 }, { _STD make_pair(line_t::value, -1), 15 },
			{ _STD make_pair(line_t::new_line, 0), 0 }, { _STD make_pair(line_t::new_line, 1), 15 }, { _STD make_pair(line_t::new_line, -1), 0 }
		};
		const std::function<bool(line_t, int)> type_cmp =
			[](line_t lt, int type)->decltype(auto)
		{
			return ((static_cast<int>(lt) & type) != 0);
		};//end std::function type_cmp(line_t, int);

		std::size_t array_num = 0;
		std::size_t prev_indent = 0;

		for (auto const& l : *tb)
		{
			if (stk.empty())
			{
				if (l._indent > 0)
					throw Ex{ "Unexpected space", l._line_num, static_cast<std::size_t>(1) };
				if (l._line_type == line_t::kv)
				{
					find_table.emplace(l.str1, l.str2);
					array_num = 0;
					prev_indent = 0;
					continue;
				}
				if (l._line_type == line_t::key)
				{
					stk.emplace(l, l.str1, "", -1);//record key path
					array_num = 0;
					prev_indent = 0;
					continue;
				}
				throw Ex{ "Unexpected '-'", l._line_num, static_cast<std::size_t>(1) };
			}
			else {//stack is not empty
				auto const element = stk.top();
				//the same indent, succeeded value or new_line
				if (l._indent == prev_indent)
				{
					if (!type_cmp(l._line_type, type_map.at(_STD make_pair(element.l._line_type, 0))))
						throw Ex{ "Unexpected indentation", l._line_num, (l._indent << 1) + 1 };
					if (l._line_type == line_t::value)
					{
						table_insert_ary(element.parent_path, ++array_num, l.str1);
						record_ary_path(l, element.parent_path, array_num);
						continue;
					}
					if (l._line_type == line_t::new_line)
					{
						record_ary_path(l, element.parent_path, ++array_num);
						array_num = 0;
						continue;
					}
				}
				//1 more indent
				if (l._indent == prev_indent + 1)
				{
					if (!type_cmp(l._line_type, type_map.at(_STD make_pair(element.l._line_type, 1))))
						throw Ex{ "Unexpected indentation", l._line_num, (l._indent << 1) + 1 };
					if (l._line_type == line_t::kv)
					{
						table_insert_kv(element.cur_path, l.str1, l.str2);
						record_key_path(l, element.cur_path, l.str1);
						array_num = 0;
					}
					if (l._line_type == line_t::key)
					{
						record_key_path(l, element.cur_path, l.str1);
						array_num = 0;
					}
					if (l._line_type == line_t::value)
					{
						array_num = 0;
						table_insert_ary(element.cur_path, ++array_num, l.str1);
						record_ary_path(l, element.cur_path, array_num);
					}
					if (l._line_type == line_t::new_line)
					{
						array_num = 0;
						record_ary_path(l, element.cur_path, ++array_num);
					}
					prev_indent++;
					continue;
				}
				//try to reduce
				if (l._indent < prev_indent)
				{
					if (!type_cmp(l._line_type, type_map.at(_STD make_pair(element.l._line_type, -1))))
						throw Ex{ "Unexpected format", l._line_num, (l._indent << 1) + 1 };

					while (!stk.empty() && stk.top().l._indent != l._indent)
						stk.pop();

					//process this line
					prev_indent = l._indent;
					if (stk.size() == 1)stk.pop();//pop the initial key.....
					if (stk.empty())
					{
						if (line_t::kv == l._line_type)
						{
							find_table.emplace(l.str1, l.str2);
							array_num = 0;
							prev_indent = 0;
							continue;
						}
						if (line_t::key == l._line_type)
						{
							stk.emplace(l, l.str1, "", -1);//record key path
							array_num = 0;
							prev_indent = 0;
							continue;
						}
						throw Ex{ "Unexpected format",l._line_num, 1u };
					}
					else {
						auto const e_top = stk.top();
						array_num = e_top.array_level;
						if (line_t::value == l._line_type)
						{
							table_insert_ary(e_top.parent_path, ++array_num, l.str1);
							record_ary_path(l, e_top.parent_path, array_num);
							continue;
						}
						if (line_t::new_line == l._line_type)
						{
							record_ary_path(l, e_top.parent_path, ++array_num);
							array_num = 0;
							continue;
						}
						throw Ex{ "Unexpected format",l._line_num, (l._indent << 1) + 1 };
					}
				}//end reduce
				throw Ex{ "Unexpected indentation",l._line_num, (prev_indent << 1) + 2 };
			}
		}//end checking semantics


		if (option::parse == opt)
		{
			std::cout << "\nvalid" << std::endl;
		}


		//-json
		if (option::json == opt)
		{
			std::ofstream outfile_json;
			outfile_json.open(
				string(search_path, strlen(search_path) - 3).append("json").c_str(),
				std::ios::out | std::ios::trunc);
			if (!outfile_json.is_open())
			{
				std::cout << "failed to write json file" << std::endl;
				return;
			}
			outfile_json << "{" << std::endl;
			const auto output_indent = [&outfile_json](std::size_t indent)->void
			{
				outfile_json << std::string((indent << 1) + 2, ' ');
			};//end std::function output_indent(std::size_t);
			const std::size_t size = tb->size();
			std::size_t cur = 0;
			std::size_t prev_indent = -1;
			//using a stack to store the level of the indent
			//and ')' '}', then output them in the proper time
			_STD stack<_STD pair<std::size_t, char>> rp;//store ')' and '}'
			const auto back_track =
				[&](std::size_t cur_indent, bool b = true)//b is for the end call
			{
				bool temp = false;
				while (!rp.empty() && rp.top().first >= cur_indent)
				{
					temp = true;
					outfile_json << std::endl;
					output_indent(rp.top().first);
					outfile_json << rp.top().second;
					rp.pop();
				}
				if (temp & b)
					outfile_json << "," << std::endl;
			};
			while (cur < size - 1)//ignore the EOF (I add EOF to the last line of the file)
			{
				const line& l = tb->operator[](cur);
				if (l._indent == prev_indent)
					outfile_json << "," << std::endl;
				back_track(l._indent);
				prev_indent = l._indent;
				output_indent(l._indent);
				cur++;
				if (line_t::kv == l._line_type)
				{
					outfile_json << "\"" << l.str1 << "\"" << ": " << l.str2;
				}
				if (line_t::value == l._line_type)
				{
					outfile_json << l.str1;
				}
				if (line_t::key == l._line_type)
				{
					outfile_json << "\"" << l.str1 << "\":";
					if (static_cast<int>(tb->operator[](cur)._line_type) & 3)//value or new_line
					{
						outfile_json << " [" << std::endl;
						rp.push(_STD make_pair(l._indent, ']'));
					}
					else {//kv or key
						outfile_json << "\n{\n";
						rp.push(_STD make_pair(l._indent, '}'));
					}
				}
				if (line_t::new_line == l._line_type)
				{
					if (static_cast<int>(tb->operator[](cur)._line_type) & 3)//value or new_line
					{
						outfile_json << "[" << std::endl;
						rp.push(_STD make_pair(l._indent, ']'));
					}
					else {//kv or key
						outfile_json << "{\n";
						rp.push(_STD make_pair(l._indent, '}'));
					}
				}
			}//end loop of table
			back_track(0u, false);
			outfile_json << "\n}" << std::endl;
			outfile_json.close();
		}//end branch -json


		//-find
		if (option::find == opt)
		{
			auto result = find_table.find(search_path);
			if (result != find_table.end())
				std::cout << "\nThe result for the search path \"" << search_path <<
				"\" is: " << result->second << std::endl;
			else throw Yaml_find_Exception{
				string{"No result for the  search path \""}.append(search_path).append("\"") };
		}//end branch -find


		std::cout << "\n***** display *****\n" << std::endl;
		display();
		std::cout << "\n***** Key - Value Table *****\n" << std::endl;
		for (auto const it : display_table)
			std::cout << std::left << std::setw(32) << it.first << "\t"
			<< std::left << std::setw(20) << it.second << std::endl;
		std::cout << std::endl;

	}//end function parsing(option, const char*);


	//for test
	void display()
	{
		auto tb = table();
		for (auto const& l : *tb)
		{
			std::printf("line %.3i :", static_cast<int>(l._line_num));
			for (int i = 0; i < l._indent << 1; ++i)
				cout << " ";
			if (l._line_type == line_t::kv)
				cout << l.str1 << ": " << l.str2;
			if (l._line_type == line_t::key)
				cout << l.str1 << ":";
			if (l._line_type == line_t::value)
				cout << "- " << l.str1;
			if (l._line_type == line_t::new_line)
				cout << "- ";
			cout << endl;
		}
	}//end function display();


}//end namespace YAMLlite
