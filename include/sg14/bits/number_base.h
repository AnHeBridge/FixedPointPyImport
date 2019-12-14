

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
	}
}



			

#endif
