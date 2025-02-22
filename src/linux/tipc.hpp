// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef POSIXX_LINUX_TIPC_HPP_
#define POSIXX_LINUX_TIPC_HPP_

#include "../socket/basic_socket.hpp" // posixx::socket

#include <linux/tipc.h> // all tipc stuff
#include <cstring> // memcpy

/// @file

namespace posixx {

/// Linux specific functionality
namespace linux {

/// TIPC socket domain.
namespace tipc {

enum
{
	/// Configuration port name.
	CFG_SRV = TIPC_CFG_SRV,
	/// Topology discovery port name.
	TOP_SRV = TIPC_TOP_SRV,
	/// Name of the last reserved port.
	RESERVED_TYPES = TIPC_RESERVED_TYPES,
	/// Maximum message size.
	MAX_USER_MSG_SIZE = TIPC_MAX_USER_MSG_SIZE,
	/// Wait forever (for subscriptions).
	WAIT_FOREVER = TIPC_WAIT_FOREVER
};

/// TIPC Address
struct addr
{

	/// 32-bit address.
	__u32 address;

	/// Constructor
	addr() throw ();

	/**
	 * Constructor from a raw 32-bit address.
	 *
	 * @param addr Raw 32-bit address.
	 */
	explicit addr(__u32 addr) throw ();

	/**
	 * Constructor from a zone, cluster and node information.
	 *
	 * @param zone Zone.
	 * @param cluster Cluster.
	 * @param node Node.
	 */
	addr(unsigned int zone, unsigned int cluster, unsigned int node)
		throw ();

	/// Set zone.
	void zone(unsigned int zone) throw ();

	/// Get zone.
	unsigned int zone() const throw ();

	/// Set cluster.
	void cluster(unsigned int cluster) throw ();

	/// Get cluster.
	unsigned int cluster() const throw ();

	/// Set node.
	void node(unsigned int node) throw ();

	/// Get node.
	unsigned int node() const throw ();

	/// Cast to a raw 32-bit address.
	operator __u32 () const throw ();

};

/**
 * Port Identificator.
 *
 * @see tipc_portid struct from linux/tipc.h.
 */
struct portid: tipc_portid
{

	/**
	 * Constructor.
	 *
	 * @param ref Unique ID fo the port in the address.
	 * @param node Node address.
	 */
	portid(__u32 ref, addr node) throw ();

	/// Access to the node addr.
	addr& node_addr() throw ();

	/// Access to the node addr.
	const addr& node_addr() const throw ();

	/// Compares 2 port ids.
	bool operator == (const portid& other) const throw ();

};

/**
 * Named port.
 *
 * @see tipc_name struct from linux/tipc.h.
 */
struct name: tipc_name
{

	/**
	 * Constructor.
	 *
	 * @param type Type of the named port.
	 * @param instance Instance of the named port.
	 */
	name(__u32 type, __u32 instance) throw ();

	/// Compares 2 port names.
	bool operator == (const name& other) const throw ();

};

/**
 * Port sequence.
 *
 * @see tipc_name_seq struct from linux/tipc.h.
 */
struct nameseq: tipc_name_seq
{

	/**
	 * Constructor.
	 *
	 * @param type Type of the sequence.
	 * @param lower Lower bound.
	 * @param upper Upper bound.
	 */

	nameseq(__u32 type, __u32 lower, __u32 upper) throw ();
	/**
	 * Constructor.
	 *
	 * @param type Type of the sequence.
	 * @param instance Lower and upper bound.
	 */
	nameseq(__u32 type, __u32 instance) throw ();

