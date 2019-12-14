
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions supporting generic treatment of numeric types

#if !defined(SG14_NUMERIC_TRAITS)
#define SG14_NUMERIC_TRAITS 1

#include <utility>


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
}

#endif

