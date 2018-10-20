#ifndef _TRIEIMPL_HPP
#define _TRIEIMPL_HPP
#include "FPTType.hpp"
#include <iterator>
#include <stack>
#include <list>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <type_traits>

namespace RSY_TOOL::FPT
{

	// bigInt help to count
	namespace util
	{
		class bigInt {
			/*
			* 1 2 ... n order
			* 0 0 ... 0 vector bits
			* the bit num is bits[bit-1]
			*/
		public:
			bigInt(std::size_t size) :bits(size, 0) {}
			bigInt(std::size_t bit, std::size_t size) :bits(max(size, bit), 0) { bits[bit - 1] = 1; }
			bigInt& operator++() { addOne(); return *this; }
			bool hasBit(std::size_t bit)
			{
				if (bits.size() < bit)
				{
					bits.resize(bit);
					return false;
				}
				return bits[bit - 1] == 1;
			}
			bool operator!=(const bigInt& other) const
			{
				if (bits.size() > other.bits.size())
				{
					const std::size_t size = other.bits.size();
					const std::size_t big_size = bits.size();
					for (std::size_t i = size; i < big_size; i++)
						if (bits[i] == 1) return true;
					for (std::size_t i = 0; i < size; i++)
						if (bits[i] != other.bits[i]) return true;
					return false;
				}
				else
				{
					const std::size_t size = bits.size();
					const std::size_t big_size = other.bits.size();
					for (std::size_t i = size; i < big_size; i++)
						if (other.bits[i] == 1) return true;
					for (std::size_t i = 0; i < size; i++)
						if (bits[i] != other.bits[i]) return true;
					return false;
				}
			}
			// return 0 ~ n-1
			std::vector<std::size_t> combination_nums() const
			{
				std::vector<std::size_t> nums;
				const std::size_t size = bits.size();
				for (std::size_t i = 0; i < size; i++)
					if (bits[i] == 1) nums.push_back(i);
				return nums;
			}
		private:
			std::vector<char> bits;
			void addOne()
			{
				const std::size_t size = bits.size();
				for (std::size_t i = 0; i < size; i++)
				{
					if (bits[i] == 1)
						bits[i] = 0;
					else
					{
						bits[i] = 1;
						return;
					}
				}
				bits.resize(size + 1);
				bits[size] = 1;
			}
		};
	}