	/// Compares 2 port name sequences.
	bool operator == (const nameseq& other) const throw ();

};

/**
 * Reasons for returned messages when recvmsg() is used.
 *
 * @see recvmsg(2) of TIPC documentation
 */
enum reason_t
{
	/// Not a returned message.
	OK = TIPC_OK,
	/// Port name doesn't exist.
	ERR_NO_NAME = TIPC_ERR_NO_NAME,
	/// Port ID doesn't exist.
	ERR_NO_PORT = TIPC_ERR_NO_PORT,
	/// Node doesn't exist.
	ERR_NO_NODE = TIPC_ERR_NO_NODE,
	/// Reception queue is full.
	ERR_OVERLOAD = TIPC_ERR_OVERLOAD,
	/// Connection shutted down.
	CONN_SHUTDOWN = TIPC_CONN_SHUTDOWN
};

/**
 * Subscription filter type.
 *
 * @see TIPC documentation: 1.4.3 Name Subscriptions
 */
enum subscr_t
{

	/**
	 * Causes the topology service to generate a PUBLISHED event
	 * for each port name or port name sequence it finds that overlaps
	 * the specified port name sequence; a WITHDRAWN event is issued
	 * each time a previously reported name becomes unavailable.
	 *
	 * Allows the topology service to inform the application if there
	 * are @b any ports of interest.
	 */
	SUB_PORTS = TIPC_SUB_PORTS,

	/**
	 * Causes the topology service to generate a single publish event for
	 * the first port it finds with an overlapping name and a single
	 * withdraw event when the last such port becomes unavailable.
	 *
	 * Allows the topology service to inform the application about
	 * @b all ports of interest.
	 */
	SUB_SERVICE = TIPC_SUB_SERVICE,

	/**
	 * Instruct the topology service to cancel a previously requested
	 * subscription.
	 *
	 * The application simply resends the original subscription request
	 * with SUB_CANCEL logically OR'd into the event filter.
	 *
	 * @note This is implemented in TIPC 1.7+ only (according to TIPC
	 *       documentation).
	 */
	SUB_CANCEL = TIPC_SUB_CANCEL

};

/**
 * Subscription request message.
 *
 * @see TIPC documentation: 1.4.3 Name Subscriptions
 */
struct subscr: tipc_subscr
{

	/**
	 * Constructor.
	 *
	 * @param seq The port name sequence of interest to the application.
	 * @param timeout A subscription timeout value, in ms
	 *                (or WAIT_FOREVER).
	 * @param filter An event filter specifying which events are of
	 *               interest to the application (see subscr_t).
	 * @param usr_handle An 8 byte user handle that is application-defined,
	 *                   (treated as a string).
	 */
	subscr(nameseq seq, __u32 timeout, __u32 filter,
			const char* usr_handle = "") throw ();

	/**
	 * Constructor.
	 *
	 * @param seq The port name sequence of interest to the application.
	 * @param timeout A subscription timeout value, in ms
	 *                (or WAIT_FOREVER).
	 * @param filter An event filter specifying which events are of
	 *               interest to the application (see subscr_t).
	 * @param usr_handle An 8 byte user handle that is application-defined.
	 *                   (treated as binary data).
	 * @param handle_size Size of the usr_handle buffer (it should be at
	 *                    most 8)
	 */
	subscr(nameseq seq, __u32 timeout, __u32 filter,
			const char* usr_handle, size_t handle_size)
			throw ();

	/// Access to the subscribed name sequence.
	nameseq& name_seq() throw ();

	/// Access to the subscribed name sequence.
	const nameseq& name_seq() const throw ();

	/// Set the user handle as a string.
	void handle(const char* usr_handle) throw ();

	/// Set the user handle as binary data.
	void handle(const char* usr_handle, size_t handle_size) throw ();

	/// Compares 2 subscription request messages.
	bool operator == (const subscr& other) const throw ();

};

/// Type of events.
enum event_t
{

	/// The port has been published.
	PUBLISHED = TIPC_PUBLISHED,

	/// The port has been withdrawn.
	WITHDRAWN = TIPC_WITHDRAWN,

	/// The event has timed out.
	TIMEOUT = TIPC_SUBSCR_TIMEOUT

};

/**
 * Event message.
 *
 * @see TIPC documentation: 1.4.3 Name Subscriptions
 */
struct subscr_event: tipc_event
{

