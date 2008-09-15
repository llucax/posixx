#ifndef POSIXX_STATIC_ASSERT_HPP_
#define POSIXX_STATIC_ASSERT_HPP_

template <bool x> struct POSIXX_STATIC_ASSERTION_FAILURE;
template <> struct POSIXX_STATIC_ASSERTION_FAILURE<true> { enum {value = 1}; };
#define static_assert( B, M ) \
		enum { static_assert_enum_ ## __LINE__ \
			= sizeof(POSIXX_STATIC_ASSERTION_FAILURE< (bool)( B ) >) }

#endif // POSIXX_STATIC_ASSERT_HPP_
