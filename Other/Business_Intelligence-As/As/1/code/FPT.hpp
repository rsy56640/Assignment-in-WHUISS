#ifndef _FPT_HPP
#define _FPT_HPP
#include "FPTImpl.hpp"
#include <memory>

namespace RSY_TOOL::FPT
{

	/*
	 * TODO: sorted set
	 *
	 */
	template<typename Key> class FPT :FPTType<Key>
	{
		using node_ptr = typename FPTType<Key>::node_ptr;
		using key_type = typename FPTType<Key>::key_type;

	public:

		explicit FPT() :_pImpl(std::make_unique<FPTImpl<Key>>()) {}
		FPT(const FPT&) = delete;
		FPT& operator=(const FPT&) = delete;
		FPT(FPT&&) = default;
		FPT& operator=(FPT&&) = default;

		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<typename _InIt>
		bool find(_InIt first, _InIt last) const
		{
			return _pImpl->find(first, last);
		}

		template<typename C>
		bool find(C&& container) const
		{
			return find(std::begin(container), std::end(container));
		}

		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<typename _InIt>
		void insert(_InIt first, _InIt last)
		{
			_pImpl->insert(first, last);
		}

		template<typename C>
		void insert(C&& container)
		{
			return insert(std::begin(container), std::end(container));
		}


	private:

		std::unique_ptr<FPTImpl<Key>> _pImpl;

	};//end class FPT

}//end namespace RSY_TOOL::FPT

#endif // !_FPT_HPP