	/// Access to the subscribed name sequence.
	portid& port_id() throw ();

	/// Access to the subscribed name sequence.
	const portid& port_id() const throw ();

	/// Access to the subscribed name sequence.
	subscr& subscription() throw ();

	/// Access to the subscribed name sequence.
	const subscr& subscription() const throw ();

};

/// Type of TIPC address
enum type_t
{
	ID = TIPC_ADDR_ID,           ///< Port ID
	NAME = TIPC_ADDR_NAME,       ///< Port name
	NAMESEQ = TIPC_ADDR_NAMESEQ  ///< Name sequence or multicast
};

/**
 * Bind scope.
 *
 * @see TIPC documentation: 2.1.2 bind
 */
enum scope_t
{
	ZONE = TIPC_ZONE_SCOPE,       ///< Zone scope.
	CLUSTER = TIPC_CLUSTER_SCOPE, ///< Cluster scope.
	NODE = TIPC_NODE_SCOPE        ///< Node scope.
};

/**
 * TIPC socket address (name).
 *
 * @see bind(2), Socket::bind()
 * @see connect(2), Socket::connect()
 * @see getsockaddr(2), Socket::getsockaddr()
 * @see setsockaddr(2), Socket::setsockaddr()
 * @see accept(2), Socket::accept()
 * @see sendto(2), Socket::send()
 * @see recvfrom(2), Socket::recv()
 */
struct sockaddr: sockaddr_tipc
{

	/// Constructor.
	sockaddr() throw ();

	/**
	 * Constructor using a port ID.
	 *
	 * @param port Port ID.
	 * @param scope Bind scope.
	 */
	sockaddr(portid port, scope_t scope = ZONE) throw ();

	/**
	 * Constructor using a port name.
	 *
	 * @param name Port name.
	 * @param scope Bind scope.
	 * @param domain Domain lookup.
	 *
	 * @see Documentación de TIPC: 1.4.1 Address Resolution
	 */
	sockaddr(name name, scope_t scope = ZONE,
			tipc::addr domain = tipc::addr(0u))
		throw ();

	/**
	 * Constructor using a port name sequence.
	 *
	 * @param nameseq Port name sequence.
	 * @param scope Bind scope.
	 */
	sockaddr(nameseq nameseq, scope_t scope = ZONE) throw ();

	/// Type of TIPC address
	type_t type() const throw ();

	/// Length of this unix socket address
	socklen_t length() const throw ();

	/// Compare two TIPC socket addresses
	bool operator == (const sockaddr& other) const throw ();

	/// Access to the port ID (only valid if addrtype == ID)
	portid& port_id() throw ();

	/// Access to the port ID (only valid if addrtype == ID)
	const portid& port_id() const throw ();

	/// Access to the port name (only valid if addrtype == NAME)
	name& port_name() throw ();

	/// Access to the port name (only valid if addrtype == NAME)
	const name& port_name() const throw ();

	/// Access to the port name domain (only valid if addrtype == NAME)
	tipc::addr& name_domain() throw ();

	/// Access to the port name domain (only valid if addrtype == NAME)
	const tipc::addr& name_domain() const throw ();

	/// Access to the port name sequence (only valid if addrtype == NAMESEQ)
	nameseq& name_seq() throw ();

	/// Access to the port name sequence (only valid if addrtype == NAMESEQ)
	const nameseq& name_seq() const throw ();

};

/// TIPC socket traits.
struct traits
{

	/// Socket address type.
	typedef tipc::sockaddr sockaddr;

	/// Protocol family.
	enum { PF = PF_TIPC };

};

/// TIPC socket
typedef posixx::socket::basic_socket< traits > socket;

} } } // namespace posixx::linux::tipc


inline
posixx::linux::tipc::addr::addr() throw (): address(0u)
{
}

inline
posixx::linux::tipc::addr::addr(__u32 address) throw (): address(address)
{
}

