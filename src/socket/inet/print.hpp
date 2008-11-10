#ifndef POSIXX_SOCKET_INET_PRINT_HPP_
#define POSIXX_SOCKET_INET_PRINT_HPP_

#include "../inet.hpp" // posixx::socket::inet::sockaddr
#include <ostream> // std::ostream

inline
std::ostream& operator << (std::ostream& os,
		const posixx::socket::inet::sockaddr& sa) throw()
{
	return os << "inet::sockaddr(port=" << ntohs(sa.sin_port)
			<< ", addr=" << inet_ntoa(sa.sin_addr) << ")";
}

#endif // POSIXX_SOCKET_INET_PRINT_HPP_
