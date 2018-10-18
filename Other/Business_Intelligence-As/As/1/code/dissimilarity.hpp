#pragma once
#ifndef _DISSIMILARITY_HPP
#define _DISSIMILARITY_HPP
#include <vector>
#include <variant>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <cassert>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace BI_dissimilarity
{
	namespace _impl
	{
		template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
		template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

		// Now is Fucking C++17, template lambda is supported until C++20
		struct other_t {
			std::vector<double>& _dissimilarity_vector;
			other_t(std::vector<double>& dissimilarity_vector) :
				_dissimilarity_vector(dissimilarity_vector) {}
			template<typename T> void operator()(T const& d1, T const& d2) const {
				_dissimilarity_vector.push_back((d1 == d2) ? 0.0 : 1.0);
			}
		};

		using numeric_t = std::tuple<const double>;
		using ordinal_t = std::tuple<const std::size_t>;
		using binary_t = std::tuple<const bool>;

		// maybe user customized for extensibility
		using item_t = std::variant<numeric_t, ordinal_t, binary_t, std::string>;


		/*
		 * Requirement:
		 *     the item matrixs must have regular size,
		 *     and type is the same for each column.
		 *
		 * Promise:
		 *     no coupling with item_t,
		 *     which can be customized by users.
		 */
		static std::vector<std::vector<double>> dissimilarity(const std::vector<std::vector<item_t>>& items)
		{
			const std::size_t size = items.size();
			std::vector<std::vector<double>> dissimilarity_matrix;
			if (size == 0) return dissimilarity_matrix;
			dissimilarity_matrix.resize(size);
			const std::size_t dimension = items[0].size();
			std::for_each(dissimilarity_matrix.begin(), dissimilarity_matrix.end(),
				[size](std::vector<double>& vec) { vec.resize(size); });

			using dimension_t = std::size_t;
			// (max - min)    // for numeric calculation
			std::unordered_map<dimension_t, numeric_t> range;
			for (std::size_t j = 0; j < dimension; j++)
				if (auto pval = std::get_if<numeric_t>(&items[0][j]))
					range.insert(std::make_pair(j, std::make_tuple<const double>(std::accumulate(
						items.begin(), items.end(), std::numeric_limits<double>::lowest(),
						[j](double& _max, std::vector<item_t> const& xi) { return max(_max, std::get<0>(std::get<numeric_t>(xi[j]))); })
						- std::accumulate(items.begin(), items.end(), std::numeric_limits<double>::infinity(),
							[j](double& _min, std::vector<item_t> const& xi) { return min(_min, std::get<0>(std::get<numeric_t>(xi[j]))); }))));

			// for ordinal calculation
			std::unordered_map<dimension_t, std::vector<numeric_t>> ordinals;
			for (std::size_t j = 0; j < dimension; j++)
				if (auto pval = std::get_if<ordinal_t>(&items[0][j]))
				{
					const std::size_t ordinal_max = std::accumulate(items.begin(), items.end(), 0u,
						[j](std::size_t& _max, std::vector<item_t> const& xi) { return max(_max, std::get<0>(std::get<ordinal_t>(xi[j]))); });
					ordinals[j].reserve(size);
					for (std::size_t i = 0; i < size; i++)
						ordinals[j].push_back(std::make_tuple<const double>(1.0 * (std::get<0>(std::get<ordinal_t>(items[i][j])) - 1) / (ordinal_max - 1)));
					range.insert(std::make_pair(j, std::make_tuple<const double>(std::accumulate(
						ordinals[j].begin(), ordinals[j].end(), std::numeric_limits<double>::lowest(),
						[](double& _max, numeric_t const& xif) { return max(_max, std::get<0>(xif)); })
						- std::accumulate(ordinals[j].begin(), ordinals[j].end(), std::numeric_limits<double>::infinity(),
							[](double& _min, numeric_t const& xif) { return min(_min, std::get<0>(xif)); }))));
				}

			// check binary column.
			std::unordered_set<dimension_t> bin_dimension;
			for (dimension_t k = 0; k < dimension; k++)
				if (auto pval = std::get_if<binary_t>(&items[0][k]))
					bin_dimension.insert(k);

			// calculate d[i][j] dissimilarity-vector
			for (std::size_t i = 0; i < size; i++)
				for (std::size_t j = 0; j < i; j++)
				{
					using data_pair_t = std::tuple<item_t, item_t>;
					std::vector<std::tuple<dimension_t, data_pair_t>> data_vec;
					data_vec.reserve(dimension);
					for (std::size_t k = 0; k < dimension; k++)
						data_vec.push_back(std::make_tuple(k, std::make_tuple(items[i][k], items[j][k])));

					std::vector<double> dissimilarity_vector;
					dissimilarity_vector.reserve(dimension);

					for (auto const&[_dimension, data_pair] : data_vec)
						std::visit(overloaded{
								[_dimension, &dissimilarity_vector, &range](numeric_t const& n1, numeric_t const& n2) { dissimilarity_vector.push_back(std::abs(std::get<0>(n1) - std::get<0>(n2)) / std::get<0>(range[_dimension])); },
								[i, j, _dimension, &dissimilarity_vector, &ordinals, &range](ordinal_t const& o1, ordinal_t const& o2) { dissimilarity_vector.push_back(std::abs(std::get<0>(ordinals[_dimension][i]) - std::get<0>(ordinals[_dimension][j])) / std::get<0>(range[_dimension])); },
								[&dissimilarity_vector](binary_t const& b1, binary_t const& b2) { dissimilarity_vector.push_back((std::get<0>(b1) == std::get<0>(b2)) ? 0.0 : 1.0); },
								// [&dissimilarity_vector]<typename T>(T const& d1, T const& d2) { dissimilarity_vector.push_back((d1 == d2) ? 0.0 : 1.0); },  until C++20
								other_t{ dissimilarity_vector },
								[](auto const& d1, auto const& d2) { std::cout << "fucking match" << std::endl; assert(false); },
							}, std::get<0>(data_pair), std::get<1>(data_pair));

					// d[i][j] dissimilarity-vector  =>  d[i][j].dissimilarity
					double dissimilarity_result = 0.0;
					dimension_t delta = 0;
					for (dimension_t k = 0; k < dimension; k++)
					{
						if (bin_dimension.find(k) != bin_dimension.end() // binary_t
							&& std::get<0>(std::get<binary_t>(items[i][k])) == false
							&& std::get<0>(std::get<binary_t>(items[j][k])) == false)
							continue;
						delta++;
						dissimilarity_result += dissimilarity_vector[k];
					}
					dissimilarity_result /= delta;
					dissimilarity_matrix[i][j] = dissimilarity_result;

				}

			// fill dissimilarity_matrix
			for (std::size_t i = 0; i < size; i++) dissimilarity_matrix[i][i] = 0;
			for (std::size_t i = 0; i < size; i++)
				for (std::size_t j = i + 1; j < size; j++)
					dissimilarity_matrix[i][j] = dissimilarity_matrix[j][i];

			return dissimilarity_matrix;
		}

	} // end namespace _impl

	void test()
	{
		using namespace _impl;
		const std::string s1 = "hello";
		const std::string s2 = "qwe";
		const std::string s3 = "asd";
		const std::string s4 = "rsy";


		std::vector<std::vector<item_t>> items;
		std::vector<item_t> item;

		item.push_back(std::tuple<const double>(3.14));
		item.push_back(std::tuple<const double>(113.14));
		item.push_back(std::tuple<const std::size_t>(72u));
		item.push_back(std::tuple<const std::size_t>(2u));
		item.push_back(std::tuple<const bool>(false));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(s1);
		item.push_back(s1);
		item.push_back(s2);
		item.push_back(s3);
		item.push_back(s4);
		items.push_back(item);
		item.clear();

		item.push_back(std::tuple<const double>(0.7));
		item.push_back(std::tuple<const double>(-23.5));
		item.push_back(std::tuple<const std::size_t>(12u));
		item.push_back(std::tuple<const std::size_t>(999u));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(s2);
		item.push_back(s3);
		item.push_back(s3);
		item.push_back(s1);
		item.push_back(s4);
		items.push_back(item);
		item.clear();

		item.push_back(std::tuple<const double>(77.5));
		item.push_back(std::tuple<const double>(-123.4));
		item.push_back(std::tuple<const std::size_t>(99u));
		item.push_back(std::tuple<const std::size_t>(1u));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(std::tuple<const bool>(false));
		item.push_back(s4);
		item.push_back(s3);
		item.push_back(s2);
		item.push_back(s2);
		item.push_back(s1);
		items.push_back(item);
		item.clear();

		item.push_back(std::tuple<const double>(0.78));
		item.push_back(std::tuple<const double>(94.2));
		item.push_back(std::tuple<const std::size_t>(12u));
		item.push_back(std::tuple<const std::size_t>(21u));
		item.push_back(std::tuple<const bool>(false));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(s3);
		item.push_back(s2);
		item.push_back(s2);
		item.push_back(s4);
		item.push_back(s4);
		items.push_back(item);
		item.clear();

		item.push_back(std::tuple<const double>(1204.2));
		item.push_back(std::tuple<const double>(-127.2));
		item.push_back(std::tuple<const std::size_t>(756u));
		item.push_back(std::tuple<const std::size_t>(121u));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(std::tuple<const bool>(true));
		item.push_back(s1);
		item.push_back(s3);
		item.push_back(s4);
		item.push_back(s1);
		item.push_back(s1);
		items.push_back(item);
		item.clear();

		item.push_back(std::tuple<const double>(-9.2));
		item.push_back(std::tuple<const double>(233.3));
		item.push_back(std::tuple<const std::size_t>(9u));
		item.push_back(std::tuple<const std::size_t>(10425u));
		item.push_back(std::tuple<const bool>(false));
		item.push_back(std::tuple<const bool>(false));
		item.push_back(s3);
		item.push_back(s1);
		item.push_back(s4);
		item.push_back(s2);
		item.push_back(s1);
		items.push_back(item);
		item.clear();


		std::vector<std::vector<double>> dissimilarity_matrix = dissimilarity(items);

		const char* output_path = "./dissimilarity_matrix.txt";
		std::ofstream dissimilarity_output;
		dissimilarity_output.open(output_path, std::ios::out | std::ios::trunc);
		if (!dissimilarity_output.is_open())
		{
			std::cout << "failed to open dissimilarity_matrix.txt" << std::endl;
			exit(0);
		}
		for (auto const& vec : dissimilarity_matrix)
		{
			for (double rate : vec)
				dissimilarity_output << std::left << std::setw(12) << rate;
			dissimilarity_output << std::endl;
		}
		dissimilarity_output.close();
	}

}// end namespace BI_dissimilarity

#endif // !_DISSIMILARITY_HPP