#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
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

constexpr std::size_t item_amount = 40;
constexpr std::size_t transaction = 2000;
constexpr std::size_t item_each_transaction = 16;
constexpr double magic_coefficient = 0.20;
constexpr double min_support_ratio = 0.25;
constexpr double confidence = 0.8;
constexpr bool difficult = false;
constexpr std::size_t min_support =
difficult
? static_cast<std::size_t>(transaction * item_each_transaction * magic_coefficient / item_amount)
	: static_cast<std::size_t>(min_support_ratio * transaction);
constexpr double normal_mean = item_amount / 2.0;
constexpr double normal_deviation = item_amount / 4.0;

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

	std::vector<std::set<std::size_t>> s; s.reserve(transaction);
	std::set<std::size_t> set;
	for (std::size_t i = 0; i < transaction; i++)
	{
		const std::size_t transaction_item = ((std::rand() % item_each_transaction) << 1) + 1;
		while (set.size() < transaction_item)
			//set.insert(shuffle_map[std::rand() % item_amount]);
			set.insert(shuffle_map[(static_cast<std::size_t>(d(gen))) % item_amount]);
		s.push_back(set);
		set.clear();
	}
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


void test_Apriori()
{

	std::vector<std::set<std::size_t>> s = generate();

	// output data
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


#ifdef OUTPUT_FILE
	const char* output_path = "./AP.txt";
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

#ifdef OUTPUT_FILE // always output in console
	std::cout << "item_amount: " << item_amount << std::endl;
	std::cout << "transaction: " << transaction << std::endl;
	std::cout << "item_each_transaction: " << item_each_transaction << std::endl;
	std::cout << "min_support_ratio: " << 1.0 * min_support / transaction << std::endl;
	std::cout << "min_support: " << min_support << std::endl;
	std::cout << "data generated" << std::endl << std::endl;
	std::cout << std::endl;
#endif // OUTPUT_FILE


	AP_output << "item_amount: " << item_amount << std::endl;
	AP_output << "transaction: " << transaction << std::endl;
	AP_output << "item_each_transaction: " << item_each_transaction << std::endl;
	AP_output << "min_support_ratio: " << 1.0 * min_support / transaction << std::endl;
	AP_output << "min_support: " << min_support << std::endl;
	AP_output << "data generated" << std::endl << std::endl;
	AP_output << std::endl;

#ifdef OUTPUT_FILE
	AP_output.close();
#endif // OUTPUT_FILE

#ifdef GET_TIME_ON_WINDOWS
	DWORD  start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS

	auto result = BI_Apriori::Apriori<std::set<std::size_t>>(s, min_support);

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
#endif // GET_TIME_ON_WINDOWS

	AP_output << std::endl << "result size:" << result.size() << std::endl;
	std::size_t result_line = 0;
	AP_output << "count\tsupport\t\titem set" << std::endl;
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
	BI_Apriori::generate_assosiation_rule(result, transaction, confidence);

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

	// test FPT

	const char* output_path = "./FP.txt";
	std::ofstream FP_output;
	FP_output.open(output_path, std::ios::out | std::ios::trunc);
	if (!FP_output.is_open())
	{
		std::cout << "failed to open FP.txt" << std::endl;
		exit(0);
	}

#define TEST_SIMPLEx //
#ifdef TEST_SIMPLE
	std::vector<std::set<std::string>> str_container;
	std::set<std::string> ss;
	ss = { "milk", "bread", "butter", "cheese","jam" }; str_container.push_back(ss);
	ss = { "milk", "bread", "butter", "jam" }; str_container.push_back(ss);
	ss = { "milk", "butter" }; str_container.push_back(ss);
	ss = { "milk", "cheese" }; str_container.push_back(ss);
	ss = { "bread", "butter", "cheese" }; str_container.push_back(ss);
	ss = { "bread", "jam" }; str_container.push_back(ss);
	ss = { "milk", "discard" }; str_container.push_back(ss);
	using Item = std::string;
	std::tuple<
		std::vector<std::tuple<std::set<Item>, std::size_t>>,
		std::map<Item, std::size_t>,
		std::map<std::size_t, Item>
	> result = BI_Apriori::Apriori_FP<Item>(str_container, 2);
#else
	std::vector<std::set<std::size_t>> s = generate();

	// output data
	const char* output_data_path = "./FP_data.txt";
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
	FP_output << "data generated" << std::endl;

#ifdef GET_TIME_ON_WINDOWS
	DWORD start = GetTickCount();

	std::tuple<
		std::vector<std::tuple<std::set<Item>, std::size_t>>,
		std::map<Item, std::size_t>,
		std::map<std::size_t, Item>
	> result = BI_Apriori::Apriori_FP<Item>(s, min_support);

	DWORD end = GetTickCount();
	BI_Apriori::print_time(start, end, FP_output, "FP completed: ");
#endif // GET_TIME_ON_WINDOWS
#endif // TEST_SIMPLE

	std::map<Item, std::size_t> item2int = std::move(std::get<1>(result));
	std::map<std::size_t, Item> in2item = std::move(std::get<2>(result));

	for (auto const&[set, amount] : std::get<0>(result))
	{
		FP_output << amount << "\t:\t";
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
}


int main()
{
	//BI_dissimilarity::test();
	test_Apriori();
	//test_Apriori_FPT();
	printf("\n%s\n", "done");
	getchar();
	return 0;
}
