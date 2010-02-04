
#include <posixx/buffer.hpp> // buffer
#include <ostream> // std::ostream
#include <iomanip> // std::hex, setfill, setw

// declared here so boost.Test can see it
std::ostream& operator << (std::ostream& os, const posixx::buffer& b);

#include <boost/test/unit_test.hpp>

using posixx::buffer;

std::ostream& operator << (std::ostream& os, const posixx::buffer& b)
{
	os << "buffer(size=" << b.size() << ", data=";
	for (posixx::buffer::const_iterator i = b.begin(); i != b.end(); ++i)
		os << std::hex << std::setfill('0') << std::setw(2) << *i;
	return os << ")";
}

// buffer iterators are char*, so they are printed as C-style string, leading
// to memory problems because they are not usually null-terminated.
BOOST_TEST_DONT_PRINT_LOG_VALUE( buffer ) // FIXME
BOOST_TEST_DONT_PRINT_LOG_VALUE( buffer::iterator )
BOOST_TEST_DONT_PRINT_LOG_VALUE( buffer::const_iterator )
BOOST_TEST_DONT_PRINT_LOG_VALUE( buffer::reverse_iterator )
BOOST_TEST_DONT_PRINT_LOG_VALUE( buffer::const_reverse_iterator )

// avoid compilation warning concerning unused variables
void dummy(buffer::const_iterator i) {}

BOOST_AUTO_TEST_SUITE( buffer_suite )

BOOST_AUTO_TEST_CASE( types_test )
{
	buffer::value_type v = 1;
	buffer::reference r = v;
	BOOST_CHECK_EQUAL(r, 1);
	r = 2;
	BOOST_CHECK_EQUAL(v, 2);
	buffer::const_reference cr = v;
	BOOST_CHECK_EQUAL(cr, 2);
	buffer b;
	buffer::iterator i = b.end();
	buffer::const_iterator ci = i;
	dummy(ci);
	buffer::size_type s = 5;
	++s;
	buffer::pointer p = &v;
	BOOST_CHECK_EQUAL(*p, 2);
	*p = 3;
	BOOST_CHECK_EQUAL(v, 3);
	BOOST_CHECK_EQUAL(r, 3);
	BOOST_CHECK_EQUAL(cr, 3);
	buffer::const_pointer cp = p;
	BOOST_CHECK_EQUAL(*cp, 3);
	buffer::difference_type d = p - cp;
	BOOST_CHECK_EQUAL(d, 0);
}

BOOST_AUTO_TEST_CASE( default_constructor_test )
{
	buffer b;
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() == b.end());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(!b.c_array());
}

BOOST_AUTO_TEST_CASE( const_default_constructor_test )
{
	const buffer b;
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() == b.end());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(!b.c_array());
}

BOOST_AUTO_TEST_CASE( n_constructor_test )
{
	buffer b(100);
	BOOST_CHECK_EQUAL(b.size(), 100);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	b[0] = b[b.size()-1];
	b.at(1) = b.at(b.size()-2);
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
}

BOOST_AUTO_TEST_CASE( const_n_constructor_test )
{
	const buffer b(100);
	BOOST_CHECK_EQUAL(b.size(), 100);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	buffer::value_type x = b[50];
	++x;
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (buffer::const_iterator i = b.begin(); i != b.end(); ++i);
}

BOOST_AUTO_TEST_CASE( default_n_copy_constructor_test )
{
	buffer b(100, 0x5f);
	BOOST_CHECK_EQUAL(b.size(), 100);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	buffer::value_type x = b[50];
	++x;
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (buffer::iterator i = b.begin(); i != b.end(); ++i)
		BOOST_CHECK_EQUAL(*i, 0x5f);
	b[b.size()-1] = 0xf5;
	b[0] = b[b.size()-1];
	BOOST_CHECK_EQUAL(b[b.size()-1], 0xf5);
	BOOST_CHECK_EQUAL(b.back(), 0xf5);
	BOOST_CHECK_EQUAL(*(b.end()-1), 0xf5);
	BOOST_CHECK_EQUAL(b[0], 0xf5);
	BOOST_CHECK_EQUAL(b.front(), 0xf5);
	BOOST_CHECK_EQUAL(*b.begin(), 0xf5);
	b.at(b.size()-2) = 0xff;
	b.at(1) = b.at(b.size()-2);
	BOOST_CHECK_EQUAL(b[b.size()-2], 0xff);
	BOOST_CHECK_EQUAL(*(b.end()-2), 0xff);
	BOOST_CHECK_EQUAL(b[1], 0xff);
	BOOST_CHECK_EQUAL(*(b.begin()+1), 0xff);
}

