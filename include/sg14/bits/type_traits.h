
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `sg14::fixed_point` type

#if !defined(SG14_TYPE_TRAITS_H)
#define SG14_TYPE_TRAITS_H 1

#include <type_traits>

// study group 14 of c++ working group

namespace sg14 {
	namespace _impl {
		// sg14::impl::common_type_t

		//pre-c++14 common_type_t
		template<class ... T>
		using common_type_t = typename std::common_type<T ...>::type;

		// pre-c++14 enable_if_t
		template <bool C, class ... T>
		using enable_if_t = typename std::enable_if<C,T ...>::type;
	}
}

#endif
