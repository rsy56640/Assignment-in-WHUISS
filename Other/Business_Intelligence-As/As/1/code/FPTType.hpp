#ifndef _TRIETYPE_HPP
#define _TRIETYPE_HPP
#include "FPTNode.hpp"

namespace RSY_TOOL::FPT
{

	template<typename Key> struct FPTType
	{
		using node_ptr = typename FPTNode<Key>::node_ptr;
		using key_type = typename FPTNode<Key>::key_type;
	};

}//end namespace RSY_TOOL::FPT

#endif // !_TRIETYPE_HPP
