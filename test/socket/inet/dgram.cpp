// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)



#include "common.hpp" // test_address1, test_address2 (for the generic test)
#include <boost/test/unit_test.hpp> // unit testing stuff
#include "../../socket/generic_test_includes.hpp" // (for the generic test)

BOOST_AUTO_TEST_SUITE( socket_inet_dgram_suite )

#define TEST_DGRAM
#define TEST_NS ::posixx::socket::inet
#define TEST_PF_INET
#define TEST_PROTOCOL 0
#define TEST_CHECK_ADDR
#include "../generic_test.hpp"

BOOST_AUTO_TEST_SUITE_END()

