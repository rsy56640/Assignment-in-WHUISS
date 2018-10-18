#pragma once
#ifndef _APRIORI_H
#define _APRIORI_H
#include <vector>
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <tuple>
#include <utility>
#include <limits>
#include <unordered_map>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#ifdef DEBUG
#include <iostream>
#ifdef OUTPUT_FILE
#include <fstream>
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
#include <Windows.h>
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG
#ifdef SET_IS_COMPARABLE
#include <map>
#include <set>
#else   
#include <unordered_map>
#include <unordered_set>
#endif // SET_IS_COMPARABLE

#include "FPT.hpp"


namespace BI_Apriori
{

#ifdef GET_TIME_ON_WINDOWS
	void print_time(DWORD  start, DWORD end, std::ostream& out, const char* msg)
	{
		std::size_t total_s = (end - start) / 1000;
		std::size_t hours, minutes, seconds;
		minutes = total_s / 60;
		seconds = total_s - minutes * 60;
		hours = minutes / 60;
		minutes = minutes - hours * 60;
		out << msg << hours << "h " << minutes << "min " << seconds << "s" << std::endl << std::endl;
	}
#endif // GET_TIME_ON_WINDOWS

	/*
	 * Apriori Algorithm
	 *
	 * Requirements:
	 *     Set:
	 *          - `count()`
	 *          - `insert()`
	 *          - `erase()`
	 *          - Set::value_type `==`
	 *          - `<`    // comparable or hashable  ???  only choose one
	 *          - Set::value_type is comparable or hashable.
	 *
	 * Macro to be defined:
	 *     ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE  or  ALL_SETS_GREATER_THAN_SUPPORT
	 *     SET_IS_COMPARABLE
	 *     DEBUG
	 */
	template<
		typename Set,
		std::void_t<typename Set::value_type>* = nullptr,
		std::enable_if_t<
		std::is_same_v<std::size_t, decltype(std::declval<Set>().count(std::declval<typename Set::value_type>()))>
		&& std::is_same_v<std::pair<typename Set::iterator, bool>, decltype(std::declval<Set>().insert(std::declval<typename Set::value_type>()))>
		&& std::is_same_v<typename Set::iterator, decltype(std::declval<Set>().erase(std::declval<typename Set::iterator>()))>
		&& std::is_same_v<bool, decltype(std::declval<typename Set::value_type>() == std::declval<typename Set::value_type>())>
#ifdef SET_IS_COMPARABLE
		&& std::is_same_v<bool, decltype(std::declval<Set>() < std::declval<Set>())>
		&& std::is_same_v<bool, decltype(std::declval<typename Set::value_type>() < std::declval<typename Set::value_type>())>
#else   
		&& std::is_same_v<std::size_t, decltype(std::declval<std::hash<Set>::operator()>())>
		&& std::is_same_v<std::size_t, decltype(std::declval<std::hash<typename Set::value_type>::operator()>())>
#endif // SET_IS_COMPARABLE
		>* = nullptr
	> std::vector<std::tuple<Set, std::size_t>> Apriori(const std::vector<Set>& sets, const std::size_t min_support)
	{
		using Item = typename Set::value_type;
#ifdef SET_IS_COMPARABLE
		std::map<Item, std::size_t> total;
#else   
		std::unordered_map<Item, std::size_t> total;
#endif // SET_IS_COMPARABLE
		for (auto const& set : sets)
			for (const Item& item : set)
				total[item]++;
		// prune
		for (auto it = total.begin(); it != total.end(); )
		{
			if (it->second < min_support) it = total.erase(it);
			else ++it;
		}

		std::vector<std::tuple<Set, std::size_t>> return_sets;
#ifdef SET_IS_COMPARABLE
		std::map<Set, std::size_t> prev_sets;
		std::map<Set, std::size_t> cur_sets;
		std::set<Set> extensible_prune; // optimization for ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
#else   
		std::unordered_map<Set, std::size_t> prev_sets;
		std::unordered_map<Set, std::size_t> cur_sets;
		std::unordered_set<Set> extensible_prune;
#endif // SET_IS_COMPARABLE

		for (auto const&[item, count] : total)
		{
			Set s{}; s.insert(item);
			cur_sets[s] = 0;
		}

		/*
		 * mining next level frequent item set.
		 *     1. scan support
		 *     2. erase item set if support < min_support,
		 *        if frequent set is empty, return previous,
		 *        if frequent set has 1 item, return 1 item,
		 *        else join to generate next level item set.
		 *     3. join
		 */

		 // help to judge whether big contains small
		auto const set_contain = [](const Set& small_set, const Set& big_set)->bool
		{
			/*             [[Deprecated]]
			Set s{};
			std::set_intersection(small.begin(), small.end(),
				big.begin(), big.end(),
				std::inserter(s, s.begin()));
			if (small == s) return true;
			else return false;
			*/
#ifdef SET_IS_COMPARABLE
			auto first1 = small_set.begin();
			auto first2 = big_set.begin();
			auto last1 = small_set.end();
			auto last2 = big_set.end();
			while (first1 != last1 && first2 != last2)
			{
				if (*first2 != *first1)
					++first2;
				else {
					++first1;
					++first2;
				}
			}
			if (first2 == last2 && first1 != last1) return false;
			return true;
#else   
			// static_assert(false, "TODO: WTF, how to check one hash contains another hash???");
			for (auto const& item : small)
				if (big.count(item) == 0) return false;
			return true;
#endif // SET_IS_COMPARABLE
		};

#ifdef DEBUG
		std::size_t round = 0;
#ifdef OUTPUT_FILE
		const char* search_path = "./AP.txt";
		std::ofstream AP_output;
#else
		auto& AP_output = std::cout;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
		DWORD  start, end;
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

		while (true)
		{
#ifdef DEBUG

			// In each round, reopen the file and reclose the file,
			// so if the process is terminated,
			// the output has been saved and could be traced.
#ifdef OUTPUT_FILE
			AP_output.open(search_path, std::ios::out | std::ios::app);
			if (!AP_output.is_open())
			{
				std::cout << "failed to open AP.txt" << std::endl;
				exit(0);
			}
#endif // OUTPUT_FILE

			round++;
			AP_output << "round: " << round << "\treturn_sets: " << return_sets.size() << std::endl;
			AP_output << "round: " << round << "\tprev_sets: " << prev_sets.size() << std::endl;
			AP_output << "round: " << round << "\tcur_sets: " << cur_sets.size() << std::endl;
			AP_output << std::endl;
#ifdef OUTPUT_FILE // always output in console
			std::cout << "round: " << round << "\treturn_sets: " << return_sets.size() << std::endl;
			std::cout << "round: " << round << "\tprev_sets: " << prev_sets.size() << std::endl;
			std::cout << "round: " << round << "\tcur_sets: " << cur_sets.size() << std::endl;
			std::cout << std::endl;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

			// 1. scan support
			for (auto &[set, count] : cur_sets)
				for (auto const& item_set : sets)
					if (set_contain(set, item_set)) count++;

#ifdef DEBUG
			AP_output << "round: " << round << "\tscan support" << std::endl;
#ifdef OUTPUT_FILE // always output in console
			std::cout << "round: " << round << "\tscan support" << std::endl;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

			// 2. erase item set if support < min_support.
			for (auto it = cur_sets.begin(); it != cur_sets.end(); )
			{
				if (it->second < min_support) it = cur_sets.erase(it);
				else ++it;
			}

#ifdef DEBUG
			AP_output << "round: " << round << "\terase item set if support < min_support" << std::endl;
			AP_output << "round: " << round << "\tcur_sets: " << cur_sets.size() << std::endl;
#ifdef OUTPUT_FILE // always output in console
			std::cout << "round: " << round << "\terase item set if support < min_support" << std::endl;
			std::cout << "round: " << round << "\tcur_sets: " << cur_sets.size() << std::endl;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

#ifdef ALL_SETS_GREATER_THAN_SUPPORT
#undef ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
			for (auto const&[prev_set, prev_set_count] : prev_sets)
			{
				if (extensible_prune.find(prev_set) != extensible_prune.end()) continue;
				return_sets.push_back(std::make_tuple(prev_set, prev_set_count));
			}
#endif // ALL_SETS_GREATER_THAN_SUPPORT
#ifdef ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
			// add prev_sets to return_sets
			for (auto const&[prev_set, prev_set_count] : prev_sets)
			{
				if (extensible_prune.find(prev_set) != extensible_prune.end()) continue;
				bool is_contain = false;
				for (auto const&[cur_set, cur_set_count] : cur_sets)
				{
					if (set_contain(prev_set, cur_set))
					{
						is_contain = true;
						break;
					}
				}
				if (!is_contain) return_sets.push_back(std::make_tuple(prev_set, prev_set_count));
			}
#endif // ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE

#ifdef DEBUG
			AP_output << "round: " << round << "\tadd prev_sets to return_sets" << std::endl;
			AP_output << "round: " << round << "\treturn_sets: " << return_sets.size() << std::endl;
#ifdef OUTPUT_FILE // always output in console
			std::cout << "round: " << round << "\tadd prev_sets to return_sets" << std::endl;
			std::cout << "round: " << round << "\treturn_sets: " << return_sets.size() << std::endl;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

			// if cur_sets is empty
			if (cur_sets.empty())
			{
#ifndef ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
#ifndef ALL_SETS_GREATER_THAN_SUPPORT
				return_sets.reserve(return_sets.size() + prev_sets.size() - extensible_prune.size());
				for (auto const&[set, count] : prev_sets)
					if (extensible_prune.find(set) != extensible_prune.end())
						return_sets.push_back(set);
#endif //ALL_SETS_GREATER_THAN_SUPPORT
#endif // !ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
#ifdef DEBUG
				AP_output << "round: " << round << "\tcur_sets is empty." << std::endl << std::endl;
#ifdef OUTPUT_FILE // always output in console
				std::cout << "round: " << round << "\tcur_sets is empty." << std::endl << std::endl;
#endif // OUTPUT_FILE
#endif // DEBUG
				return return_sets;
			}

			// if cur_sets has 1 item set
			if (cur_sets.size() == 1)
			{
#ifdef DEBUG
				AP_output << "round: " << round << "\tcur_sets has only 1 item set." << std::endl << std::endl;
#ifdef OUTPUT_FILE // always output in console
				std::cout << "round: " << round << "\tcur_sets has only 1 item set." << std::endl << std::endl;
#endif // OUTPUT_FILE
#endif // DEBUG
				auto const& it = cur_sets.begin();
				return_sets.push_back(std::make_tuple(it->first, it->second));
				return return_sets;
			}

			// 3. join to generate next level item set
			// In join operation, we have some optimization: prune
			cur_sets.swap(prev_sets);
			cur_sets.clear();
			extensible_prune.clear();

			for (auto const&[set, count] : prev_sets)
			{
				Set s = set;
				bool is_extensible = false;

				for (auto const&[item, count] : total)
				{
					if (s.count(item) != 0) continue;
					s.insert(item);
					if (cur_sets.find(s) != cur_sets.end())
					{
						s.erase(item);
						is_extensible = true;
						continue;
					}

					bool subset_ok = true;

					// prune: one's sub item set must be if one's.
					// so check every sub item set.
					// (NB: the iterator processing is annoying and tedious.)
					for (auto item_it = s.begin(); item_it != s.end();)
					{
						const Item temp_item = *item_it;
						item_it = s.erase(item_it);
						if (prev_sets.find(s) == prev_sets.end()) // no match subset
						{
							subset_ok = false;
							s.insert(temp_item);
							break;
						}
						else
						{
							item_it = s.insert(temp_item).first;
							++item_it;
						}
					}

					if (!subset_ok)
					{
						s.erase(item);
						continue;
					}

					// it's maybe ok, just insert.
					cur_sets[s] = 0;
					is_extensible = true;
					s.erase(item);

				}

				// optimization for ALL_SETS_GREATER_THAN_SUPPORT_AND_NONEXTENSIBLE
				if (!is_extensible)
				{
					extensible_prune.insert(set);
					return_sets.push_back(std::make_tuple(set, count));
				}

			}

#ifdef DEBUG
			AP_output << "round: " << round << "\tjoin finished" << std::endl;
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
#endif // GET_TIME_ON_WINDOWS
			AP_output << "round: " << round << "\tfinished" << std::endl;
			AP_output << std::endl << "---------------------------------------------------------------------"
				<< std::endl << std::endl;
#ifdef OUTPUT_FILE // always output in console
			std::cout << "round: " << round << "\tjoin finished" << std::endl;
			std::cout << "round: " << round << "\tfinished" << std::endl;
			std::cout << std::endl << "---------------------------------------------------------------------"
				<< std::endl << std::endl;
#endif // OUTPUT_FILE
#endif // DEBUG

#ifdef DEBUG
#ifdef OUTPUT_FILE
			AP_output.close();
#endif // OUTPUT_FILE
#endif // DEBUG

		} // end while

	} // end function Apriori();


