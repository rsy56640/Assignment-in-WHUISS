#ifndef _TRIEIMPL_HPP
#define _TRIEIMPL_HPP
#include "FPTType.hpp"
#include <iterator>
#include <stack>
#include <list>

namespace RSY_TOOL::FPT
{

	template<typename Key> class FPTImpl :public FPTType<Key>
	{
		using node_ptr = typename FPTType<Key>::node_ptr;
		using key_type = typename FPTType<Key>::key_type;

	public:

		explicit FPTImpl() :_root(make_root<Key>()) {}
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
		> bool find(_InIt first, _InIt last) const
		{
			node_ptr root = _root;
			node_ptr next;
			while (first != last)
			{
				next = root->find(*first);
				if (next == nullptr) return false;
				root = next;
				++first;
			}
			return root->_isKey;
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
						_table[*first].push_back(next);
						root = root->addKey(*first, next);
					}
					else root = next;
				}
				else // make new node
				{
					next = make_node<Key>(*first);
					_table[*first].push_back(next);
					root = root->addKey(*first, next);
				}
				root->addReference();
				++first;
			}
		} // end function insert();

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


	private:

		node_ptr _root;
		std::map<Key, std::list<node_ptr>> _table;

	};//end class FPTImpl


}//end namespace RSY_TOOL::FPT

#endif // !_TRIEIMPL_HPP
