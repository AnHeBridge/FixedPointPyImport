
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions supporting generic treatment of numeric types

#if !defined(SG14_NUMERIC_TRAITS)
#define SG14_NUMERIC_TRAITS 1

#include <utility>
#include <bits/limits.h>
#include <bits/type_traits.h>


namespace sg14 {

	using _digits_type = int;
	
	namespace _num_traits_impl {
		template <class T>
		using scale_result_type = decltype(std::declval<T>() * std::declval<T>());
	
		template <class T>
		scale_result_type<T> pown(int base,int exp) {
			return exp ? pown<T>(base,exp - 1) * static_cast<scale_result_type<T>>(base)
				: static_cast<scale_result_type<T>>(1);
		}

		template <class T>
		scale_result_type<T> pow2(int exp) {
			return scale_result_type<T>{1} << exp;
		}


		template <class T>
		scale_result_type<T> pow(int base,int exp) {
			return (base == 2) ? pow2<T>(exp) : pown<T>(base,exp);
		}

		template<_digits_type MinNumDigits,class Smaller,class T>
		struct enable_for_range : std::enable_if<MinNumDigits <= std::numeric_limits<T>::digits && 
					std::numeric_limits<Smaller>::digits < MinNumDigits> {};

		template<_digits_type MinNumDigits,class Smallest>
		struct enable_for_range<MinNumDigits,void ,Smallest> : std::enable_if<MinNumDigits <= std::numeric_limits<Smallest>::digits> {};

		template<_digits_type MinNumDigits,class Smaller,class T>
		using enable_for_range_t = typename enable_for_range<MinNumDigits,Smaller,T>::type;

		//sg14::_num_traits_impl::set_digits_signed
		template<_digits_type MinNumDigits,class Enable = void>
		struct set_digits_signed;

		template<_digits_type MinNumDigits>
		struct set_digits_signed<MinNumDigits,enable_for_range_t<MinNumDigits,void,std::int8_t>> {
			using type = std::int8_t;
		};

        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int8_t, std::int16_t>> {
            using type = std::int16_t;
        };

        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int16_t, std::int32_t>> {
            using type = std::int32_t;
        };

        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int32_t, std::int64_t>> {
            using type = std::int64_t;
		};

        template<_digits_type MinNumDigits, class Enable = void>
        struct set_digits_unsigned;

        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, void, std::uint8_t>> {
            using type = std::uint8_t;
        };

        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint8_t, std::uint16_t>> {
            using type = std::uint16_t;
        };

        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint16_t, std::uint32_t>> {
            using type = std::uint32_t;
        };

        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint32_t, std::uint64_t>> {
            using type = std::uint64_t;
        };

		template<class Integer,_digits_type MinNumDigits>
		using set_digits_integer = typename std::conditional<
		std::numeric_limits<Integer>::is_signed,
		set_digits_signed<MinNumDigits>,
		set_digits_unsigned<MinNumDigits> >::type;
		
	}

	// digits
	template <class T,class Enable = void>
	struct digits: std::integral_constant<_digits_type,std::numeric_limits<T>::digits> {
	};

	// sg14::from_rep
	template<class Number, class Enable = void>
	struct from_rep {
		template<class Rep>
		Number operator() (const Rep& rep) const {
			//by default , anumber type's rep type is the number type itself
			return static_cast<Number>(rep);
		}
	};

	template<class T,_digits_type Digits,class Enable = void>
	struct set_digits;

	template<class T,_digits_type Digits>
	struct set_digits<T,Digits,_impl::enable_if_t<std::is_integral<T>::value>> :
	 _num_traits_impl::set_digits_integer<T,Digits> {};

	template<class T,_digits_type Digits>
	using set_digits_t = typename set_digits<T,Digits>::type;

	template<class,class = void>
	struct make_signed;

	template<class T>
	struct make_signed<T,_impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {};

	template<class T>
	using make_signed_t = typename make_signed<T>::type;

	template<class, class = void>
	struct make_unsigned;

	template<class T>
	struct make_unsigned<T,_impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> { };

	template<class T>
	using make_unsigned_t = typename make_unsigned<T>::type;

	namespace _impl {
		template <class Number,class Rep>
		const auto from_rep(const Rep& rep) -> decltype(sg14::from_rep<Number>()(rep)) {
			return sg14::from_rep<Number>()(rep);
		}
	}

	template<class Number,class Enable = void>
	struct to_rep {
		Number operator() (const Number& number) const {
			// by default ,the rep type of a number type is the number type itself
			return number;
		}
	};

	namespace _impl {
		template <class Number,class Enable = void>
		auto to_rep(const Number& number) -> decltype(sg14::to_rep<Number>()(number)) {
			auto result = sg14::to_rep<Number>()(number);
			return result;
		}
	}

	template <class T>
	struct scale {
		auto operator() (const T& i ,int base,int exp) const 
			-> _num_traits_impl::scale_result_type<T> {
				return _impl::from_rep<_num_traits_impl::scale_result_type<T>> (
					(exp < 0) ?
						_impl::to_rep<T>(i) / _num_traits_impl::pow<T>(base,-exp) :
						_impl::to_rep<T>(i) * _num_traits_impl::pow<T>(base,exp));
			}
	};

	namespace _impl {
		template <class T>
		auto scale(const T& i,int base,int exp) -> decltype(scale<T>()(i,base,exp)) {
			return sg14::scale<T>()(i,base,exp);
		}
	}

	namespace _impl {
		template<class T,bool isSigned = true>
		struct make_signed;

		template<class T>
		struct make_signed<T,true> : ::sg14::make_signed<T> {};

		template<class T>
		struct make_signed<T,false> : ::sg14::make_unsigned<T> {};

		template<class T,bool isSigned>
		using make_signed_t = typename make_signed<T,isSigned>::type;
	}
}

#endif

