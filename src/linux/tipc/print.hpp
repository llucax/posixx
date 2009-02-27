#ifndef POSIXX_LINUX_TIPC_PRINT_HPP_
#define POSIXX_LINUX_TIPC_PRINT_HPP_

#include "../tipc.hpp" // posixx::linux::tipc::sockaddr
#include <ostream> // std::ostream

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::addr& a) throw()
{
	return os << a.zone() << "." << a.cluster() << "." << a.node();
}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::portid& p) throw()
{
	return os << "portid(" << p.ref << ", " << p.node
			<< " [" << p.node_addr() << "])";
}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::name& n) throw()
{
	return os << "name(" << n.type << ", " << n.instance << ")";
}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::nameseq& ns) throw()
{
	return os << "nameseq(" << ns.type << ", " << ns.lower << ", "
			<< ns.upper << ")";
}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::subscr& s) throw()
{
	using namespace posixx::linux::tipc;

	bool has_filter = false;

	os << "subscr(" << s.name_seq() << ", " << s.timeout << ", ";

	os << s.filter;
	if (s.filter & SUB_PORTS) {
		os << " [PORTS";
		has_filter = true;
	}
	if (s.filter & SUB_SERVICE) {
		if (has_filter)
			os << "|";
		else
			os << "[";
		os << "SERVICE";
		has_filter = true;
	}
	if (s.filter & SUB_CANCEL) {
		if (has_filter)
			os << "|";
		else
			os << "[";
		os << "CANCEL";
		has_filter = true;
	}
	if (has_filter)
		os << "]";

	os << ", '" << std::string(s.usr_handle, sizeof(s.usr_handle)) << "')";

	return os;

}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::subscr_event& e) throw()
{
	using namespace posixx::linux::tipc;
	os << "subscr_event(" << e.event << "[";
	if (e.event == PUBLISHED)
		os << "PUBLISHED";
	else if (e.event == WITHDRAWN)
		os << "WITHDRAWN";
	else if (e.event == TIMEOUT)
		os << "TIMEOUT";
	return os << "], " << e.found_lower << ", " << e.found_upper << ", "
			<< e.port_id() << ", " << e.subscription() << ")";
}

inline
std::ostream& operator << (std::ostream& os,
		const posixx::linux::tipc::sockaddr& sa) throw()
{
	using posixx::linux::tipc::sockaddr;
	os << "sockaddr(" << sa.family << ", " << unsigned(sa.scope) << ", ";
	if (sa.type() == sockaddr::ID)
		os << sa.port_id();
	else if (sa.type() == sockaddr::NAME)
		os << sa.port_name() << ", " << sa.name_domain();
	else if (sa.type() == sockaddr::NAMESEQ)
		os << sa.name_seq();
	else
		os << "[UNKNOW addrtype=" << sa.addrtype << "]";
	return os << ")";
}

#endif // POSIXX_LINUX_TIPC_PRINT_HPP_