BOOST_AUTO_TEST_CASE( copy_constructor_test )
{
	buffer b(2);
	b[0] = 56;
	b[1] = 123;
	const buffer bb(b);
	BOOST_CHECK_EQUAL(b.size(), bb.size());
	BOOST_CHECK_EQUAL(b.capacity(), bb.capacity());
	BOOST_CHECK_EQUAL(b.empty(), bb.empty());
	BOOST_CHECK_EQUAL(b.at(0), bb.at(0));
	BOOST_CHECK_EQUAL(b.at(1), bb.at(1));
}

BOOST_AUTO_TEST_CASE( iterator_constructor_test )
{
	buffer::value_type a[5] = { 5, 6, 7, 8, 9 };
	buffer b(a, a + 5);
	BOOST_CHECK_EQUAL(b.size(), 5);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (int i = 0; i != 5; ++i)
		BOOST_CHECK_EQUAL(b.at(i), i + 5);
}

BOOST_AUTO_TEST_CASE( const_iterator_constructor_test )
{
	buffer::value_type a[5] = { 5, 6, 7, 8, 9 };
	const buffer b(a, a + 5);
	BOOST_CHECK_EQUAL(b.size(), 5);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (int i = 0; i != 5; ++i)
		BOOST_CHECK_EQUAL(b.at(i), i + 5);
}

BOOST_AUTO_TEST_CASE( operator_equals_test )
{
	buffer::value_type a[] = { 56, 123 };
	const buffer b(a, a + 2);
	buffer bb;
	bb = b;
	BOOST_CHECK_EQUAL(b.size(), bb.size());
	BOOST_CHECK_EQUAL(b.capacity(), bb.capacity());
	BOOST_CHECK_EQUAL(b.empty(), bb.empty());
	BOOST_CHECK_EQUAL(b.at(0), bb.at(0));
	BOOST_CHECK_EQUAL(b.at(1), bb.at(1));
	buffer::pointer p = bb.c_array();
	bb = bb;
	BOOST_CHECK_EQUAL(bb.c_array(), p);
	BOOST_CHECK_EQUAL(b.size(), 2);
}

BOOST_AUTO_TEST_CASE( iterators_test )
{
	buffer::value_type a[] = { 56, 123, 253 };
	buffer b(a, a + 3);
	BOOST_CHECK_NE(b.begin(), b.end());
	BOOST_CHECK_LT(b.begin(), b.end());
	buffer::iterator i = b.begin();
	BOOST_CHECK_EQUAL(*i, 56);
	i++;
	BOOST_CHECK_EQUAL(*i, 123);
	++i;
	BOOST_CHECK_EQUAL(*i, 253);
	i += 1;
	BOOST_CHECK_EQUAL(i, b.end());
	i--;
	BOOST_CHECK_EQUAL(*i, 253);
	--i;
	BOOST_CHECK_EQUAL(*i, 123);
	i -= 1;
	BOOST_CHECK_EQUAL(*i, 56);
	i++;
	BOOST_CHECK_EQUAL(*(i + 1), 253);
	BOOST_CHECK_EQUAL(*(i - 1), 56);
	BOOST_CHECK_EQUAL(i - 1, b.begin());
	BOOST_CHECK_EQUAL(i + 2, b.end());
	BOOST_CHECK_EQUAL(i - b.begin(), 1);
	*i = 77;
	BOOST_CHECK_EQUAL(*i, 77);
	BOOST_CHECK_EQUAL(*(b.begin() + 1), 77);
}

