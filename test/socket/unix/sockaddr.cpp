
#include "common.hpp" // PATH{1,2}, test_address{1,2}
#include <boost/test/unit_test.hpp> // unit testing stuff

BOOST_AUTO_TEST_CASE( socket_unix_sockaddr_test )
{
	BOOST_CHECK_EQUAL(test_address1.path(), PATH1);
	BOOST_CHECK_EQUAL(test_address2.path(), PATH2);
}

