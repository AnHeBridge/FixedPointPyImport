
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to `sg14::fixed_point` arithmetic

#if !defined(SG14_FIXED_POINT_ARITHMETIC_H)
#define SG14_FIXED_POINT_ARITHMETIC_H 1

#include "common.h"
#include "fixed_point_type.h"

// study group 14 of the c++ working group

namespace sg14 {
	// implemetation-specific definitions
	namespace _impl {
		namespace fp {
			namespace arithmetic {
				struct lean_tag;
				struct raw_tag;
	
				//sg14::impl::fp::arithmetic::binary_pair
				template<class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
				struct binary_pair_base {
					using lhs_type = fixed_point<LhsRep,LhsExponent>;
					using rhs_type = fixed_point<RhsRep,RhsExponent>;
				};

				template<class Lhs,class Rhs>
				struct binary_pair;

				template<class LhsRep,int LhsExponent,class RhsRep,int RhsExponent>
				struct binary_pair<fixed_point<LhsRep,LhsExponent>,fixed_point<RhsRep,RhsExponent>> : binary_pair_base<LhsRep,LhsExponent,RhsRep,RhsExponent> {};

				template<class LhsRep,int LhsExponent,class Rhs>
				struct binary_pair<fixed_point<LhsRep,LhsExponent>,Rhs> : 
					binary_pair_base<LhsRep,LhsExponent,Rhs,0> {
					static_assert(std::numeric_limits<Rhs>::is_integer,"named arthmetic functions take only fixed_point and integral types");
				};

				template<class Lhs,class RhsRep,int RhsExponent>
				struct binary_pair<Lhs,fixed_point<RhsRep,RhsExponent>> : binary_pair_base<Lhs,0,RhsRep,RhsExponent> {
					static_assert(std::numeric_limits<Lhs>::is_integer,"named arithmetic functions take only fixed_point and integral types");
				};
				
				//sg14::_impl::fp::arithmetic::rep_op_exponent
				template<class OperationTag, class ... Operands>
				struct rep_op_exponent;

				template<class Lhs,class Rhs>
				struct rep_op_exponent<_impl::add_op,Lhs,Rhs> : public std::integral_constant<int,_impl::min<int>(Lhs::exponent,Rhs::exponent) > {};

				// sg14::_impl::fp::arithmetic::result
				template<class PolicyTag,class OperationTag,class Lhs,class Rhs>
				struct result;

				// result<raw_tag>
				template<class OperationTag,class Lhs,class Rhs>
				struct result<raw_tag,OperationTag,Lhs,Rhs> {
					using lhs_rep = typename Lhs::rep;
					using rhs_rep = typename Rhs::rep;
					using rep_op_result = _impl::op_result<OperationTag, lhs_rep,rhs_rep>;

					static const int exponent = rep_op_exponent<OperationTag,Lhs,Rhs>::value;

					using type = fixed_point<rep_op_result,exponent>;
				};

				template <class OperationTag,class Lhs,class Rhs>
				struct result<lean_tag,OperationTag,Lhs,Rhs> : result<raw_tag,OperationTag,Lhs,Rhs>{};

				template<class PolicyTag,class OperationTag,class Lhs,class Rhs>
				struct intermediate;

				// bare add/substrct
				template<class OperationTag,class Lhs,class Rhs>
				struct intermediate<lean_tag,OperationTag,Lhs,Rhs> {
					using _result = result<lean_tag,OperationTag,Lhs,Rhs>;
					using lhs_type = typename _result::type;
					using rhs_type = lhs_type;
				};




				//sg14::_impl::fp:arithmetic::operator_params	
				template<class PolicyTag,class OperationTag,class Lhs,class Rhs>
				struct operate_params {
					using _binary_pair = binary_pair<Lhs,Rhs>;
					using lhs_type = typename _binary_pair::lhs_type;
					using rhs_type = typename _binary_pair::rhs_type;

					using _intermediate = intermediate<PolicyTag,OperationTag,lhs_type,rhs_type>;
					
					using intermediate_lhs = typename _intermediate::lhs_type;
					using intermediate_rhs = typename _intermediate::rhs_type;

					using _result = result<PolicyTag,OperationTag,lhs_type,rhs_type>;

					using result_type = typename _result::type;
				};
			}

			// mappings from name function stragies to public API
			// strategy aliases for ease of flip-flopping
			using arithmetic_operator_tag = arithmetic::lean_tag;

			template <class PolicyTag, class Operation,class Lhs,class Rhs>
			const auto operate(const Lhs& lhs,const Rhs& rhs,Operation) 
			-> typename arithmetic::operate_params<PolicyTag,Operation,Lhs,Rhs>::result_type {
				using params = arithmetic::operate_params<PolicyTag,Operation,Lhs,Rhs>;
				using intermediate_lhs = typename params::intermediate_lhs;
				using intermediate_rhs = typename params::intermediate_rhs;
				using result_type = typename params::result_type;
				using result_rep = typename result_type::rep;

				auto lhs_data_type = static_cast<intermediate_lhs>(lhs);
				auto lhs_data = lhs_data_type.data();
				auto rhs_data_type = static_cast<intermediate_rhs>(rhs);
				auto rhs_data = rhs_data_type.data();

				auto result_data = static_cast<result_rep>(Operation()(lhs_data,rhs_data));

				return result_type::from_data(result_data);
			};
		}
	}
}
#endif
