// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef TEST_SOCKET_UNIX_COMMON_HPP_
#define TEST_SOCKET_UNIX_COMMON_HPP_

#include <posixx/socket/unix.hpp> // posixx::socket::unix
#include <posixx/socket/unix/print.hpp> // address ostream formatting
#include <boost/test/unit_test.hpp> // unit testing stuff
#include <ostream> // std::ostream
#include <unistd.h> // unlink
#include <sys/stat.h> // struct stat

#define PATH "/tmp/posixx_socket_unix_test"
#define PATH1 PATH "1"
#define PATH2 PATH "2"

static inline
void clean_test_address(posixx::socket::unix::socket& socket,
		const posixx::socket::unix::sockaddr& addr)
{
	// remove the socket file, if exists
	struct stat st;
	if (stat(addr.sun_path, &st) == 0)
		BOOST_REQUIRE_EQUAL(unlink(addr.sun_path), 0);
}

static posixx::socket::unix::sockaddr test_address1(PATH1);
static posixx::socket::unix::sockaddr test_address2(PATH2);

#endif // TEST_SOCKET_UNIX_COMMON_HPP_
