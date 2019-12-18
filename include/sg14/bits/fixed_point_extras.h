
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
}


#endif