inline
posixx::linux::tipc::addr::addr(unsigned int zone, unsigned int cluster,
		unsigned int node) throw ():
	address(tipc_addr(zone, cluster, node))
{
}

inline
void posixx::linux::tipc::addr::zone(unsigned int zone) throw ()
{
	address = tipc_addr(zone, cluster(), node());
}

inline
unsigned int posixx::linux::tipc::addr::zone() const throw ()
{
	return tipc_zone(address);
}

inline
void posixx::linux::tipc::addr::cluster(unsigned int cluster) throw ()
{
	address = tipc_addr(zone(), cluster, node());
}

inline
unsigned int posixx::linux::tipc::addr::cluster() const throw ()
{
	return tipc_cluster(address);
}

inline
void posixx::linux::tipc::addr::node(unsigned int node) throw ()
{
	address = tipc_addr(zone(), cluster(), node);
}

inline
unsigned int posixx::linux::tipc::addr::node() const throw ()
{
	return tipc_node(address);
}

inline
posixx::linux::tipc::addr::operator __u32 () const throw ()
{
	return address;
}

inline
posixx::linux::tipc::portid::portid(__u32 r, addr n) throw ()
{
	ref = r;
	node = n;
}

inline
posixx::linux::tipc::addr& posixx::linux::tipc::portid::node_addr() throw ()
{
	return *reinterpret_cast<addr*>(&node);
}

inline
const posixx::linux::tipc::addr& posixx::linux::tipc::portid::node_addr() const
		throw ()
{
	return *reinterpret_cast<const addr*>(&node);
}

inline
bool posixx::linux::tipc::portid::operator == (
		const posixx::linux::tipc::portid& other) const throw ()
{
	return memcmp(this, &other, sizeof(*this)) == 0;
}

inline
posixx::linux::tipc::name::name(__u32 t, __u32 i) throw ()
{
	type = t;
	instance = i;
}

inline
bool posixx::linux::tipc::name::operator == (
		const posixx::linux::tipc::name& other) const throw ()
{
	return memcmp(this, &other, sizeof(*this)) == 0;
}

inline
posixx::linux::tipc::nameseq::nameseq(__u32 t, __u32 low, __u32 up) throw ()
{
	type = t;
	lower = low;
	upper = up;
}

inline
posixx::linux::tipc::nameseq::nameseq(__u32 t, __u32 instance) throw ()
{
	type = t;
	lower = instance;
	upper = instance;
}

inline
bool posixx::linux::tipc::nameseq::operator == (
		const posixx::linux::tipc::nameseq& other) const throw ()
{
	return memcmp(this, &other, sizeof(*this)) == 0;
}

inline
posixx::linux::tipc::subscr::subscr(nameseq s, __u32 t, __u32 f,
		const char* uh) throw ()
{
	seq = s;
	timeout = t;
	filter = f;
	handle(uh);
}

inline
posixx::linux::tipc::subscr::subscr(nameseq s, __u32 t, __u32 f,
		const char* uh, size_t uh_size) throw ()
{
	seq = s;
	timeout = t;
	filter = f;
	handle(uh, uh_size);
}

inline
posixx::linux::tipc::nameseq& posixx::linux::tipc::subscr::name_seq() throw ()
{
	return *reinterpret_cast<nameseq*>(&seq);
}

inline
const posixx::linux::tipc::nameseq&
posixx::linux::tipc::subscr::name_seq() const throw ()
{
	return *reinterpret_cast<const nameseq*>(&seq);
}

inline
void posixx::linux::tipc::subscr::handle(const char* uh) throw ()
{
	std::strncpy(usr_handle, uh, sizeof(usr_handle));
}

inline
void posixx::linux::tipc::subscr::handle(const char* uh, size_t uh_size)
		throw ()
{
	std::memcpy(usr_handle, uh, uh_size);
}

inline
bool posixx::linux::tipc::subscr::operator == (const subscr& other) const
		throw ()
{
	return memcmp(this, &other, sizeof(*this)) == 0;
}

