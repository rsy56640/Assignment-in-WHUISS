#ifndef _TRIENODE_HPP
#define _TRIENODE_HPP
#include <optional>
#include <type_traits>
#include <map>

namespace RSY_TOOL::FPT
{

	template<typename Key> struct FPTNode
	{
		using node_ptr = FPTNode<Key>*;
		using key_type = Key; // std::optional<const Key>; Since Key in FP must be std::size_t
		key_type _key;
		std::size_t _support = 0;
		node_ptr _parent = nullptr;
		std::map<Key, node_ptr> next;
		template<
			typename Key_t,
			std::enable_if_t<std::is_convertible_v<std::decay_t<Key_t>, Key>>* = nullptr
		> explicit FPTNode(Key_t&& key) : _key(std::forward<Key_t>(key)) {}
		explicit FPTNode() :_key(0) {}
		FPTNode(const FPTNode&) = delete;
		FPTNode& operator=(const FPTNode&) = delete;

		void addReference() { this->_support++; }

		node_ptr find(const Key& key)
		{
			auto it = next.find(key);
			if (it != next.end()) return it->second;
			return nullptr;
		}

		node_ptr addKey(const Key& key, node_ptr new_node)
		{
			new_node->_parent = this;
			return next.insert(std::make_pair(key, new_node)).first->second;
		}

	};


	template<typename Key, typename Key_t> decltype(auto) make_node(Key_t&& key)
	{
		return new FPTNode<Key>(std::forward<Key_t>(key));
	}

	template<typename Key> decltype(auto) make_root()
	{
		return new FPTNode<Key>();
	}

	template<typename Key> void erase_node(FPTNode<Key>* node)
	{
		delete node;
	}

}//end namespace RSY_TOOL::FPT

#endif // !_TRIENODE_HPP
