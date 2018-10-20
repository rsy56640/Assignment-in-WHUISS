#pragma once
#ifndef _DISSIMILARITY_HPP
#define _DISSIMILARITY_HPP
#include <vector>
#include <variant>
#include <optional>
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

		using numeric_t = std::optional<std::tuple<const double>>;
		using ordinal_t = std::optional<std::tuple<const float>>;
		using binary_t = std::optional<std::tuple<const bool>>;


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

			const std::tuple<const double> lowest = std::tuple<const double>(std::numeric_limits<double>::lowest());
			const std::tuple<const double> infinity = std::tuple<const double>(std::numeric_limits<double>::infinity());
			const std::tuple<const std::size_t> ordinal_max = std::tuple<const std::size_t>(0xffffffff);
			const std::tuple<const std::size_t> ordinal_min = std::tuple<const std::size_t>(0);


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
				if (auto pval = std::get_if<numeric_t>(&(items[0][j])))
					range.insert(std::make_pair(j, std::make_tuple<const double>(std::accumulate(
						items.begin(), items.end(), std::numeric_limits<double>::lowest(),
						[j, lowest](double& _max, std::vector<item_t> const& xi) { return max(_max, std::get<0>(std::get<numeric_t>(xi[j]).value_or(lowest))); })
						- std::accumulate(items.begin(), items.end(), std::numeric_limits<double>::infinity(),
							[j, infinity](double& _min, std::vector<item_t> const& xi) { return min(_min, std::get<0>(std::get<numeric_t>(xi[j]).value_or(infinity))); }))));

			// for ordinal calculation
			// 1 1 null 7 8 => 1 1 null 2 3
			std::unordered_map<dimension_t, std::vector<numeric_t>> ordinals;
			for (std::size_t j = 0; j < dimension; j++)
				if (auto pval = std::get_if<ordinal_t>(&items[0][j]))
				{
					using value_t = float;
					std::vector<value_t> v;
					for (std::size_t i = 0; i < size; i++)
						if (std::get<ordinal_t>(items[i][j]).has_value()) v.push_back(std::get<0>(std::get<ordinal_t>(items[i][j]).value()));

					// cannot compute
					if (v.size() <= 1)
					{
						std::vector<numeric_t> v(size, std::nullopt);
						ordinals[j] = std::move(v);
						if (v.empty()) range.insert(std::make_pair(j, std::nullopt));
						else range.insert(std::make_pair(j, 0));
						continue;
					}

					std::sort(v.begin(), v.end());

					using ord_t = std::size_t;

					std::set<value_t> filter_same;
					for (auto _v : v) filter_same.insert(_v);
					const ord_t value_max = filter_same.size(); // M

					std::map<value_t, double> value; // value -> (r - 1) / (M - 1)
					value_t _value = v[0];
					ord_t _count = 1;
					value[_value] = 1.0 * (_count - 1) / (value_max - 1);

					const std::size_t v_size = v.size();
					for (ord_t i = 1; i < v_size; i++)
					{
						if (v[i] > _value) // v[i] order = (i+1)
						{
							_count++;
							value[v[i]] = 1.0 * (_count - 1) / (value_max - 1);
						}
						else // v[i] order = _count
							value[v[i]] = 1.0 * (_count - 1) / (value_max - 1);
					}

					ordinals[j].reserve(size);
					for (std::size_t i = 0; i < size; i++)
					{
						if (std::get<ordinal_t>(items[i][j]).has_value())
							ordinals[j].push_back(value[std::get<0>(std::get<ordinal_t>(items[i][j]).value())]);
						else ordinals[j].push_back(std::nullopt);
					}

					range.insert(std::make_pair(j, 1));

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
								[_dimension, &dissimilarity_vector, &range](numeric_t const& n1, numeric_t const& n2) { if (n1.has_value() && n2.has_value()) dissimilarity_vector.push_back(std::abs(std::get<0>(n1.value()) - std::get<0>(n2.value())) / std::get<0>(range[_dimension].value())); else dissimilarity_vector.push_back(1.0); },
								[i, j, _dimension, &dissimilarity_vector, &ordinals, &range](ordinal_t const& o1, ordinal_t const& o2) { if (ordinals[_dimension][i].has_value() && ordinals[_dimension][j].has_value()) dissimilarity_vector.push_back(std::abs(std::get<0>(ordinals[_dimension][i].value()) - std::get<0>(ordinals[_dimension][j].value())) / std::get<0>(range[_dimension].value())); else dissimilarity_vector.push_back(1.0); },
								[&dissimilarity_vector](binary_t const& b1, binary_t const& b2) { if (b1.has_value() && b2.has_value()) dissimilarity_vector.push_back((std::get<0>(b1.value()) == std::get<0>(b2.value())) ? 0.0 : 1.0); else dissimilarity_vector.push_back(1.0); },
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
							&& std::get<0>(std::get<binary_t>(items[i][k]).value_or(false)) == false
							&& std::get<0>(std::get<binary_t>(items[j][k]).value_or(false)) == false)
							continue;
						if (auto pval = std::get_if<numeric_t>(&items[0][k]))
						{
							if (!std::get<numeric_t>(items[i][k]).has_value() || !std::get<numeric_t>(items[j][k]).has_value())
								continue;
						}
						if (auto pval = std::get_if<ordinal_t>(&items[0][k]))
						{
							if (!std::get<ordinal_t>(items[i][k]).has_value() || !std::get<ordinal_t>(items[j][k]).has_value())
								continue;
						}
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
		const std::string spring = "spring";
		const std::string summer = "summer";
		const std::string fall = "fall";
		const std::string winter = "winter";

		const numeric_t non_num = std::nullopt;
		const ordinal_t non_ord = std::nullopt;
		const binary_t non_bin = std::nullopt;


		/*          num     num       scalar      ord   binary        num        num
		 *          大小    含水量    成熟季节    pH    是否要去皮    维C含量    蛋白质
		 *   苹果   113     0.68      fall        4.9   false         4.0        0.27
		 *   梨     100     0.71      fall        3.5   false         4.0        0.3
		 *   西瓜   5000    0.91      summer      6.6   true          8.1        0.61
		 *   葡萄   4       0.83      summer      4.5   true          10.8       0.72
		 *   草莓   5       0.81      summer      3.0   false         58.8       0.67
		 */

		std::vector<std::vector<item_t>> items;
		std::vector<item_t> item;

		item.push_back(std::make_optional(std::tuple<const double>(113)));
		item.push_back(std::make_optional(std::tuple<const double>(0.68)));
		item.push_back(fall);
		item.push_back(std::make_optional(std::tuple<const float>(4.9)));
		item.push_back(std::make_optional(std::tuple<const bool>(false)));
		item.push_back(std::make_optional(std::tuple<const double>(4.0)));
		item.push_back(std::make_optional(std::tuple<const double>(0.27)));
		items.push_back(item);
		item.clear();

		item.push_back(std::make_optional(std::tuple<const double>(100)));
		item.push_back(std::make_optional(std::tuple<const double>(0.71)));
		item.push_back(fall);
		item.push_back(std::make_optional(std::tuple<const float>(3.5)));
		item.push_back(std::make_optional(std::tuple<const bool>(false)));
		item.push_back(std::make_optional(std::tuple<const double>(4.0)));
		item.push_back(std::make_optional(std::tuple<const double>(0.3)));
		items.push_back(item);
		item.clear();

		item.push_back(std::make_optional(std::tuple<const double>(5000)));
		item.push_back(std::make_optional(std::tuple<const double>(0.91)));
		item.push_back(summer);
		item.push_back(std::make_optional(std::tuple<const float>(6.6)));
		item.push_back(std::make_optional(std::tuple<const bool>(true)));
		item.push_back(std::make_optional(std::tuple<const double>(8.1)));
		item.push_back(std::make_optional(std::tuple<const double>(0.61)));
		items.push_back(item);
		item.clear();

		item.push_back(std::make_optional(std::tuple<const double>(4)));
		item.push_back(std::make_optional(std::tuple<const double>(0.83)));
		item.push_back(summer);
		item.push_back(std::make_optional(std::tuple<const float>(4.5)));
		item.push_back(std::make_optional(std::tuple<const bool>(true)));
		item.push_back(std::make_optional(std::tuple<const double>(10.8)));
		item.push_back(std::make_optional(std::tuple<const double>(0.72)));
		items.push_back(item);
		item.clear();

		item.push_back(std::make_optional(std::tuple<const double>(5)));
		item.push_back(std::make_optional(std::tuple<const double>(0.81)));
		item.push_back(summer);
		item.push_back(std::make_optional(std::tuple<const float>(3.0)));
		item.push_back(std::make_optional(std::tuple<const bool>(false)));
		item.push_back(std::make_optional(std::tuple<const double>(58.8)));
		item.push_back(std::make_optional(std::tuple<const double>(0.67)));
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
				dissimilarity_output << std::fixed << std::setprecision(4) << rate << "   ";
			dissimilarity_output << std::endl;
		}
		dissimilarity_output.close();
	}

}// end namespace BI_dissimilarity

#endif // !_DISSIMILARITY_HPP
