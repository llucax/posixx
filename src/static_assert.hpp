// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef POSIXX_STATIC_ASSERT_HPP_
#define POSIXX_STATIC_ASSERT_HPP_

template <bool x> struct POSIXX_STATIC_ASSERTION_FAILURE;
template <> struct POSIXX_STATIC_ASSERTION_FAILURE<true> { enum {value = 1}; };
#define static_assert( B, M ) \
		enum { static_assert_enum_ ## __LINE__ \
			= sizeof(POSIXX_STATIC_ASSERTION_FAILURE< (bool)( B ) >) }

#endif // POSIXX_STATIC_ASSERT_HPP_