BOOST_AUTO_TEST_CASE( reverse_iterators_test )
{
	buffer::value_type a[] = { 56, 123, 253 };
	buffer b(a, a + 3);
	BOOST_CHECK_NE(b.rbegin(), b.rend());
	BOOST_CHECK_LT(b.rbegin(), b.rend());
	buffer::reverse_iterator i = b.rbegin();
	BOOST_CHECK_EQUAL(*i, 253);
	i++;
	BOOST_CHECK_EQUAL(*i, 123);
	++i;
	BOOST_CHECK_EQUAL(*i, 56);
	i += 1;
	BOOST_CHECK_EQUAL(i, b.rend());
	i--;
	BOOST_CHECK_EQUAL(*i, 56);
	--i;
	BOOST_CHECK_EQUAL(*i, 123);
	i -= 1;
	BOOST_CHECK_EQUAL(*i, 253);
	i++;
	BOOST_CHECK_EQUAL(*(i + 1), 56);
	BOOST_CHECK_EQUAL(*(i - 1), 253);
	BOOST_CHECK_EQUAL(i - 1, b.rbegin());
	BOOST_CHECK_EQUAL(i + 2, b.rend());
	BOOST_CHECK_EQUAL(i - b.rbegin(), 1);
	*i = 77;
	BOOST_CHECK_EQUAL(*i, 77);
	BOOST_CHECK_EQUAL(*(b.rbegin() + 1), 77);
}

BOOST_AUTO_TEST_CASE( resize_test )
{
	buffer b;
	BOOST_CHECK(!b.c_array());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	b.resize(10);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 10);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	b[9] = 1;
	BOOST_CHECK_EQUAL(b[9], 1);
	BOOST_CHECK_EQUAL(b.at(9), 1);
	BOOST_CHECK_EQUAL(b.c_array()[9], 1);
	b.resize(15, 0x77);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 15);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK_EQUAL(b[9], 1);
	BOOST_CHECK_EQUAL(b.at(9), 1);
	BOOST_CHECK_EQUAL(b.c_array()[9], 1);
	for (buffer::const_iterator i = b.begin()+10; i != b.end(); ++i)
		BOOST_CHECK_EQUAL(*i, 0x77);
	b.resize(0);
	BOOST_CHECK(!b.c_array());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
}

BOOST_AUTO_TEST_CASE( reserve_test )
{
	buffer b;
	b.reserve(10);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 10);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	b[9] = 1;
	BOOST_CHECK_EQUAL(b[9], 1);
	BOOST_CHECK_EQUAL(b.at(9), 1);
	BOOST_CHECK_EQUAL(b.c_array()[9], 1);
	b.reserve(5);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 10);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK_EQUAL(b[9], 1);
	b.reserve(20);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 20);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK_EQUAL(b[9], 1);
	b.reserve(0);
	BOOST_CHECK(b.c_array());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_EQUAL(b.size(), 20);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK_EQUAL(b[9], 1);
}

BOOST_AUTO_TEST_CASE( assign_iterator_test )
{
	buffer::value_type a[5] = { 5, 6, 7, 8, 9 };
	buffer b;
	BOOST_CHECK(!b.c_array());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	b.assign(a, a + 5);
	BOOST_CHECK_EQUAL(b.size(), 5);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (int i = 0; i != 5; ++i)
		BOOST_CHECK_EQUAL(b.at(i), i + 5);
}

