#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include <functional>
#include "Apriori.h"
#include "dissimilarity.hpp"
#ifdef DEBUG
#ifdef GET_TIME_ON_WINDOWS
#include <Windows.h>
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG


#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

constexpr std::size_t item_amount = 100;
constexpr std::size_t transaction = 10000;
constexpr std::size_t item_each_transaction = 40; // less than half of `item_amount`
constexpr double min_support_ratio = 0.350;
constexpr std::size_t min_support = static_cast<std::size_t>(min_support_ratio * transaction);
constexpr double confidence = 0.8;


constexpr double normal_mean = item_amount / 2.0;
constexpr double normal_deviation = item_amount / 4.5;

constexpr double support_increment = 0;
constexpr double confidence_increment = 0.0;

constexpr std::size_t qinding_amount = 4 + item_amount / 4 - item_each_transaction / 2;


std::vector<std::set<std::size_t>> generate()
{
	srand(static_cast<std::size_t>(time(nullptr)));
	std::random_device rd1{};
	std::mt19937 gen{ rd1() };

	std::normal_distribution<> d{ normal_mean, normal_deviation };

	// shuffle the order ID, looks more natural
	std::size_t shuffle_map[item_amount];
	for (std::size_t i = 0; i < item_amount; i++) shuffle_map[i] = i + 1;
	std::random_device rd2;
	std::mt19937 g(rd2());
	std::shuffle(std::begin(shuffle_map), std::end(shuffle_map), g);


	std::multimap<std::size_t, std::size_t> QinDing;
	for (std::size_t i = 0; i < item_amount * qinding_amount; i++)
	{
		const std::size_t num1 = 1 + std::rand() % item_amount;
		const std::size_t num2 = 1 + std::rand() % item_amount;
		QinDing.insert(std::make_pair(num1, num2));
		QinDing.insert(std::make_pair(num2, num1));
	}


	std::vector<std::set<std::size_t>> s; s.reserve(transaction);
	std::set<std::size_t> set;
	for (std::size_t i = 0; i < transaction; i++)
	{
		const std::size_t transaction_item = (((std::rand() % item_each_transaction) << 1) + 1) % item_amount;
		while (set.size() < transaction_item)
		{
			const std::size_t insert_num = shuffle_map[static_cast<std::size_t>(std::rand() % item_amount)];
			//const std::size_t insert_num = shuffle_map[(static_cast<std::size_t>(d(gen))) % item_amount];
			set.insert(insert_num);
			if (QinDing.find(insert_num) != QinDing.end())
			{
				const std::size_t proportion = std::rand() % 1000;
				if (proportion % 1 != 0) continue;
				if (proportion < confidence * 1000 * 1.2 // filter
					|| transaction_item < item_each_transaction / 2.5
					|| true)
				{
					auto const&[first, end] = QinDing.equal_range(insert_num);
					for (auto it = first; it != end; ++it)
						set.insert(it->second);
				}
			}
		}
		s.push_back(set);
		set.clear();
	}
	return s;
}

static std::size_t test_round = -1;
std::vector<std::set<std::size_t>> get_data()
{
	static std::vector<std::set<std::size_t>> s = generate();
	test_round++;
	return s;
}


template<
	typename outputItem,
	std::enable_if_t<
	std::is_same_v<std::ostream&,
	decltype(std::declval<std::ostream>() << std::declval<outputItem>())>
	>* = nullptr
> void output_data(std::ostream& data_output, const std::vector<std::set<outputItem>>& _data)
{
	data_output << "line\tsize\t\ttransaction item" << std::endl;
	std::size_t data_line = 0;
	std::ostream_iterator<std::size_t> _data_out_it(data_output, " ");
	for (auto const& data : _data)
	{
		data_output << ++data_line << "\t" << data.size() << "\t:\t";
		std::copy(data.begin(), data.end(), _data_out_it);
		data_output << std::endl;
	}
}

