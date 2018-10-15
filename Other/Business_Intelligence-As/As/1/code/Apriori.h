#pragma once
#ifndef _APRIORI_H
#define _APRIORI_H
#include <vector>
#include <type_traits>
#include <iterator>
#include <tuple>
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
#include <algorithm>
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
	 *     ALL_SETS_GREATER_THAN_SUPPORT
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
		std::set<Set> extensible_prune; // optimization for ALL_SETS_GREATER_THAN_SUPPORT
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
		AP_output.open(search_path, std::ios::out | std::ios::app);
		if (!AP_output.is_open())
		{
			std::cout << "failed to open AP.txt" << std::endl;
			exit(0);
		}
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
			round++;
			AP_output << "round: " << round << "\treturn_sets:" << return_sets.size() << std::endl;
			AP_output << "round: " << round << "\tprev_sets:" << prev_sets.size() << std::endl;
			AP_output << "round: " << round << "\tcur_sets:" << cur_sets.size() << std::endl;
			AP_output << std::endl;
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
			AP_output << "round: " << round << "\tcur_sets:" << cur_sets.size() << std::endl;
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

#ifdef ALL_SETS_GREATER_THAN_SUPPORT
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
#endif // ALL_SETS_GREATER_THAN_SUPPORT

#ifdef DEBUG
			AP_output << "round: " << round << "\tadd prev_sets to return_sets" << std::endl;
			AP_output << "round: " << round << "\treturn_sets:" << return_sets.size() << std::endl;
#ifdef GET_TIME_ON_WINDOWS
			end = GetTickCount();
			print_time(start, end, AP_output, "");
			start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

			// if cur_sets is empty
			if (cur_sets.empty())
			{
#ifndef ALL_SETS_GREATER_THAN_SUPPORT
				return_sets.reserve(return_sets.size() + prev_sets.size() - extensible_prune.size());
				for (auto const&[set, count] : prev_sets)
					if (extensible_prune.find(set) != extensible_prune.end())
						return_sets.push_back(set);
#endif // !ALL_SETS_GREATER_THAN_SUPPORT
#ifdef DEBUG
				AP_output << "round: " << round << "\tcur_sets is empty." << std::endl << std::endl;
#endif // DEBUG
				return return_sets;
			}

			// if cur_sets has 1 item set
			if (cur_sets.size() == 1)
			{
#ifdef DEBUG
				AP_output << "round: " << round << "\tcur_sets has only 1 item set." << std::endl << std::endl;
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

				// optimization for ALL_SETS_GREATER_THAN_SUPPORT
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
#ifdef OUTPUT_FILE
			AP_output.close();
#endif // OUTPUT_FILE
#endif // DEBUG

		} // end while

	} // end function Apriori();


	/*
	 * Frequent Pattern Tree
	 *
	 *
	 */
	template<
		typename Set,
		std::void_t<typename Set::value_type>* = nullptr,
		std::enable_if_t<
		std::is_same_v<std::size_t, decltype(std::declval<Set>().count(std::declval<typename Set::value_type>()))>
		&& std::is_same_v<bool, decltype(std::declval<typename Set::value_type>() == std::declval<typename Set::value_type>())>
		&& std::is_same_v<bool, decltype(std::declval<Set>() < std::declval<Set>())>
		&& std::is_same_v<std::pair<typename Set::iterator, bool>, decltype(std::declval<Set>().insert(std::declval<typename Set::value_type>()))>
		&& std::is_same_v<typename Set::iterator, decltype(std::declval<Set>().erase(std::declval<typename Set::iterator>()))>
		>* = nullptr
		> std::vector<Set> Apriori_FP(std::vector<Set>& sets, const std::size_t min_support)
	{


		return std::vector<Set>{};

	} // end funciton Apriori_FP();


} // end namespace BI_Apriori

#endif // !_APRIORI_H