

//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#if !defined(SG14_number_base_H)
#define SG14_number_base_H 1

#include <limits>
#include <sg14/num_traits.h>

namespace sg14 {
	namespace _impl {
		template <class Derived, class Rep>
		class number_base {
		public :
			using rep = Rep;
			using _derived = Derived;

			number_base() = default;
			
			constexpr number_base(const rep&r) : _rep(r) {}

			template <class T>
			number_base& operator = (const T& r) {
				if (&r == this) 
					return static_cast<Derived&>(*this);
				_rep = r;
				return static_cast<Derived&>(*this);
			}

			explicit constexpr operator bool() const {
				return static_cast<bool>(_rep);
			}

			constexpr const rep& data() const {
				return _rep;
			}

			static constexpr Derived from_data(const rep& r) {
				return Derived(r);
			}

		private :
			rep _rep;
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
        // sg14::_impl::number_base operators

        // compound assignment

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator+=(Lhs& lhs, const Rhs& rhs)
	    -> decltype(lhs = lhs + rhs)
        {
			return lhs = lhs + rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator-=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs - rhs)
        {
            return lhs = lhs - rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator*=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs * rhs)
        {
            return lhs = lhs * rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator/=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs / rhs)
        {
            return lhs = lhs / rhs;
        }
	}
}



			

#endif