template<typename Item>
void output_assosiation_rules(
	std::vector<std::tuple<std::set<std::size_t>, std::set<std::size_t>, double, double>>&& assosiation_rules,
	const std::string& output_path,
	const std::function<Item(std::size_t)>& int2item)
{
	using Set = std::set<std::size_t>;
	std::ofstream Assosiation_Rules_out;
	Assosiation_Rules_out.open(output_path, std::ios::out | std::ios::trunc);
	if (!Assosiation_Rules_out.is_open())
	{
		std::cout << "failed to open Assosiation_Rules.txt" << std::endl;
		exit(0);
	}
	auto print_Set = [&Assosiation_Rules_out, &int2item](const Set& s)
	{
		Assosiation_Rules_out << "[";
		const std::size_t size = s.size();
		const auto back_it = --s.end();
		for (auto it = s.begin(); it != back_it; ++it)
			Assosiation_Rules_out << int2item(*it) << ", ";
		Assosiation_Rules_out << int2item(*back_it);
		Assosiation_Rules_out << "]";
	};

	Assosiation_Rules_out << "No\t:\t[A]\t->\t[B]\t[s, c]" << std::endl;
	Assosiation_Rules_out << "-----------------------------------------------------------------------------" << std::endl;
	std::size_t line_num = 0;
	for (auto const&[A, B, support_AB, confidence_AB] : assosiation_rules)
	{
		Assosiation_Rules_out << ++line_num << "\t:\t";
		print_Set(A);
		Assosiation_Rules_out << "  ->  ";
		print_Set(B);
		Assosiation_Rules_out << "  ";
		Assosiation_Rules_out << "[" << std::fixed << std::setprecision(4) << support_AB << ", " << std::fixed << std::setprecision(4) << confidence_AB << "]";
		Assosiation_Rules_out << std::endl;
	}
	Assosiation_Rules_out.close();
}


void test_Apriori()
{

	std::vector<std::set<std::size_t>> s = get_data();

	// output data
	if (test_round == 0)
	{
		const char* data_path = "./AP_data.txt";
		std::ofstream AP_data_output;
		AP_data_output.open(data_path, std::ios::out | std::ios::trunc);
		if (!AP_data_output.is_open())
		{
			std::cout << "failed to open AP_data.txt" << std::endl;
			exit(0);
		}
		output_data<std::size_t>(AP_data_output, s);
		AP_data_output.close();
	}

#ifdef OUTPUT_FILE
	const std::string output_path = "./AP_" + std::to_string(test_round) + ".txt";
	std::ofstream AP_output;
	AP_output.open(output_path, std::ios::out | std::ios::trunc);
	if (!AP_output.is_open())
	{
		std::cout << "failed to open AP.txt" << std::endl;
		exit(0);
	}
#else
	auto& AP_output = std::cout;
#endif // OUTPUT_FILE

	const double _min_support_ratio = (min_support_ratio + test_round * support_increment);
	const std::size_t _min_support = static_cast<std::size_t>(_min_support_ratio * transaction);
	const double _confidence = confidence + test_round * confidence_increment;

#ifdef OUTPUT_FILE // always output in console
	std::cout << "item_amount: " << item_amount << std::endl;
	std::cout << "transaction: " << transaction << std::endl;
	std::cout << "item_each_transaction: " << item_each_transaction << std::endl;
	std::cout << "min_support_ratio: " << _min_support_ratio << std::endl;
	std::cout << "min_support: " << _min_support << std::endl;
	std::cout << "confidence: " << _confidence << std::endl;
	std::cout << "data generated: normal distribution N("
		<< normal_mean << ", " << normal_deviation << ")"
		<< std::endl << std::endl;
	std::cout << std::endl;
#endif // OUTPUT_FILE


	AP_output << "item_amount: " << item_amount << std::endl;
	AP_output << "transaction: " << transaction << std::endl;
	AP_output << "item_each_transaction: " << item_each_transaction << std::endl;
	AP_output << "min_support_ratio: " << _min_support_ratio << std::endl;
	AP_output << "min_support: " << _min_support << std::endl;
	AP_output << "confidence: " << _confidence << std::endl;
	AP_output << "data generated: normal distribution N("
		<< normal_mean << ", " << normal_deviation << ")"
		<< std::endl << std::endl;
	AP_output << std::endl;

#ifdef OUTPUT_FILE
	AP_output.close();
#endif // OUTPUT_FILE

#ifdef GET_TIME_ON_WINDOWS
	DWORD  start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS

	auto result = BI_Apriori::Apriori<std::set<std::size_t>>(s, _min_support, output_path);

#ifdef OUTPUT_FILE
	AP_output.open(output_path, std::ios::out | std::ios::app);
	if (!AP_output.is_open())
	{
		std::cout << "failed to open AP.txt" << std::endl;
		exit(0);
	}
#else
	auto& AP_output = std::cout;
#endif // OUTPUT_FILE
#ifdef GET_TIME_ON_WINDOWS
	DWORD  end = GetTickCount();
	BI_Apriori::print_time(start, end, AP_output, "Apriori time used: ");
	BI_Apriori::print_time(start, end, std::cout, "Apriori time used: ");
#endif // GET_TIME_ON_WINDOWS

	AP_output << std::endl << "result size:" << result.size() << std::endl;
	std::size_t result_line = 0;
	AP_output << "count\tsupport\t\titem set" << std::endl;
	AP_output << "---------------------------------------------------------------------" << std::endl;
	for (auto const&[set, count] : result)
	{
		AP_output << ++result_line << "\t" << count << "\t:\t";
		for (auto const& item : set)
			AP_output << item << " ";
		AP_output << std::endl;
	}
	AP_output << "Apriori Completed" << std::endl;
#ifdef OUTPUT_FILE
	AP_output.close();
#endif // OUTPUT_FILE

	// Generate Assosiation Rules
	std::vector<std::tuple<std::set<std::size_t>, std::set<std::size_t>, double, double>>
		assosiation_rules = BI_Apriori::generate_assosiation_rule(result, transaction, confidence);

	// output Assosiation Rules
	std::string assosiation_rules_output_path = std::string("./Assosiation_Rules_AP_") + std::to_string(test_round) + ".txt";
	output_assosiation_rules<std::size_t>(std::move(assosiation_rules), assosiation_rules_output_path, [](std::size_t i) { return i; });

}


