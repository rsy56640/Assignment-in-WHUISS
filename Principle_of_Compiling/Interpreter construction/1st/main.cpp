/* Sample input format:
 *     1: G[N]                     //
 *     2: N D                      // one space seperated
 *     3: N::=ND|D                 // one line per production
 *     n: D::=0|1|2|3|4            // e represents epsilon
 *
 * Sample output format:
 *     1: G[N] = ({N, D}, {0, 1, 2, 3, 4}, P, N)
 *     2: P: N::=ND|D
 *     3:    D::=0|1|2|3|4
 *     4: Chomsky2 grammer
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <variant>
#include <numeric>
#include <algorithm>
#include <iterator>

enum class grammer_t { U, I, II, II_Ex, III, III_Ex };
const char* grammer[6] = { "0-grammer","1-grammer","2-grammer",
"2-grammer extension","3-grammer","3-grammer extension" };

/*
 * determine the grammer type of the text
 * return std::string if some error occurs.
 */
std::variant<std::string, grammer_t> judge(const std::string& filename)
{
	constexpr std::size_t MAXSIZE = 256;
	char buffer[MAXSIZE];
	std::ifstream inputFile{ filename, std::ios::in };
	if (!inputFile.is_open()) {
		return std::string("failed to open ") + filename;
	}
	else {
		std::size_t line_unm = 0;
		std::set<char> VN, VT;
		std::unordered_map<char, bool> VN_occur; // check whether the VN occur in productions.
		grammer_t g_t = grammer_t::III_Ex;
		char start_symbol;
		std::list<std::string> productions;
		std::unordered_map<std::string, std::set<std::string>> productions_head;
		bool has_e = false;
		while (!inputFile.eof())
		{
			inputFile.getline(buffer, MAXSIZE - 1);
			line_unm++;
			const std::size_t length = strlen(buffer);
			if (line_unm == 1) // G[N]
			{
				start_symbol = buffer[2];
				VN.insert(start_symbol);
				VN_occur[start_symbol] = false;
				continue;
			}
			if (line_unm == 2) // VN: N D...
			{
				bool is_match = false;
				for (std::size_t i = 0; i < length; i += 2)
				{
					char c = buffer[i];
					if (c == start_symbol) is_match = true;
					VN.insert(buffer[i]);
					VN_occur[buffer[i]] = false;
				}
				if (!is_match)
					return std::string{ "The start symbol is not in VN." };
				continue;
			}
			// productions
			productions.push_back(std::string(buffer, length));
			std::size_t i = 0u;
			for (; i < length; i++)
				if (buffer[i] == ':')break;
			static const auto tokenize = [](std::string const &str, const char delim,
				std::vector<std::string> &out)
			{
				std::size_t start;
				std::size_t end = 0;
				while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
				{
					end = str.find(delim, start);
					out.push_back(str.substr(start, end - start));
				}
			};
			static const auto change_grammer = [](grammer_t& g_t, grammer_t _g_t)
			{
				if (static_cast<int>(_g_t) < static_cast<int>(g_t))
					g_t = _g_t;
			};
			if (i == 1) // A::=abc|N|e, grammer is II or III.
			{
				if (VN.find(buffer[0]) == VN.end())
					return std::string("'") + std::string(1, buffer[0])
					+ "'" + " is not in VN at line: " + std::to_string(line_unm);
				VN_occur[buffer[0]] = true;

				// judge whether the grammer is III.
				std::vector<std::string> items;
				tokenize(std::string(buffer + 4, length - 4), '|', items);
				grammer_t cur_g_t = grammer_t::III_Ex;
				for (auto const& item : items)
				{

					productions_head[std::string(1, buffer[0])].insert(item);

					if (item.size() == 1 && VN.find(item[0]) == VN.end()) // A -> a
						VT.insert(item[0]);
					else if (item.size() == 2 && VN.find(item[0]) == VN.end() // A -> aB
						&& VN.find(item[1]) != VN.end() && item[1] != buffer[0])
						VT.insert(item[0]);
					else // => II_Ex or II
					{
						cur_g_t = grammer_t::II_Ex;
						for (char c : item)
							if (VN.find(c) == VN.end())
								VT.insert(c);
					}
					if (item.size() == 1 && item[0] == 'e')has_e = true;
				}
				change_grammer(g_t, cur_g_t);
			}
			else // aBc::=...|..|.. compare the length, 0 or I grammer
			{
				std::size_t i = 0u;
				for (; buffer[i] != ':'; i++)
				{
					if (VN.find(buffer[i]) == VN.end())
						VT.insert(buffer[i]);
					else VN_occur[buffer[i]] = true;
				}
				std::vector<std::string> items;
				tokenize(std::string(buffer + i + 3, length - (i + 3)), '|', items);

				for (auto const& item : items)
					productions_head[std::string(buffer, i)].insert(item);

				std::size_t min_size = std::accumulate(items.begin(), items.end(), std::numeric_limits<std::size_t>::max(),
					[](std::size_t _min, const std::string& s) {return std::min(_min, s.size()); });
				if (min_size >= i) change_grammer(g_t, grammer_t::I);
				else change_grammer(g_t, grammer_t::U);
			}
		} // while(eof)
		inputFile.close();

		// adjust the grammar_type according to epsilon
		static const auto erase_Ex = [](grammer_t& g_t)
		{
			if (g_t == grammer_t::III_Ex)g_t = grammer_t::III;
			else if (g_t == grammer_t::II_Ex)g_t = grammer_t::II;
		};
		static const auto down_cast = [](grammer_t& g_t)
		{
			if (g_t == grammer_t::I)g_t = grammer_t::U;
		};

		VT.erase('e');
		if (!has_e) erase_Ex(g_t);
		if (has_e) down_cast(g_t);

		// check VN symbols do not occur
		std::vector<char> VN_not_occur;
		for (auto c : VN_occur)
		{
			if (!c.second)
				VN_not_occur.push_back(c.first);
		}
		if (!VN_not_occur.empty())
		{
			std::string VN_not_occur_s = "'";
			VN_not_occur_s += std::string(1, VN_not_occur[0]) + "'";
			const std::size_t size = VN_not_occur.size();
			for (std::size_t i = 1; i < size; i++)
				VN_not_occur_s += " ,'" + std::string(1, VN_not_occur[i]) + "'";
			std::string p = " doesn't";
			if (size > 1)p = " don't";
			return std::string("VN: ") + VN_not_occur_s + p + " occur in productions";
		}

		// output
		std::ostream_iterator<char> _Oit(std::cout, ", ");
		std::cout << std::string("G[") + start_symbol + "] = ({";
		std::copy(VN.begin(), --VN.end(), _Oit); std::cout << *--VN.end() << "}, {";
		std::copy(VT.begin(), --VT.end(), _Oit); std::cout << *--VT.end() << "}, P, ";
		std::cout << start_symbol << ")" << std::endl;
		std::cout << "P:";
		for (auto p = productions_head.begin(); p != productions_head.end(); ++p)
		{
			if (p == productions_head.begin()) std::cout << "  " << p->first << "::= ";
			else std::cout << "    " << p->first << "::= ";
			for (auto item = p->second.begin(); item != p->second.end(); ++item)
			{
				if (item == p->second.begin())
					std::cout << *item;
				else std::cout << "|" << *item;
			}
			std::cout << std::endl;
		}
		return g_t;
	}
}

int main()
{
	const std::vector<std::string> filenames = { "Chomsky0.txt" ,
		"Chomsky1.txt" ,"Chomsky2.txt" , "Chomsky3.txt", "Chomsky2Ex.txt",
		"Chomsky3Ex.txt", "Chomsky_error1.txt", "Chomsky_error2.txt", "Chomsky_error3.txt" };
	for (auto& filename : filenames)
		std::visit(
			[](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, std::string>)
			std::cout << arg
			<< "\n\n-------------------------------------------\n"
			<< std::endl;
		else if constexpr (std::is_same_v<T, grammer_t>)
			std::cout << grammer[static_cast<int>(arg)]
			<< "\n\n-------------------------------------------\n"
			<< std::endl;
	}, judge(filename));
	system("pause");
	return 0;
}