inline
posixx::linux::tipc::portid& posixx::linux::tipc::subscr_event::port_id()
		throw ()
{
	return *reinterpret_cast<portid*>(&port);
}

inline
const posixx::linux::tipc::portid& posixx::linux::tipc::subscr_event::port_id()
		const throw ()
{
	return *reinterpret_cast<const portid*>(&port);
}

inline
posixx::linux::tipc::subscr& posixx::linux::tipc::subscr_event::subscription()
		throw ()
{
	return *reinterpret_cast<subscr*>(&s);
}

inline
const posixx::linux::tipc::subscr&
posixx::linux::tipc::subscr_event::subscription() const throw ()
{
	return *reinterpret_cast<const subscr*>(&s);
}

inline
posixx::linux::tipc::sockaddr::sockaddr() throw ()
{
	memset(this, 0, sizeof(*this));
	family = AF_TIPC;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(portid port, scope_t s) throw ()
{
	family = AF_TIPC;
	addrtype = ID;
	scope = s;
	addr.id = port;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(name name, scope_t s, tipc::addr d)
		throw ()
{
	family = AF_TIPC;
	addrtype = NAME;
	scope = s;
	addr.name.name = name;
	addr.name.domain = d;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(nameseq nameseq, scope_t s) throw ()
{
	family = AF_TIPC;
	addrtype = NAMESEQ;
	scope = s;
	addr.nameseq = nameseq;
}

inline
posixx::linux::tipc::type_t posixx::linux::tipc::sockaddr::type()
		const throw ()
{
	return static_cast< type_t >(addrtype);
}

inline
socklen_t posixx::linux::tipc::sockaddr::length() const throw ()
{
	return sizeof(sockaddr_tipc);
}

inline
bool posixx::linux::tipc::sockaddr::operator == (const sockaddr& other) const
		throw ()
{
	if (family != other.family)
		return false;
	if (addrtype != other.addrtype)
		return false;
	if (scope != other.scope)
		return false;
	if (addrtype == ID)
		return port_id() == other.port_id();
	if (addrtype == NAME)
		return name_domain() == other.name_domain()
				&& port_name() == other.port_name();
	if (addrtype == NAMESEQ)
		return name_seq() == other.name_seq();
	return memcmp(this, &other, sizeof(*this)) == 0;
}

inline
posixx::linux::tipc::portid& posixx::linux::tipc::sockaddr::port_id() throw ()
{
	return *reinterpret_cast<portid*>(&addr.id);
}

inline
const posixx::linux::tipc::portid& posixx::linux::tipc::sockaddr::port_id()
		const throw ()
{
	return *reinterpret_cast<const portid*>(&addr.id);
}

inline
posixx::linux::tipc::name& posixx::linux::tipc::sockaddr::port_name() throw ()
{
	return *reinterpret_cast<name*>(&addr.name.name);
}

inline
const posixx::linux::tipc::name& posixx::linux::tipc::sockaddr::port_name()
		const throw ()
{
	return *reinterpret_cast<const name*>(&addr.name.name);
}

inline
posixx::linux::tipc::addr& posixx::linux::tipc::sockaddr::name_domain()
		throw ()
{
	return *reinterpret_cast<tipc::addr*>(&addr.name.domain);
}

inline
const posixx::linux::tipc::addr& posixx::linux::tipc::sockaddr::name_domain()
		const throw ()
{
	return *reinterpret_cast<const tipc::addr*>(&addr.name.domain);
}

inline
posixx::linux::tipc::nameseq& posixx::linux::tipc::sockaddr::name_seq()
		throw ()
{
	return *reinterpret_cast<nameseq*>(&addr.nameseq);
}

inline
const posixx::linux::tipc::nameseq& posixx::linux::tipc::sockaddr::name_seq()
		const throw ()
{
	return *reinterpret_cast<const nameseq*>(&addr.nameseq);
}

#endif // POSIXX_LINUX_TIPC_HPP_
