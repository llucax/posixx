#ifndef POSIXX_LINUX_TIPC_PRINT_HPP_
#define POSIXX_LINUX_TIPC_PRINT_HPP_

#include "../tipc.hpp" // posixx::linux::tipc::sockaddr
#include <ostream> // std::ostream

inline
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

#endif // POSIXX_LINUX_TIPC_PRINT_HPP_
