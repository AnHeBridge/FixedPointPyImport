
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `sg14::fixed_point` type;
/// definitions that straddle two homes, e.g. fixed_point and cmath, traits or limits;
/// included from sg14/fixed_point - do not include directly!

#if !defined(SG14_FIXED_POINT_EXTRAS_H)
#define SG14_FIXED_POINT_EXTRAS_H 1

#include "fixed_point_type.h"

#include <cmath>
#include <istream>

namespace sg14 {
	//sg14::fixed_point streaming - placeholder implementation
	template<class Rep,int Exponent>
	::std::ostream& operator << (::std::ostream& out,const fixed_point<Rep,Exponent>&fp) {
		return out << static_cast<long double>(fp);
	}

	template<class Rep,int Exponent>
	::std::istream& operator>>(::std::istream& in,fixed_point<Rep,Exponent>& fp) {
		long double ld;
		in >> ld;
		fp = ld;
		return in;
	}

    template<class Rep, int Exponent>
        struct digits<fixed_point<Rep, Exponent>> : digits<Rep> {
    };

    template<class Rep, int Exponent, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent>;
    };

    template<class Rep, int Exponent, class Value>
    struct from_value<fixed_point<Rep, Exponent>, Value> {
        using type = fixed_point<Value>;
	};

	template<class Rep, int Exponent>
	fixed_point<Rep,Exponent> sqrt(const fixed_point<Rep,Exponent>& src) {
		Rep data = src.data();
		if (src <= 0) 
			return fixed_point<Rep, Exponent>::from_data(0);
		Rep result = 0;
		Rep bit = static_cast<Rep>(1) << ((std::numeric_limits<Rep>::digits + std::numeric_limits<Rep>::is_signed) - 2);

		while (bit > data) 
			bit >>= 2;

		while(bit != 0) {
			if (data >= result + bit) {
				data -= result + bit;
				result = (result >> 1) + bit;
			}
			else
				result >>= 1;
			bit >>= 2;
		}
		using highrep = typename _num_traits_impl::set_digits_integer<Rep,std::numeric_limits<Rep>::digits / 2>::type;
		return static_cast<fixed_point<Rep,Exponent>>(fixed_point<highrep, Exponent / 2>::from_data(result));
	}

}


#endif
