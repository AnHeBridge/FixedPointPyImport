
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
	////////////////////////////////////////////////////////////////////////////////
	// (fixed_point @ fixed_point) arithmetic operators
	
	//negate
	template<class RhsRep,int RhsExponent>
	constexpr auto operator - (const fixed_point<RhsRep,RhsExponent>& rhs) 
	-> fixed_point<decltype(-rhs.data()),RhsExponent> {
		using result_type = fixed_point<decltype(-rhs.data()),RhsExponent>;
		return result_type::from_data(-rhs.data());
	}

	// add
	template<class LhsRep,int LhsExponent,
			class RhsRep,int RhsExponent>
	auto operator + (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::add_tag)) {
		return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag);
	}

	//subtract
	template <class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	auto operator - (const fixed_point<LhsRep,LhsExponent>&lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::subtract_tag)) {
			return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::subtract_tag);
	}

	//multiply
	template <class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	auto operator * (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>&rhs) 
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::multiply_tag)) {
			return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::multiply_tag);
	}

	// fixed_point,floating-point -> floating-point
	template<class LhsRep,int LhsExponent,class RhsFloat,typename = _impl::enable_if_t<std::is_floating_point<RhsFloat>::value>>
	auto operator + (const fixed_point<LhsRep,LhsExponent>& lhs,const RhsFloat& rhs) 
	-> _impl::common_type_t<fixed_point<LhsRep,LhsExponent>,RhsFloat> {
		using result_type = _impl::common_type_t<fixed_point<LhsRep,LhsExponent>,RhsFloat>;
		return static_cast<result_type>(lhs) + static_cast<result_type>(rhs);
	}

	// divide
	template<class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	auto operator / (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::divide_tag)) {
		return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::divide_tag);
	}

	// fixed_point @ non_fixed_point arithmetic operators
	// fixed_point ,integer -> fixedpoint
	
	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::integer>>
	auto operator + (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs) 
	-> decltype(lhs + fixed_point<RhsInteger,0>{rhs}) {
		return lhs + fixed_point<RhsInteger,0>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::integer>>
	auto operator - (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs - fixed_point<RhsInteger,0>{rhs}) {
		return lhs - fixed_point<RhsInteger,0>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::integer>>
	auto operator * (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs * fixed_point<RhsInteger,0>{rhs}) {
		return lhs * fixed_point<RhsInteger,0>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::integer>>
	auto operator * (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs / fixed_point<RhsInteger,0>{rhs}) {
		return lhs / fixed_point<RhsInteger,0>{rhs};
	}
}

#endif
