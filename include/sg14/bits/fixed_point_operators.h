
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
	constexpr auto operator + (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::add_tag)) {
		return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag);
	}

	//subtract
	template <class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	constexpr auto operator - (const fixed_point<LhsRep,LhsExponent>&lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::subtract_tag)) {
			return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::subtract_tag);
	}

	//multiply
	template <class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	constexpr auto operator * (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>&rhs) 
	-> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::multiply_tag)) {
			return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs,rhs,_impl::multiply_tag);
	}

	// divide
	template<class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	constexpr auto operator / (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::divide_tag)) {
		return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::divide_tag);
	}
	
	// mod
	template<class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
	constexpr auto operator % (const fixed_point<LhsRep,LhsExponent>& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::mod_tag)) {
		return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs,rhs,_impl::mod_tag);
	}

	////////////////////////////////////////////////////////////////////////////////
	//// heterogeneous operator overloads
	////
	//// compare two objects of different fixed_point specializations
	namespace _fixed_point_operators_impl {
		template<class Lhs,class Rhs>
		constexpr bool is_heterogeneous() {
			return (!std::is_same<Lhs,Rhs>::value) &&
				(_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value);
		}
	}

	namespace _impl {
		template<class Operator,class Lhs,int Exponent,class Rhs,class = _impl::enable_if_t<Operator::is_comparison && std::numeric_limits<Rhs>::is_specialized>>
		constexpr auto operate(const fixed_point<Lhs,Exponent>& lhs,const Rhs& rhs,Operator op)
		-> decltype(op(lhs,static_cast<fixed_point<Lhs,Exponent>>(rhs))) {
			return op(lhs,static_cast<fixed_point<Lhs,Exponent>>(rhs));
		}

		template<class Operator,class Lhs,class Rhs,int Exponent,class = _impl::enable_if_t<Operator::is_comparison && std::numeric_limits<Lhs>::is_specialized>>
		constexpr auto operate(const Lhs& lhs,const fixed_point<Rhs,Exponent>& rhs,Operator op)
		-> decltype(op(static_cast<fixed_point<Rhs,Exponent>>(lhs),rhs)) {
			return (op(static_cast<fixed_point<Rhs,Exponent>>(lhs),rhs));
		}

		template<class Operator,class Rep,int Exponent,class = _impl::enable_if_t<Operator::is_comparison>>
		constexpr auto operate(const fixed_point<Rep,Exponent>&lhs,const fixed_point<Rep,Exponent>& rhs,Operator op) 
		-> decltype(op(lhs.data(),rhs.data())){
			return op(lhs.data(),rhs.data());
		}
	}

	// fixed_point @ non_fixed_point arithmetic operators
	// fixed_point ,integer -> fixedpoint
	
	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
	constexpr auto operator + (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs) 
	-> decltype(lhs + fixed_point<LhsRep,LhsExponent>{rhs}) {
		return lhs + fixed_point<LhsRep,LhsExponent>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
	constexpr auto operator - (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs - fixed_point<LhsRep,LhsExponent>{rhs}) {
		return lhs - fixed_point<LhsRep,LhsExponent>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
	constexpr auto operator * (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs * fixed_point<LhsRep,LhsExponent>{rhs}) {
		return lhs * fixed_point<LhsRep,LhsExponent>{rhs};
	}

	template<class LhsRep,int LhsExponent,class RhsInteger,typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
	constexpr auto operator / (const fixed_point<LhsRep,LhsExponent>&lhs,const RhsInteger& rhs)
	-> decltype(lhs / fixed_point<LhsRep,LhsExponent>{rhs}) {
		return lhs / fixed_point<LhsRep,LhsExponent>{rhs};
	}

	// integer,fixed_point -> fixed_point
	template<class LhsInteger, class RhsRep, int RhsExponent,typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
	constexpr auto operator+(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
	-> decltype(fixed_point<RhsRep, RhsExponent>{lhs} + rhs) {
		return fixed_point<RhsRep, RhsExponent>{lhs} + rhs;
	}

	template<class LhsInteger,class RhsRep,int RhsExponent,typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
	constexpr auto operator-(const LhsInteger& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(fixed_point<RhsRep, RhsExponent>{lhs} - rhs) {
		return fixed_point<RhsRep, RhsExponent>{lhs} - rhs; 
	}

	template<class LhsInteger,class RhsRep,int RhsExponent,typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
	constexpr auto operator*(const LhsInteger& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(fixed_point<RhsRep, RhsExponent>{lhs} * rhs) {
		return fixed_point<RhsRep, RhsExponent>{lhs} * rhs;
	}

	template<class LhsInteger,class RhsRep,int RhsExponent,typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
	constexpr auto operator/(const LhsInteger& lhs,const fixed_point<RhsRep,RhsExponent>& rhs)
	-> decltype(fixed_point<RhsRep,RhsExponent>{lhs} / fixed_point<RhsRep,RhsExponent>{rhs}) {
		return fixed_point<RhsRep, RhsExponent>{lhs} / fixed_point<RhsRep,RhsExponent>{rhs};
	}

	// fixed-point, floating-point -> floating-point
	template<class LhsRep,int LhsExponent,class RhsFloat,typename = _impl::enable_if_t<std::is_floating_point<RhsFloat>::value>>
	constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)
	->_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat> {
		using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
		return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
	}

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t <std::is_floating_point<RhsFloat>::value>>
	constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat> {
		using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
		return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
	}

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>,_impl::enable_if_t<std::is_floating_point<RhsFloat>::value, RhsFloat>>
    {
        return lhs * fixed_point<LhsRep, LhsExponent>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>,_impl::enable_if_t<std::is_floating_point<RhsFloat>::value, RhsFloat>>
    {
        return lhs / fixed_point<LhsRep, LhsExponent>(rhs);
    }


    // floating-point, fixed-point -> floating-point
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t <std::is_floating_point<LhsFloat>::value>>
    constexpr auto operator+(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t <std::is_floating_point<LhsFloat>::value>>
    constexpr auto operator-(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t <_impl::enable_if_t<std::is_floating_point<LhsFloat>::value, LhsFloat>, fixed_point<RhsRep, RhsExponent>>
    {
        return fixed_point<RhsRep, RhsExponent>(lhs) * rhs;
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t <_impl::enable_if_t<std::is_floating_point<LhsFloat>::value, LhsFloat>, fixed_point<RhsRep, RhsExponent>>
    {
        return fixed_point<RhsRep, RhsExponent>(lhs) / rhs;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators
	//
	// 
	template<class LhsRep,int LhsExponent,class Rhs>
	constexpr auto operator <<(const fixed_point<LhsRep,LhsExponent>& lhs,const Rhs& rhs)
	-> decltype(_impl::from_rep<fixed_point<decltype(lhs.data() << rhs), LhsExponent>>(lhs.data() << rhs)) {
		return _impl::from_rep<fixed_point<decltype(lhs.data() << rhs), LhsExponent>>(lhs.data() << rhs);
	}

	template<class LhsRep, int LhsExponent, class Rhs>
	constexpr auto operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
	-> decltype(_impl::from_rep<fixed_point<decltype(lhs.data() >> rhs), LhsExponent>>(lhs.data() >> rhs)) {
		return _impl::from_rep<fixed_point<decltype(lhs.data() >> rhs), LhsExponent>>(lhs.data() >>rhs);
	}
}

#endif
