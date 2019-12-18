
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(SG14_COMMON_H)
#define SG14_COMMON_H 1

#include <utility>

namespace sg14 {
	namespace _impl {
		
		template<class T>
		constexpr T min(T a,T b) {
			return (a < b) ? a : b;
		}
		
		template<class T>
		constexpr T max(T a,T b) {
			return (a > b) ? a : b;
		}

		struct arithmetic_op {
			static const bool is_arthmetic = true;
		};

		struct add_op : arithmetic_op {
			template<class Lhs,class Rhs>
			auto operator() (const Lhs& lhs,const Rhs& rhs) const -> decltype(lhs + rhs) {
				return lhs + rhs;
			}
		};

		static add_op add_tag {};

		//sg14::_impl::rep_op_result
		template<class Operator,class Lhs,class Rhs>
		using op_result = decltype(Operator()(std::declval<Lhs>(),std::declval<Rhs>()));
	}
}

#endif
