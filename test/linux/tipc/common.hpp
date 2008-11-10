#ifndef TEST_LINUX_TIPC_COMMON_HPP_
#define TEST_LINUX_TIPC_COMMON_HPP_

#include <posixx/linux/tipc.hpp> // posixx::linux::tipc
#include <posixx/linux/tipc/print.hpp> // address output formatting

#define PTYPE 10000
#define INST1 10001
#define INST2 10002

// no need to clean addresses
#define clean_test_address(socket, addr)

static posixx::linux::tipc::sockaddr test_address1(
		posixx::linux::tipc::name(PTYPE, INST1));
static posixx::linux::tipc::sockaddr test_address2(
		posixx::linux::tipc::name(PTYPE, INST2));

#endif // TEST_LINUX_TIPC_COMMON_HPP_
