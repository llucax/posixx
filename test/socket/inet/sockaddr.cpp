
#include "common.hpp" // IP, PORT{1,2}, test_address{1,2}
#include <posixx/socket/inet.hpp> // posixx::socket::inet
#include <boost/test/unit_test.hpp> // unit testing stuff

namespace inet = posixx::socket::inet;

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_test )
{
	BOOST_CHECK_EQUAL(test_address1.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(test_address1.addr(), IP);
	BOOST_CHECK_EQUAL(test_address1.port(), PORT1);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_default )
{
	inet::sockaddr addr;
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::any);
	BOOST_CHECK_EQUAL(addr.sin_port, 0);
	BOOST_CHECK_EQUAL(addr.port(), 0);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_broadcast )
{
	inet::sockaddr addr(inet::broadcast);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::broadcast);
	BOOST_CHECK_EQUAL(addr.sin_port, 0);
	BOOST_CHECK_EQUAL(addr.port(), 0);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_broadcast_port )
{
	inet::sockaddr addr(inet::broadcast, 12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::broadcast);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_1_1_1_1_8080 )
{
	inet::sockaddr addr("1.1.1.1", 8080);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.addr(), "1.1.1.1");
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, 0x01010101);
	BOOST_CHECK_EQUAL(addr.port(), 8080);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_16_16_16_16_12345 )
{
	inet::sockaddr addr("16.16.16.16", 12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.addr(), "16.16.16.16");
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, 0x10101010);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_broadcast_port )
{
	inet::sockaddr addr;
	addr.set(inet::broadcast, 12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::broadcast);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_16_16_16_16_12345 )
{
	inet::sockaddr addr;
	addr.set("16.16.16.16", 12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.addr(), "16.16.16.16");
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, 0x10101010);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_port_broadcast )
{
	inet::sockaddr addr(inet::broadcast);
	addr.port(12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::broadcast);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_port_16_16_16_16_12345 )
{
	inet::sockaddr addr("16.16.16.16", 0);
	addr.port(12345);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.addr(), "16.16.16.16");
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, 0x10101010);
	BOOST_CHECK_EQUAL(addr.port(), 12345);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_addr_broadcast )
{
	inet::sockaddr addr;
	addr.addr(inet::broadcast);
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, inet::broadcast);
	BOOST_CHECK_EQUAL(addr.port(), 0);
}

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_set_addr_16_16_16_16 )
{
	inet::sockaddr addr;
	addr.addr("16.16.16.16");
	BOOST_CHECK_EQUAL(addr.sin_family, AF_INET);
	BOOST_CHECK_EQUAL(addr.addr(), "16.16.16.16");
	BOOST_CHECK_EQUAL(addr.sin_addr.s_addr, 0x10101010);
	BOOST_CHECK_EQUAL(addr.port(), 0);
}