BOOST_AUTO_TEST_CASE( assign_n_copy_test )
{
	buffer b(100, 0x5f);
	b.assign(5, 0x33);
	BOOST_CHECK_EQUAL(b.size(), 5);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
	BOOST_CHECK(!b.empty());
	BOOST_CHECK_LT(b.size(), b.max_size());
	BOOST_CHECK(b.begin() != b.end());
	BOOST_CHECK_EQUAL(b.end() - b.begin(), b.size());
	//BOOST_CHECK_EQUAL(b.rbegin(), b.rend());
	buffer::value_type x = b[2];
	++x;
	BOOST_CHECK_THROW(b.at(b.size()), std::out_of_range);
	BOOST_CHECK(b.c_array());
	for (buffer::iterator i = b.begin(); i != b.end(); ++i)
		BOOST_CHECK_EQUAL(*i, 0x33);
	b[b.size()-1] = 0x66;
	b[0] = b[b.size()-1];
	BOOST_CHECK_EQUAL(b[b.size()-1], 0x66);
	BOOST_CHECK_EQUAL(b.back(), 0x66);
	BOOST_CHECK_EQUAL(*(b.end()-1), 0x66);
	BOOST_CHECK_EQUAL(b[0], 0x66);
	BOOST_CHECK_EQUAL(b.front(), 0x66);
	BOOST_CHECK_EQUAL(*b.begin(), 0x66);
	b.at(b.size()-2) = 0xff;
	b.at(1) = b.at(b.size()-2);
	BOOST_CHECK_EQUAL(b[b.size()-2], 0xff);
	BOOST_CHECK_EQUAL(*(b.end()-2), 0xff);
	BOOST_CHECK_EQUAL(b[1], 0xff);
	BOOST_CHECK_EQUAL(*(b.begin()+1), 0xff);
}

BOOST_AUTO_TEST_CASE( clear_test )
{
	buffer b(10);
	b.clear();
	BOOST_CHECK(!b.c_array());
	BOOST_CHECK(b.empty());
	BOOST_CHECK_EQUAL(b.size(), 0);
	BOOST_CHECK_EQUAL(b.capacity(), b.size());
}

BOOST_AUTO_TEST_CASE( operators_test )
{
	const buffer b1(5, 78);
	const buffer b2(5, 78);
	buffer bb(5, 78);
	bb[4] = 79;
	const buffer b3 = bb;
	bb[4] = 77;
	const buffer b4 = bb;
	const buffer b5(4, 78);
	const buffer b6;
	bb.clear();
	const buffer b7 = bb;
	BOOST_CHECK_EQUAL(b1, b1);
	BOOST_CHECK_EQUAL(b1, b2);
	BOOST_CHECK_LE(b1, b2);
	BOOST_CHECK_GE(b1, b2);
	BOOST_CHECK_NE(b1, b3);
	BOOST_CHECK_NE(b1, b4);
	BOOST_CHECK_NE(b1, b5);
	BOOST_CHECK_NE(b1, b6);
	BOOST_CHECK_GT(b1, b4);
	BOOST_CHECK_GE(b1, b4);
	BOOST_CHECK_LT(b1, b3);
	BOOST_CHECK_LE(b1, b3);
	BOOST_CHECK_GT(b1, b6);
	BOOST_CHECK_LT(b6, b2);
	BOOST_CHECK_GE(b3, b6);
	BOOST_CHECK_LE(b6, b4);
	BOOST_CHECK_EQUAL(b6, b6);
	BOOST_CHECK_EQUAL(b6, b7);
	BOOST_CHECK_GE(b6, b7);
	BOOST_CHECK_LE(b6, b7);
}

BOOST_AUTO_TEST_CASE( swap_test )
{
	buffer::value_type a1[5] = { 5, 6, 7, 8, 9 };
	buffer::value_type a2[3] = { 55, 77, 99 };
	const buffer b1c(a1, a1 + 5);
	const buffer b2c(a2, a2 + 3);
	buffer b1 = b1c;
	buffer b2 = b2c;
	b1.swap(b2);
	BOOST_CHECK_EQUAL(b1, b2c);
	BOOST_CHECK_EQUAL(b2, b1c);
	posixx::swap(b2, b1);
	BOOST_CHECK_EQUAL(b1, b1c);
	BOOST_CHECK_EQUAL(b2, b2c);
}

BOOST_AUTO_TEST_SUITE_END()