	template<typename Key> class FPTImpl :public FPTType<Key>
	{
		using node_ptr = typename FPTType<Key>::node_ptr;
		using key_type = typename FPTType<Key>::key_type;

	public:

		explicit FPTImpl(
			std::vector<Key>&& list,
			const std::size_t min_support,
			std::set<Key>&& to_be_added,
			std::map<std::set<Key>, std::size_t>& return_sets)
			:
			_root(make_root<Key>()),
			_list(std::move(list)),
			_min_support(min_support),
			_to_be_added(std::move(to_be_added)),
			_return_sets(return_sets)
		{}
		FPTImpl(const FPTImpl&) = delete;
		FPTImpl& operator=(const FPTImpl&) = delete;
		FPTImpl(FPTImpl&& other) :_root(other._root), _table(std::move(other._table)) { other._root = nullptr; }
		FPTImpl& operator=(FPTImpl&& other)
		{
			_root = other._root;
			other._root = nullptr;
			_table = std::move(other._table);
			return *this;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<_InIt>::iterator_category, std::forward_iterator_tag> &&
			std::is_convertible_v<std::decay_t<std::conditional_t<std::is_pointer_v<_InIt>, std::remove_pointer_t<_InIt>, decltype(std::declval<_InIt>().operator*())>>, Key>
			>* = nullptr
		> void insert(_InIt first, _InIt last)
		{
			node_ptr root = _root;
			node_ptr next;
			bool not_match = false;
			while (first != last)
			{
				if (!not_match)
				{
					next = root->find(*first);
					if (next == nullptr) // make new node
					{
						not_match = true;
						next = make_node<Key>(*first);
						root = root->addKey(*first, next);
					}
					else root = next;
				}
				else // make new node
				{
					next = make_node<Key>(*first);
					root = root->addKey(*first, next);
				}
				_table[*first].push_back(next);
				root->addReference();
				++first;
			}
		} // end function insert();


		template<typename C>
		void insert(C&& container)
		{
			this->insert(std::begin(container), std::end(container));
		}


		void swap(FPTImpl& other)
		{
			std::swap(_root, other._root);
			std::swap(_table, other._table);
		}


		~FPTImpl()
		{
			if (_root == nullptr) return;
			std::stack<node_ptr> stk;
			stk.push(_root);
			while (!stk.empty())
			{
				node_ptr node = stk.top(); stk.pop();
				for (auto it : node->next)
					stk.push(it.second);
				erase_node(node);
			}
		}


		/* recursively mine
		 *     1. get_cond_FP(): when branch is empty, add (_to_be_added * support).
		 *     2. after get_cond_FP() has been pruned, if set is empty, immdiately add.
		 *     3. if sub-FP is non-single, `temp_to_be_added` must be added for `*rit.size()`.
		 *     4. combine(): when branch is non-empty, add ((2^N sets + _to_be_added) * support).
		 */
		void mine()
		{
			if (is_single_tree())
			{
				combine();
				return;
			}
			for (auto rit = _list.rbegin(); rit != _list.rend(); ++rit)
			{
				// prepare context:
				//     1. list
				//     2. item2int
				//     3. to_be_added
				//     4. order sub_FP
				std::set<Key> to_be_added = _to_be_added;
				to_be_added.insert(*rit);
				std::set<Key> temp_to_be_added = to_be_added;

				std::vector<std::vector<Key>> sub_FP = get_cond_FP(*rit);

				// the empty sets has added inside `get_cond_FP()`.
				if (sub_FP.empty()) continue;

				std::map<Key, std::size_t> item_occur_set;
				for (auto const& set : sub_FP)
					for (auto const& item : set)
						item_occur_set[item]++;
				// filter according to `_min_support`
				// than make `list` order
				std::vector<Key> list;
				std::map<Key, std::size_t> item2int;
				std::set<Key> discar_items;
				for (auto const&[item, amount] : item_occur_set) // filter
				{
					if (amount >= _min_support)
						list.push_back(item);
					else discar_items.insert(item);
				}
				// erase discarded items
				for (auto& set : sub_FP)
					for (auto it = set.begin(); it != set.end();)
					{
						if (discar_items.find(*it) != discar_items.end())
							it = set.erase(it);
						else ++it;
					}
				// order list (big -> small)
				std::sort(list.begin(), list.end(),
					[&item_occur_set](Key const& lhs, Key const& rhs)->bool
				{ return item_occur_set[lhs] > item_occur_set[rhs]; });
				// order sub_FP
				std::map<Key, std::size_t> order;
				std::size_t _max = 0xffffffff;
				for (Key& const key : list)
					order[key] = _max--;
				for (auto &set : sub_FP)
					std::sort(set.begin(), set.end(), [&order](Key const& lhs, Key const& rhs)->bool
				{ return order[lhs] > order[rhs]; });

				// construct FP
				FPTImpl<Key> fp(
					std::move(list),
					_min_support,
					std::move(to_be_added),
					_return_sets
				);
				for (std::vector<Key> const& set : sub_FP)
				{
					if (set.empty()) // after get_cond_FP() has been pruned, if set is empty, immdiately add.
						_return_sets[temp_to_be_added]++;
					else fp.insert(set);
				}

				// if non-single, `temp_to_be_added` will lost.
				if (!fp.is_single_tree())
					_return_sets[temp_to_be_added] += _table[*rit].size();

				fp.mine(); // recursively mine

			}

		} // end void mine();


	private:

		node_ptr _root;
		std::map<Key, std::list<node_ptr>> _table;
		const std::vector<Key> _list;

		const std::set<Key> _to_be_added;
		std::map<std::set<Key>, std::size_t>& _return_sets;
		const std::size_t _min_support;


		bool is_single_tree() const
		{
			node_ptr root = _root;
			while (root->next.size() <= 1)
			{
				if (root->next.empty()) return true;
				root = root->next.begin()->second;
			}
			return false;
		}


		/*
		 * when one branch is empty
		 * immediately add (_to_be_added * support).
		 */
		std::vector<std::vector<Key>> get_cond_FP(const Key& key) const
		{
			const std::list<node_ptr>& table = _table.at(key);
			std::vector<std::vector<Key>> cond_fp;

			std::set<Key> temp_to_be_added = _to_be_added;
			temp_to_be_added.insert(key);

			for (node_ptr node : table)
			{
				std::vector<Key> branch;
				node_ptr last = node->_parent;
				while (last != _root)
				{
					branch.push_back(last->_key.value());
					last = last->_parent;
				}
				if (branch.empty()) // add (_to_be_added * support)
				{
					_return_sets[temp_to_be_added]++;
					continue;
				}
				cond_fp.push_back(std::move(branch));
			}
			return cond_fp;
		}


		/*
		 * when the single line has the same support, invoke `real_combine()`.
		 * if fp is a single-line-tree, than combine
		 * add the frequent sets to `_return_sets`,
		 * do not forget `_to_be_added`
		 */
		void combine()
		{
			node_ptr node = _root;
			std::size_t _size = 0; // must not be 0.Read the comment of `mine()`.
			std::vector<std::pair<Key, std::size_t>> _branch;
			while (!node->next.empty())
			{
				_size++;
				node = node->next.begin()->second;
				_branch.push_back(std::make_pair(node->_key.value(), node->_support));
			}

			const std::size_t size = _size;

			if (_branch.empty()) return;

			auto const& branches = slice_branch(_branch);
			for (auto const&[branch, support] : branches)
				real_combine(branch, support);

		}


		/*
		 * Promise:
		 *     `combine()`: support is non-increasing
		 *
		 *     AAA             A                 A
		 *     BBB       =>    B  *  2     +     B  *  1
		 *     CC              C
		 *     DD              D
		 */
		std::vector<std::pair<std::vector<Key>, std::size_t>> slice_branch(const std::vector<std::pair<Key, std::size_t>>& _branch)
		{
			const std::size_t major_support = _branch.back().second; // major_support is minimum.

			// slice the branches
			std::vector<std::size_t> slices;
			std::size_t cur_support = _branch[0].second; // promise (size!=0) in `combine()`
			std::size_t cur_location = 0;
			for (auto const&[key, support] : _branch)
			{
				if (support < cur_support)
				{
					cur_support = support;
					slices.push_back(cur_location);
				}
				cur_location++;
			}

			std::vector<std::pair<std::vector<Key>, std::size_t>> _branches;
			for (std::size_t i : slices)
			{
				std::vector<Key> branch;
				const std::size_t support = _branch[i - 1].second - _branch[i].second;
				for (std::size_t no = 0; no < i; no++)
					branch.push_back(_branch[no].first);
				_branches.push_back(std::make_pair(std::move(branch), std::move(support)));
			}

			// process major-branch
			std::vector<Key> major_branch;
			major_branch.reserve(_branch.size());
			for (auto&[key, support] : _branch) major_branch.push_back(key);
			_branches.push_back(std::make_pair(std::move(major_branch), std::move(major_support)));

			return _branches;

		}


		/*
		 * the single line has the same support,
		 * than combine.
		 */
		void real_combine(const std::vector<Key>& branch, const std::size_t support)
		{
			std::vector<std::set<Key>> sets;
			const std::size_t _size = branch.size();

			// combine
			util::bigInt it{ _size };
			const util::bigInt _max{ _size + 1, _size + 1 };
			for (; it != _max; ++it)
			{
				std::vector<std::size_t> combination_nums = it.combination_nums();
				std::set<Key> s;
				for (std::size_t i : combination_nums)
					s.insert(branch[i]);
				sets.push_back(std::move(s));
			}

			// add `_to_be_added`
			for (std::set<Key>& set : sets)
				for (Key const& key : _to_be_added)
					set.insert(key);

			// add to `_return_sets`
			for (auto const& set : sets)
				if (!set.empty()) _return_sets[set] += support;

		}


	}; // end class FPTImpl


} // end namespace RSY_TOOL::FPT

#endif // !_TRIEIMPL_HPP