	/*
	 * Frequent Pattern Tree
	 *
	 * Requirements:
	 *     Set:
	 *          - `count()`
	 *          - `insert()`
	 *          - `erase()`
	 *          - Set::value_type `==`
	 *          - `<`    // comparable or hashable  ???  only choose one
	 *          - Item must be comparable !!!  // or hashble ???
	 */
#undef max
	template<
		typename Item,
		std::enable_if_t<
		std::is_same_v<bool, decltype(std::declval<Item>() < std::declval<Item>())>
		>* = nullptr
		> std::tuple<std::vector<std::tuple<std::set<Item>, std::size_t>>, std::map<Item, std::size_t>, std::map<std::size_t, Item>>
		Apriori_FP(std::vector<std::set<Item>>& sets, const std::size_t min_support)
	{
		using Set = std::set<Item>;

		// preprocess to accelerate index
		std::map<Item, std::size_t> item_amount;
		std::map<Item, std::size_t> item2int;
		std::map<std::size_t, Item> int2item;

		for (Set const& set : sets)
			for (Item const& item : set)
				item_amount[item]++;

		// ordered item-list
		std::vector<std::pair<Item, std::size_t>> item_list; // store as sequence
		std::set<Item> discard_items; // discard
		item_list.reserve(item_amount.size());
		for (auto const&[item, amount] : item_amount)
		{
			if (amount >= min_support) item_list.push_back(std::make_pair(item, amount));
			else discard_items.insert(item);
		}
		std::sort(item_list.begin(), item_list.end(),
			[](const std::pair<Item, std::size_t>&lhs, const std::pair<Item, std::size_t>& rhs) { return lhs.second > rhs.second; });

		std::size_t no_max = std::numeric_limits<std::size_t>::max();
		for (auto const&[item, amount] : item_list)
			item2int[item] = no_max--;
		for (auto const&[item, order] : item2int) int2item[order] = item;

		std::map<std::set<std::size_t>, std::size_t> _return_sets; // _return_sets

		// filter sets and construct FPTree
		using namespace RSY_TOOL::FPT;

		std::vector<std::size_t> _item_list;
		_item_list.reserve(item_list.size());
		for (auto const&[item, amount] : item_list)
			_item_list.push_back(item2int[item]);

		FPT<std::size_t> fp(
			std::move(_item_list),
			min_support,
			_return_sets
		);

		for (Set const& set : sets)
		{
			// filter
			Set s = set;
			for (Item const& discard_item : discard_items)
				s.erase(discard_item);

			// order
			std::vector<std::size_t> items;
			items.reserve(s.size());
			for (Item i : s) items.push_back(item2int[i]);
			std::sort(items.begin(), items.end(), std::greater<std::size_t>());

			fp.insert(items);
		}

		fp.mine();

		std::vector<std::tuple<Set, std::size_t>> return_sets;
		return_sets.reserve(_return_sets.size());
		for (auto const&[set, amount] : _return_sets)
		{
			std::set<Item> s;
			for (std::size_t item_no : set)
				s.insert(int2item[item_no]);
			return_sets.push_back(std::make_tuple(s, amount));
		}

		return std::make_tuple(std::move(return_sets), std::move(item2int), std::move(int2item));

	} // end funciton Apriori_FP();


