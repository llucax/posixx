#ifndef TEST_SOCKET_IP_COMMON_HPP_
#define TEST_SOCKET_IP_COMMON_HPP_

#include <posixx/socket/inet.hpp> // posixx::socket::inet
#include <posixx/socket/opt.hpp> // posixx::socket::opt::REUSEADDR
#include <ostream> // std::ostream

#define IP "127.0.0.1"
#define PORT1 10001
#define PORT2 10002

static inline
std::ostream& operator << (std::ostream& os,
		const posixx::socket::inet::sockaddr& sa) throw()
{
	return os << "inet::sockaddr(family=" << sa.sin_family
		<< ", port=" << ntohs(sa.sin_port)
		<< ", addr=" << inet_ntoa(sa.sin_addr) << ")";
}

static inline
void clean_test_address(posixx::socket::inet::socket& socket,
		const posixx::socket::inet::sockaddr& addr)
{
	// reuse the socket address (just in case)
	socket.opt< posixx::socket::opt::REUSEADDR >(true);
}

static posixx::socket::inet::sockaddr test_address1(PORT1, IP);
static posixx::socket::inet::sockaddr test_address2(PORT2, IP);

#endif // TEST_SOCKET_IP_COMMON_HPP_
