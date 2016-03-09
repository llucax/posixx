// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef POSIXX_SOCKET_INET_PRINT_HPP_
#define POSIXX_SOCKET_INET_PRINT_HPP_

#include "../inet.hpp" // posixx::socket::inet::sockaddr
#include <ostream> // std::ostream

inline
std::ostream& operator << (std::ostream& os,
		const posixx::socket::inet::sockaddr& sa) throw()
{
	return os << "inet::sockaddr(addr=" << sa.addr()
			<< ", port=" << sa.port() << ")";
}

#endif // POSIXX_SOCKET_INET_PRINT_HPP_
