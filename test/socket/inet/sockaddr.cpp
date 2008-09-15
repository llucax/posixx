
#include "common.hpp" // IP, PORT{1,2}, test_address{1,2}
#include <posixx/socket/inet.hpp> // posixx::socket::inet
#include <boost/test/unit_test.hpp> // unit testing stuff

BOOST_AUTO_TEST_CASE( socket_inet_sockaddr_test )
{
	BOOST_CHECK_EQUAL(test_address1.addr(), IP);
	BOOST_CHECK_EQUAL(test_address1.port(), PORT1);
}

