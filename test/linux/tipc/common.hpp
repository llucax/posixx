#ifndef TEST_LINUX_TIPC_COMMON_HPP_
#define TEST_LINUX_TIPC_COMMON_HPP_

#include <posixx/linux/tipc.hpp> // posixx::linux::tipc
#include <ostream> // std::ostream

#define PTYPE 10000
#define INST1 10001
#define INST2 10002

static inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::sockaddr& sa) throw()
{
	using posixx::linux::tipc::sockaddr;
	os << "tipc::sockaddr(scope=" << unsigned(sa.scope) << ", ";
	switch (sa.type())
	{
	case sockaddr::ID:
		os << "id(ref=" << sa.addr.id.ref
				<< ", node=" << sa.addr.id.node << ")";
		break;
	case sockaddr::NAME:
		os << "name(type=" << sa.addr.name.name.type
				<< ", instance=" << sa.addr.name.name.instance
				<< ", domain=" << sa.addr.name.domain << ")";
		break;
	case sockaddr::NAMESEQ:
		os << "nameseq(type=" << sa.addr.nameseq.type
				<< ", lower=" << sa.addr.nameseq.lower
				<< ", upper=" << sa.addr.nameseq.upper << ")";
		break;
	default:
		os << "UNKNOWN!";
	}
	return os << ")";
}

// no need to clean addresses
#define clean_test_address(socket, addr)

static posixx::linux::tipc::sockaddr test_address1(
		posixx::linux::tipc::name(PTYPE, INST1));
static posixx::linux::tipc::sockaddr test_address2(
		posixx::linux::tipc::name(PTYPE, INST2));

#endif // TEST_LINUX_TIPC_COMMON_HPP_
