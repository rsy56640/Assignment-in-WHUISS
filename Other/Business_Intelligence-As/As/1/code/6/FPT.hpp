#ifndef _FPT_HPP
#define _FPT_HPP
#include "FPTImpl.hpp"
#include <memory>

namespace RSY_TOOL::FPT
{

	/*
	 * The ordered-list should be prepared outside,
	 * and insert-items should be ordered,
	 * and discarded items must not occur.
	 */
	template<typename Key> class FPT :FPTType<Key>
	{
		using node_ptr = typename FPTType<Key>::node_ptr;
		using key_type = typename FPTType<Key>::key_type;

	public:

		explicit FPT(
			std::vector<Key>&& list,
			const std::size_t min_support,
			std::map<std::set<Key>, std::size_t>& return_sets)
			:_pImpl(std::make_unique<FPTImpl<Key>>(std::move(list), min_support, std::set<Key>{}, return_sets))
		{}
		FPT(const FPT&) = delete;
		FPT& operator=(const FPT&) = delete;
		FPT(FPT&&) = default;
		FPT& operator=(FPT&&) = default;

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
			_pImpl->insert(std::begin(container), std::end(container));
		}

		void mine()
		{
			_pImpl->mine();
		}

	private:

		std::unique_ptr<FPTImpl<Key>> _pImpl;

	};//end class FPT

}//end namespace RSY_TOOL::FPT

#endif // !_FPT_HPP