	// WTF, alias template declaration cannot be declared in the funciton block.
	template<class K, class V> using Hash = std::unordered_map<K, V>;
	/*
	 * Generate Assosiation Rules
	 * [A] -> [B] [s, c]
	 *     s = support(A U B)
	 *     c = s / support(A)
	 */
	std::vector<std::tuple<std::set<std::size_t>, std::set<std::size_t>, double, double>>
		generate_assosiation_rule(
			const std::vector<std::tuple<std::set<std::size_t>, std::size_t>>& frequent_sets,
			const std::size_t total_transaction,
			const double confidence)
	{
		using Set = std::set<std::size_t>;
		using Item = std::size_t;

		/* BloomFilter
		 *     0. size
		 *     1. and     &
		 *     1. or      |        =>  and + or + xor
		 *     1. xor     ^
		 *     2. sum     +        =>  sum + mul
		 *     2. mul     *
		 */
		struct Filter
		{
			explicit Filter(const std::set<std::size_t>& s)
				:
				_and(std::accumulate(s.begin(), s.end(), 0xffffffff, [](std::size_t val, std::size_t item) { return val & item; })),
				_or(std::accumulate(s.begin(), s.end(), 0, [](std::size_t val, std::size_t item) { return val | item; })),
				_xor(std::accumulate(s.begin(), s.end(), 0, [](std::size_t val, std::size_t item) { return val ^ item; })),
				_sum(std::accumulate(s.begin(), s.end(), 0, [](std::size_t val, std::size_t item) { return val + item; })),
				_mul(std::accumulate(s.begin(), s.end(), 1, [](std::size_t val, std::size_t item) { return val * item; }))
			{}
			const std::size_t _and;
			const std::size_t _or;
			const std::size_t _xor;
			const std::size_t _sum;
			const std::size_t _mul;
		};
		class HashSet
		{
			using and_or_xor_sum_t = std::size_t;
			using sum_mul_sumt = std::size_t;
		public:
			explicit HashSet() = default;
			void insert(const std::set<std::size_t>& set, const std::size_t support)
			{
				const Filter f{ set };
				_hash[set.size()][f._and + f._or + f._xor][f._sum + f._mul].insert(std::make_pair(set, support));
			}
			std::size_t find(const std::set<std::size_t>& set) const
			{
				const Filter f{ set };
				try {
					const std::map<Set, const std::size_t>& sets =
						_hash.at(set.size()).at(f._and + f._or + f._xor).at(f._sum + f._mul);
					if (auto it = sets.find(set); it != sets.end())
						return it->second;
				}
				catch (std::out_of_range& exception) {}
				return 0;
			}
		private:
			Hash<std::size_t,
				Hash<and_or_xor_sum_t,
				Hash<sum_mul_sumt,
				std::map<Set, const std::size_t>>>>
				_hash;
		};

		/*                   [[Deprecated]]
		auto Is_intersect_null = [](const Set& A, const Set& B)->bool
		{
			auto ait = A.begin();
			auto bit = B.begin();
			while (ait != A.end() && bit != B.end())
			{
				if (*ait == *bit) return false;
				else if (*ait < *bit) ++ait;
				else if (*ait > *bit) ++bit;
			}
			return true;
		};
		auto Union = [](const Set& A, const Set& B) ->Set
		{
			Set s{};
			std::set_union(A.begin(), A.end(),
				B.begin(), B.end(),
				std::inserter(s, s.begin()));
			return s;
		};
		*/

		HashSet _hashSet;

		for (auto const&[set, support] : frequent_sets)
			_hashSet.insert(set, support);

		std::vector<std::tuple<std::set<std::size_t>, std::set<std::size_t>, double, double>> assosiation_rules;

		/*
		 * slice the set into 2 separated small sets.
		 * total = 2^(N-1) - 1
		 * compute [A -> B] or [B -> A]
		 */
		using RSY_TOOL::FPT::util::bigInt;
		for (auto const&[AB, support_AB] : frequent_sets)
		{
			const std::size_t size = AB.size();
			if (size == 1) continue;

			std::vector<std::size_t> _AB;
			_AB.reserve(size);
			for (std::size_t i : AB) _AB.push_back(i);

			bigInt it(1, size);
			const bigInt _max(size, size);
			for (; it != _max; ++it)
			{
				std::vector<std::size_t> numbers = it.combination_nums();
				std::set<std::size_t> A, B;
				std::size_t _cur = 0;
				auto num_it = numbers.begin();
				for (; _cur < size && num_it != numbers.end(); _cur++)
				{
					if (_cur == *num_it)
					{
						A.insert(_AB[_cur]);
						++num_it;
					}
					else B.insert(_AB[_cur]);
				}
				for (; _cur < size; _cur++) B.insert(_AB[_cur]);

				// compute confidence
				const std::size_t support_A = _hashSet.find(A);
				const std::size_t support_B = _hashSet.find(B);
				const double confidence_B = 1.0 * support_AB / support_A;
				const double confidence_A = 1.0 * support_AB / support_B;
				if (confidence_B >= confidence) // [A => B]
					assosiation_rules.push_back(std::make_tuple(A, B, 1.0 * support_AB / total_transaction, confidence_B));
				if (confidence_A >= confidence) // [B => A]
					assosiation_rules.push_back(std::make_tuple(B, A, 1.0 * support_AB / total_transaction, confidence_A));
			}
		}

		return assosiation_rules;

	} // end function generate_assosiation_rule();


} // end namespace BI_Apriori

#endif // !_APRIORI_H