void test_Apriori_FPT()
{
	/*
	std::vector<std::set<int>> s;
	s.push_back(std::set<int>{ 1, 3, 4 });
	s.push_back(std::set<int>{ 2, 3, 5 });
	s.push_back(std::set<int>{ 1, 2, 3, 5 });
	s.push_back(std::set<int>{ 2, 5 });
	auto result = BI_Apriori::Apriori<std::set<int>>(s, 2);
	for (auto const& set : result)
	{
	for (auto const& item : set)
	std::cout << item << " "; // 2 3 5
	std::cout << std::endl;
	}

	std::cout << "\n\n";
	s.clear();

	s.push_back(std::set<int>{ 1, 2, 3, 4 });
	s.push_back(std::set<int>{ 1, 2 });
	s.push_back(std::set<int>{ 2, 3, 4 });
	s.push_back(std::set<int>{ 2, 3 });
	s.push_back(std::set<int>{ 1, 2, 4 });
	s.push_back(std::set<int>{ 3, 4 });
	s.push_back(std::set<int>{ 2, 4 });
	result = BI_Apriori::Apriori<std::set<int>>(s, 3);
	for (auto const& set : result)
	{
	for (auto const& item : set)
	std::cout << item << " "; // 1 2; 2 3; 2 4; 3 4
	std::cout << std::endl;
	}
	*/


#define TEST_SIMPLEx //
#ifdef TEST_SIMPLE
	std::vector<std::set<std::string>> s;
	std::set<std::string> ss;
	ss = { "milk", "bread", "butter", "cheese","jam" }; s.push_back(ss);
	ss = { "milk", "bread", "butter", "jam" }; s.push_back(ss);
	ss = { "milk", "butter" }; s.push_back(ss);
	ss = { "milk", "cheese" }; s.push_back(ss);
	ss = { "bread", "butter", "cheese" }; s.push_back(ss);
	ss = { "bread", "jam" }; s.push_back(ss);
	ss = { "milk", "discard" }; s.push_back(ss);
	using Item = std::string;
	constexpr std::size_t _min_support = 2;
	constexpr std::size_t _transaction = 7;
	constexpr std::size_t _confidence = 0.2;
#else
	std::vector<std::set<std::size_t>> s = get_data();
	constexpr std::size_t FP_min_support = min_support;
	constexpr std::size_t _transaction = transaction;

	// output data
	const std::string output_data_path = "./FP_data.txt";
	std::ofstream FP_output_data;
	FP_output_data.open(output_data_path, std::ios::out | std::ios::trunc);
	if (!FP_output_data.is_open())
	{
		std::cout << "failed to open FP.txt" << std::endl;
		exit(0);
	}
	using Item = std::size_t;
	output_data<Item>(FP_output_data, s);
	FP_output_data.close();


	const std::string output_path = std::string("./FP_") + std::to_string(test_round) + ".txt";
	std::ofstream FP_output;
	FP_output.open(output_path, std::ios::out | std::ios::trunc);
	if (!FP_output.is_open())
	{
		std::cout << "failed to open FP.txt" << std::endl;
		exit(0);
	}

	const double _min_support_ratio = (min_support_ratio + test_round * support_increment);
	const std::size_t _min_support = static_cast<std::size_t>(_min_support_ratio * transaction);
	const double _confidence = confidence + test_round * confidence_increment;

	FP_output << "item_amount: " << item_amount << std::endl;
	FP_output << "transaction: " << transaction << std::endl;
	FP_output << "item_each_transaction: " << item_each_transaction << std::endl;
	FP_output << "min_support_ratio: " << _min_support_ratio << std::endl;
	FP_output << "min_support: " << _min_support << std::endl;
	FP_output << "confidence: " << _confidence << std::endl;
	FP_output << "data generated: normal distribution N("
		<< normal_mean << ", " << normal_deviation << ")"
		<< std::endl << std::endl;
	FP_output << std::endl;

	std::cout << "item_amount: " << item_amount << std::endl;
	std::cout << "transaction: " << transaction << std::endl;
	std::cout << "item_each_transaction: " << item_each_transaction << std::endl;
	std::cout << "min_support_ratio: " << _min_support_ratio << std::endl;
	std::cout << "min_support: " << _min_support << std::endl;
	std::cout << "confidence: " << _confidence << std::endl;
	std::cout << "data generated: normal distribution N("
		<< normal_mean << ", " << normal_deviation << ")"
		<< std::endl << std::endl;
	std::cout << std::endl;


#endif // TEST_SIMPLE
#ifdef GET_TIME_ON_WINDOWS
	DWORD start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS

	std::tuple<
		std::vector<std::tuple<std::set<Item>, std::size_t>>,
		std::map<Item, std::size_t>,
		std::map<std::size_t, Item>
	> _result = BI_Apriori::Apriori_FP<Item>(s, _min_support);

	std::map<Item, std::size_t>& item2int = std::get<1>(_result);
	std::map<std::size_t, Item>& int2item = std::get<2>(_result);

	std::vector<std::tuple<std::set<std::size_t>, std::size_t>> item_sets;
	item_sets.reserve(std::get<0>(_result).size());
	for (auto const&[set, amount] : std::get<0>(_result))
	{
		std::set<std::size_t> items;
		for (const Item& i : set)
			items.insert(item2int[i]);
		item_sets.push_back(std::make_tuple(std::move(items), amount));
	}

	std::tuple<
		std::vector<std::tuple<std::set<std::size_t>, std::size_t>>,
		std::map<Item, std::size_t>,
		std::map<std::size_t, Item>
	> result = std::make_tuple(std::move(item_sets), item2int, int2item);

#ifdef GET_TIME_ON_WINDOWS
	DWORD end = GetTickCount();
	BI_Apriori::print_time(start, end, FP_output, "FP completed: ");
#endif // GET_TIME_ON_WINDOWS


	// output item-set
	FP_output << "count\tsupport\t\titem set" << std::endl;
	FP_output << "---------------------------------------------------------------------" << std::endl;
	std::size_t count = 0;
	for (auto const&[set, amount] : std::get<0>(_result))
	{
		FP_output << ++count << "\t" << amount << "\t:\t";
		std::vector<Item> temp_set_for_output;
		temp_set_for_output.reserve(set.size());
		for (Item const& s : set)
			temp_set_for_output.push_back(s);
		std::sort(temp_set_for_output.begin(), temp_set_for_output.end(),
			[&item2int](const Item& lhs, const Item& rhs) { return item2int[lhs] > item2int[rhs]; });
		for (Item const& s : temp_set_for_output)
			FP_output << s << " ";
		FP_output << std::endl;
	}
	FP_output.close();

	// Generate Assosiation Rules
	std::vector<std::tuple<std::set<std::size_t>, std::set<std::size_t>, double, double>>
		assosiation_rules = BI_Apriori::generate_assosiation_rule(std::get<0>(result), _transaction, _confidence);

	// output Assosiation Rules
	const std::string assosiation_rules_output_path = std::string("./Assosiation_Rules_FP") + std::to_string(test_round) + ".txt";
	output_assosiation_rules<Item>(std::move(assosiation_rules), assosiation_rules_output_path, [&int2item](std::size_t i) { return int2item[i]; });

}


int main()
{
	//BI_dissimilarity::test();
	//for (std::size_t i = 0; i < 9; i++)
	test_Apriori();
	test_Apriori_FPT();
	//printf("\n%s\n", "done");
	//getchar();
	return 0;
}
