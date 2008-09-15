
#include "common.hpp" // PTYPE, INST{1,2}, test_address{1,2}
#include <posixx/linux/tipc.hpp> // posixx::linux::tipc
#include <boost/test/unit_test.hpp> // unit testing stuff

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_test )
{
	BOOST_CHECK_EQUAL(test_address1.addr.name.name.type, PTYPE);
	BOOST_CHECK_EQUAL(test_address1.addr.name.name.instance, INST1);
	BOOST_CHECK_EQUAL(test_address2.addr.name.name.type, PTYPE);
	BOOST_CHECK_EQUAL(test_address2.addr.name.name.instance, INST2);
}

