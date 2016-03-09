// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)



#include <posixx/error.hpp> // posixx::error

#include <boost/test/unit_test.hpp>
#include <string>
#include <fcntl.h>

BOOST_AUTO_TEST_SUITE( error_suite )

BOOST_AUTO_TEST_CASE( constructor_test )
{

	// default constructor
	open("/non/existent", 0);
	posixx::error e1;
	BOOST_CHECK_EQUAL(e1.no, ENOENT);

	// "where" string constructor
	open("/non/existent", 0);
	posixx::error e2("open");
	BOOST_CHECK_EQUAL(e2.no, ENOENT);
	BOOST_CHECK_EQUAL(std::string(e2.what()).find("open: "), 0);

	// e1 and e2 should be the same appart from the "where" string
	BOOST_CHECK_EQUAL(std::string(e2.what()).substr(6), e1.what());

}

BOOST_AUTO_TEST_SUITE_END()

