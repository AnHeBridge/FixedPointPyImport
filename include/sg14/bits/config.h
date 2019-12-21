

//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SG14_CONFIG_H
#define SG14_CONFIG_H
////////////////////////////////////////////////////////////////////////////////
// SG14_EXCEPTIONS_ENABLED macro definition

#if defined(SG14_EXCEPTIONS_ENABLED)
#error SG14_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define SG14_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define SG14_EXCEPTIONS_ENABLED
#endif
#else
#define SG14_EXCEPTIONS_ENABLED
#endif

#endif // SG14_CONFIG_H
