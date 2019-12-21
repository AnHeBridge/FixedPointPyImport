
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_DEF_H)
#define SG14_FIXED_POINT_DEF_H 1

#include <sg14/bits/number_base.h>
#include <sg14/bits/type_traits.h>

// study group 14 of the c++ working group
namespace sg14 {
	// forward declaration
	template <class Rep = int, int Exponent = 0>
	class fixed_point;

	// implementation-specific definitions
	namespace _impl {
		namespace fp {
			// sg14::_impl::float_of_size 暂时跳过
		}
	}


    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile sg14/fixed_point
    ///
    /// \tparam Rep the underlying type used to represent the value
    /// \tparam Exponent the value by which to scale the integer value in order to get the real value
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 3 integer bits and 4 fractional bits:
    /// \snippet snippets.cpp define a fixed_point value
	
	template <class Rep, int Exponent>
	class fixed_point : public _impl::number_base<fixed_point<Rep,Exponent>,Rep> {
		using _base = _impl::number_base<fixed_point<Rep,Exponent>,Rep>;
	
	public :
		
		using rep = Rep;

		// constant
		
		// value of template parameter Exponent
		constexpr static int exponent = Exponent;
		
        /// number of binary digits this type can represent;
        /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
		constexpr static int digits = std::numeric_limits<Rep>::digits;
		
		// number of binary digits devoted to integer part of value;
		// can be negative for specialiations with especially small ranges
		constexpr static int integer_digits = digits + exponent;
		
		// number of binary digits devoted to fractional part of value
		// can be negative for specializations with especially large ranges
		constexpr static int fractional_digits = -exponent;
	
	private :
		// constructor taking representation explicitly using operator ++(int)-style trick
		fixed_point(rep r,int) : _base(r) { }
	
	public :
		// default constructor
		fixed_point() : _base() {}

		// constructor taking a fixed-point type
		template <class FromRep,int FromExponent>
		fixed_point(const fixed_point<FromRep,FromExponent>& rhs) : _base(fixed_point_to_rep(rhs)) { }
		
		template <class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer,int> Dummy = 0>
		fixed_point(const S& s) : fixed_point(fixed_point<S,0>::from_data(s)) {}

		static fixed_point from_data(rep const& r) {
			return fixed_point(r,0);
		}

		
		//constructor from a float-type
		template <class S,_impl::enable_if_t<std::numeric_limits<S>::is_iec559,int>Dummy = 0>
		fixed_point(S s) : _base(floating_point_to_rep(s)) {}

		//returns value represented as floating-point
		template<class S,_impl::enable_if_t<std::numeric_limits<S>::is_iec559,int>Dummy = 0>
		explicit constexpr operator S() const {
			return rep_to_floating_point<S>(_base::data());
		}
	
	private :
		template <class FromRep,int FromExponent>
		static rep fixed_point_to_rep(const fixed_point<FromRep,FromExponent>& rhs);

		template <class S>
		static rep floating_point_to_rep(S s);

		template <class S>
		static S rep_to_floating_point(rep r);

		template <class S,_impl::enable_if_t<std::numeric_limits<S>::is_iec559,int>Dummy = 0>
		static S one();

		template <class S>
		static S inverse_one();
	};
	
	
	namespace _impl {
		// sg14::_impl::is_fixed_point
		
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::shift_left

        // performs a shift operation by a fixed number of bits avoiding two pitfalls:
        // 1) shifting by a negative amount causes undefined behavior
        // 2) converting between integer types of different sizes can lose significant bits during shift right

        // Exponent == 0
		template <int exp,class Output, class Input>
		Output shift_left(Input i) {
			using larger = typename std::conditional<digits<Input>::value <= digits<Output>::value,Output,Input> :: type;

			return (exp >= -std::numeric_limits<larger>::digits) ? 
				static_cast<Output>(_impl::scale<larger>(static_cast<larger>(i),2,exp)) :
						Output{0};
		}

		namespace fp {
			namespace type {
				template <class S,int Exponent,enable_if_t<Exponent == 0,int> Dummy = 0>
				S pow2() {
					static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
					return S{1.};
				}

				template <class S,int Exponent,enable_if_t<!(Exponent <= 0) && (Exponent < 8),int> Dummy = 0>
				S pow2() {
					static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
					return pow2<S,Exponent - 1>() * S(2);
				}

				template <class S,int Exponent,enable_if_t<(Exponent >= 8),int>Dummy = 0>
				S pow2() {
					static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
					return pow2<S,Exponent - 8> * S(256);
				}


				template <class S,int Exponent,enable_if_t<!(Exponent >= 0) && (Exponent > -8),int> Dummy = 0>
				S pow2() {
					static_assert(std::numeric_limits<S>::is_iec559,"S must be floating-point type");
					return pow2<S,Exponent + 1>() * S(.5);
				}

                template<class S, int Exponent, enable_if_t<(Exponent<=-8), int> Dummy = 0>
                S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent+8>()*S(.003906250);
				}
			}
		}
	}


	template<class Rep,int Exponent>
	template<class FromRep,int FromExponent>
	typename fixed_point<Rep,Exponent>::rep fixed_point<Rep,Exponent>::fixed_point_to_rep(const fixed_point<FromRep,FromExponent>& rhs) {
		return _impl::shift_left<FromExponent - exponent,rep>(rhs.data());
	}

	template<class Rep,int Exponent>
	template<class S>
	typename fixed_point<Rep,Exponent>::rep fixed_point<Rep,Exponent>::floating_point_to_rep(S s) {
		static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
		return static_cast<rep>(s * one<S>());
	}

	template<class Rep,int Exponent>
	template<class S>
	S fixed_point<Rep,Exponent>::rep_to_floating_point(rep r) {
		static_assert(std::numeric_limits<S>::is_iec559,"S must be floating-point type");
		return S(r) * inverse_one<S>();
	}

	template<class Rep,int Exponent>
	template<class S,_impl::enable_if_t<std::numeric_limits<S>::is_iec559,int>Dummy>
	S fixed_point<Rep,Exponent>::one() {
		auto result = _impl::fp::type::pow2<S,-exponent>();
		return result;
	}

	template<class Rep,int Exponent>
	template<class S>
	S fixed_point<Rep,Exponent>::inverse_one() {
		static_assert(std::numeric_limits<S>::is_iec559,"S must be floating-point type");
		return _impl::fp::type::pow2<S,exponent>();
	}
}

#endif
