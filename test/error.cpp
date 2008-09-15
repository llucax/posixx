
#include <posixx/error.hpp> // posixx::error

#include <boost/test/unit_test.hpp>
#include <string>
#include <unistd.h>

BOOST_AUTO_TEST_SUITE( error_suite )

BOOST_AUTO_TEST_CASE( constructor_test )
{

	// default constructor
	close(-123);
	posixx::error e1;
	BOOST_CHECK_EQUAL(e1.no, EBADF);

	// "where" string constructor
	close(-123);
	posixx::error e2("fopen");
	BOOST_CHECK_EQUAL(e2.no, EBADF);
	BOOST_CHECK_EQUAL(std::string(e2.what()).find("fopen: "), 0);

	// e1 and e2 should be the same appart from the "where" string
	BOOST_CHECK_EQUAL(std::string(e2.what()).substr(7), e1.what());

}

BOOST_AUTO_TEST_SUITE_END()

