#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include "Apriori.h"
#ifdef DEBUG
#ifdef GET_TIME_ON_WINDOWS
#include <Windows.h>
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG


#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

template<class _Ty> class SumPowers {
public:
	SumPowers(int _power = 1) :power(_power) {}
	const _Ty operator()(const _Ty& total, const _Ty& value) {
		_Ty _value = value;
		for (int i = 0; i < power - 1; ++i)
			_value *= value;
		return total + _value;
	}
private:
	const int power;
};

void test()
{
	double miu = 80;
	std::vector<double> data = { 84,82,96,78,84,90,86,79,88 };

	double X_bar = std::accumulate(data.begin(), data.end(), 0.0, SumPowers<double>(1)) / data.size();
	double Sample_variance = std::accumulate(data.begin(), data.end(),
		-1.0*data.size()*X_bar*X_bar, SumPowers<double>(2)) / (data.size() - 1);
	double Standard_deviation_of_samples = sqrt(Sample_variance);
	//res--t(n-1)
	//double res = 1.0*(X_bar - miu)*sqrt(data.size()) / Standard_deviation_of_samples;
}


void test_Apriori()
{
	constexpr std::size_t item_amount = 100;
	constexpr std::size_t transaction = 5000;
	constexpr std::size_t item_each_transaction = 45;
	constexpr double magic_coefficient = 0.20;
	constexpr double min_support_ratio = 0.3;
	constexpr std::size_t min_support =
		false
		? static_cast<std::size_t>(transaction / item_amount * item_each_transaction * magic_coefficient)
		: static_cast<std::size_t>(min_support_ratio * transaction);

	constexpr double normal_mean = item_amount / 2.0;
	constexpr double normal_deviation = item_amount / 4.0;

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

	// output data
	const char* data_path = "./AP_data.txt";
	std::ofstream AP_data_output;
	AP_data_output.open(data_path, std::ios::out | std::ios::trunc);
	if (!AP_data_output.is_open())
	{
		std::cout << "failed to open AP_data.txt" << std::endl;
		exit(0);
	}
	std::size_t data_line = 0;
	for (auto const& data : s)
	{
		AP_data_output << ++data_line << "  (" << data.size() << ")\t:\t";
		std::copy(data.begin(), data.end(), std::ostream_iterator<std::size_t>(AP_data_output, " "));
		AP_data_output << std::endl;
	}
	AP_data_output.close();


#ifdef DEBUG
#ifdef OUTPUT_FILE
	const char* output_path = "./AP.txt";
	std::ofstream AP_output;
	AP_output.open(output_path, std::ios::out | std::ios::app);
	if (!AP_output.is_open())
	{
		std::cout << "failed to open AP.txt" << std::endl;
		exit(0);
	}
#else
	auto& AP_output = std::cout;
#endif // OUTPUT_FILE
#endif // DEBUG

	AP_output << "data generated" << std::endl << std::endl;

#ifdef DEBUG
#ifdef GET_TIME_ON_WINDOWS
	DWORD  start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

	auto result = BI_Apriori::Apriori<std::set<std::size_t>>(s, min_support);

#ifdef DEBUG
#ifdef GET_TIME_ON_WINDOWS
	DWORD  end = GetTickCount();
	BI_Apriori::print_time(start, end, AP_output, "Apriori time used: ");
#endif // GET_TIME_ON_WINDOWS
#endif // DEBUG

	AP_output << std::endl << "result size:" << result.size() << std::endl;
	std::size_t result_line = 0;
	for (auto const&[set, count] : result)
	{
		AP_output << ++result_line << "\t" << count << "\t:";
		for (auto const& item : set)
			AP_output << item << " ";
		AP_output << std::endl;
	}
	AP_output << "Apriori Completed" << std::endl;
#ifdef OUTPUT_FILE
	AP_output.close();
#endif // OUTPUT_FILE
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
	RSY_TOOL::FPT::FPT<std::string> _fpt;
	std::vector<std::vector<std::string>> str_container;

	std::vector<std::string> ss;
	ss = { "milk", "bread", "butter", "cheese","jam" }; str_container.push_back(ss);
	ss = { "milk", "bread", "butter", "jam" }; str_container.push_back(ss);
	ss = { "milk", "butter" }; str_container.push_back(ss);
	ss = { "milk", "cheese" }; str_container.push_back(ss);
	ss = { "bread", "butter", "cheese" }; str_container.push_back(ss);
	ss = { "bread", "jam", }; str_container.push_back(ss);

	for (auto const& strs : str_container)
		_fpt.insert(strs);


}


int main(void)
{
	test_Apriori();
	//test_Apriori_FPT();
	printf("%s\n", "done");
	getchar();
	return 0;
}
