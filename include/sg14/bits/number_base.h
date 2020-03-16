
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_number_base_H)
#define SG14_number_base_H 1

//#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/common.h>
#include <sg14/num_traits.h>
#include <sg14/bits/number_split.h>
#include <limits>
#include <type_traits>

namespace sg14 {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base {
        public:
            using rep = Rep ;//拆分成LowHigh的一个对象，该对象要定义基本的operator方法
            using _derived = Derived;

            number_base() = default;

            constexpr number_base(const rep& r)
                : _rep(r) { }

            //template<class T>
            //number_base& operator=(const T& r) {
            //    _rep = r;
            //    return static_cast<Derived&>(*this);
            //}

            explicit constexpr operator bool() const {
                return static_cast<bool>(_rep);
            }

            constexpr const rep data() const {
                return _rep.get_data();
            }

			constexpr const number_split<Rep> rep_data() const {
				return _rep;
			}

#if (__cplusplus >= 201402L)
            constexpr rep data() {
                return _rep.get_data();
            }
#endif

            static constexpr Derived from_data(const rep& r) {
                return Derived(r);
            }

        private:
            number_split<Rep> _rep;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_class_derived_from_number_base

        // true iff T's base class is sg14::_impl::number_base;
        // T must be a class;
        // used by sg14::_impl::is_derived_from_number_base
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_class_derived_from_number_base<
                Derived,
                enable_if_t<std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value>>
                : std::true_type {};

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_derived_from_number_base

        // true if T is the Derived parameter of a number_base type
        template<class T, class Enable = void>
        struct is_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_derived_from_number_base<Derived, enable_if_t<std::is_class<Derived>::value>>
        : is_class_derived_from_number_base<Derived> { };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::enable_if_precedes
        
        template<class Former, class Latter>
        struct precedes {
            static constexpr bool value =
                    (std::is_floating_point<Former>::value && !std::is_floating_point<Latter>::value)
                            || (is_derived_from_number_base<Former>::value &&
                                    !(is_derived_from_number_base<Latter>::value
                                            || std::is_floating_point<Latter>::value));
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::number_base operators

        // compound assignment
		
		
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator+=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs + rhs) {
            return lhs = lhs + rhs;
        }
		
		
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator-=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs - rhs) {
            return lhs = lhs - rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator*=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs * rhs) {
            return lhs = lhs * rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator/=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs / rhs) {
            return lhs = lhs / rhs;
        }
		
        // comparison operator
		
        template<class Lhs, class Rhs>
        constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, equal_tag)) {
            return operate(lhs, rhs, equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, not_equal_tag)) {
            return operate(lhs, rhs, not_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_tag)) {
            return operate(lhs, rhs, less_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_tag)) {
            return operate(lhs, rhs, greater_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_or_equal_tag)) {
            return operate(lhs, rhs, less_than_or_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_or_equal_tag)) {
            return operate(lhs, rhs, greater_than_or_equal_tag);
        }
		
	}
    ////////////////////////////////////////////////////////////////////////////////
    // _impl::number_base<> numeric traits

    template<class Number>
    struct is_composite<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
    };

    namespace _impl {
        template<class Number>
        struct get_rep;

        template<class Number>
        using get_rep_t = typename get_rep<Number>::type;

        // given a Number type and an alternative Rep type, make a new Number type
        // e.g. set_rep_t<fixed_point<int64_t, 42>, uint8_t> --> fixed_point<uint8_t, 42>
        template<class Number, class NewRep, class Enable = void>
        struct set_rep;

        template<class Number, class NewRep>
        using set_rep_t = typename set_rep<Number, NewRep>::type;
    }

    template<class Number>
    struct make_signed<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_signed_t<_impl::get_rep_t<Number>>>;
    };

    template<class Number>
    struct make_unsigned<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_unsigned_t<_impl::get_rep_t<Number>>>;
    };

    template<class Number>
    struct from_rep<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        template<class Rep>
        constexpr auto operator()(const Rep &rep) const -> Number {
            return Number::from_data(static_cast<typename Number::rep>(rep));
        }
    };

    template<class Number>
    struct to_rep<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        constexpr auto operator()(const typename Number::_derived& number) const
        -> decltype(number.data()){
            return number.data();
        }
    };

    template<class Derived, class Rep>
    struct scale<_impl::number_base<Derived, Rep>> {
        template<class Input>
        constexpr Rep operator()(const Input &i, int base, int exp) const {
            return (exp < 0)
                   ? _impl::to_rep(i) / _num_traits_impl::pow<Rep>(base, -exp)
                   : _impl::to_rep(i) * _num_traits_impl::pow<Rep>(base, exp);
        }
    };
}

#endif  // SG14_NUMBER_BASE_H
