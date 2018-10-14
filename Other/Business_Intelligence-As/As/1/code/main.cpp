#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include "Apriori.h"
#ifdef GET_TIME_ON_WINDOWS
#include <Windows.h>
#endif // GET_TIME_ON_WINDOWS


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
	constexpr std::size_t item_amount = 50;
	constexpr std::size_t transaction = 1500;
	constexpr std::size_t item_each_transaction = 25;
	constexpr double magic_coefficient = 0.2;
	constexpr std::size_t min_support = static_cast<std::size_t>(transaction / item_amount * item_each_transaction * magic_coefficient);

	std::vector<std::set<std::size_t>> s; s.reserve(transaction);
	std::set<std::size_t> set;
	srand(time(NULL));
	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::normal_distribution<> d{ item_amount / 2.0, item_amount / 4.0 };

	for (std::size_t i = 0; i < transaction; i++)
	{
		const std::size_t transaction_item = (std::rand() % item_each_transaction) + 1;
		while (set.size() < transaction_item)
			//set.insert(std::rand() % item_amount + 1);
			set.insert(1 + (static_cast<std::size_t>(d(gen))) % item_amount);
		s.push_back(set);
		set.clear();
	}

	std::cout << "data generated" << std::endl << std::endl;

#ifdef GET_TIME_ON_WINDOWS
	DWORD  start = GetTickCount();
#endif // GET_TIME_ON_WINDOWS

	auto result = BI_Apriori::Apriori<std::set<std::size_t>>(s, min_support);

#ifdef GET_TIME_ON_WINDOWS
	DWORD  end = GetTickCount();
	std::size_t total_s = (end - start) / 1000;
	std::size_t minutes, seconds;
	minutes = total_s / 60;
	seconds = total_s - minutes * 60;
	std::cout << "Apriori time used: " << minutes << "min " << seconds << "s" << std::endl << std::endl;
#endif // GET_TIME_ON_WINDOWS

	std::cout << std::endl << "result size:" << result.size() << std::endl;
	for (auto const& set : result)
	{
		for (auto const& item : set)
			std::cout << item << " ";
		std::cout << std::endl;
	}
	std::cout << "Apriori Completed" << std::endl;
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
	getchar();
	return 0;
}
