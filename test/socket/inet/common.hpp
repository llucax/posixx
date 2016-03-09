// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef TEST_SOCKET_IP_COMMON_HPP_
#define TEST_SOCKET_IP_COMMON_HPP_

#include <posixx/socket/inet.hpp> // posixx::socket::inet
#include <posixx/socket/inet/print.hpp> // address ostream formatting
#include <posixx/socket/opt.hpp> // posixx::socket::opt::REUSEADDR
#include <ostream> // std::ostream

#define IP "127.0.0.1"
#define PORT1 10001
#define PORT2 10002

static inline
void clean_test_address(posixx::socket::inet::socket& socket,
		const posixx::socket::inet::sockaddr& addr)
{
	// reuse the socket address (just in case)
	socket.opt< posixx::socket::opt::REUSEADDR >(true);
}

static posixx::socket::inet::sockaddr test_address1(IP, PORT1);
static posixx::socket::inet::sockaddr test_address2(IP, PORT2);

#endif // TEST_SOCKET_IP_COMMON_HPP_
