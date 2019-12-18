
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `sg14::fixed_point` operators

#if !defined(SG14_FIXED_POINT_OPERATORS_H)
#define SG14_FIXED_POINT_OPERATORS_H 1

#include "fixed_point_arithmetic.h"

namespace sg14 {
	
	// add
	template<class LhsRep,int LhsExponent,
			class RhsRep,int RhsExponent>
	auto operator + (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::add_tag)) {
		return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag);
	}
}

#endif
