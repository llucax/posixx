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
};

/**
 * Port sequence.
 *
 * @see Multicast
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
};

/**
 * Multicast port sequence.
 *
 * It's a port sequence just like nameseq used to simplify the use of the
 * sockaddr.
 *
 * @see nameseq
 * @see Estructura tipc_name_seq de linux/tipc.h
 */
struct multicast: tipc_name_seq
{
	/**
	 * Constructor.
	 *
	 * @param type Type of the multicast sequence.
	 * @param lower Lower bound.
	 * @param upper Upper bound.
	 */
	multicast(__u32 type, __u32 lower, __u32 upper) throw ();
	/**
	 * Constructor.
	 *
	 * @param type Type of the multicast sequence.
	 * @param instance Lower and upper bound.
	 */
	multicast(__u32 type, __u32 instance) throw ();
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
enum subcription_t
{
	/**
	 * Causes the topology service to generate a event::PUBLISHED event
	 * for each port name or port name sequence it finds that overlaps
	 * the specified port name sequence; a TIPC_WITHDRAWN event is issued
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
	SUB_SERVICE = TIPC_SUB_SERVICE
};

/**
 * Subscription request message.
 *
 * @see TIPC documentation: 1.4.3 Name Subscriptions
 */
struct subscription: tipc_subscr
{
	/**
	 * Constructor.
	 *
	 * @param seq The port name sequence of interest to the application.
	 * @param timeout A subscription timeout value, in ms
	 *                (or WAIT_FOREVER).
	 * @param filter An event filter specifying which events are of
	 *               interest to the application (see subscription_t).
	 * @param usr_handle An 8 byte user handle that is application-defined.
	 */
	subscription(nameseq seq, __u32 timeout, __u32 filter,
			const char* usr_handle = "") throw ();
};

/**
 * Event message.
 *
 * @see TIPC documentation: 1.4.3 Name Subscriptions
 */
struct event: tipc_event
{
	/// Type of events.
	enum type_t
	{
		/// The port has been published.
		PUBLISHED = TIPC_PUBLISHED,
		/// The port has been withdrawn.
		WITHDRAWN = TIPC_WITHDRAWN,
		/// The event has timed out.
		TIMEOUT = TIPC_SUBSCR_TIMEOUT
	};
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

	/// Type of TIPC address
	enum type_t
	{
		ID = TIPC_ADDR_ID, ///< Port ID
		NAME = TIPC_ADDR_NAME, ///< Port name
		NAMESEQ = TIPC_ADDR_NAMESEQ ///< Name sequence or multicast
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

	/**
	 * Constructor using a multicast port name sequence.
	 *
	 * @param mcast Multicast port name sequence.
	 * @param scope Bind scope.
	 */
	sockaddr(multicast mcast, scope_t scope = ZONE) throw ();

	/// Type of TIPC address
	type_t type() const throw ();

	/// Length of this unix socket address
	socklen_t length() const throw ();

	/// Compare two TIPC socket addresses
	bool operator == (const sockaddr& other) const throw ();

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
posixx::linux::tipc::name::name(__u32 t, __u32 i) throw ()
{
	type = t;
	instance = i;
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
posixx::linux::tipc::multicast::multicast(__u32 t, __u32 low, __u32 up)
	throw ()
{
	type = t;
	lower = low;
	upper = up;
}

inline
posixx::linux::tipc::multicast::multicast(__u32 t, __u32 instance) throw ()
{
	type = t;
	lower = instance;
	upper = instance;
}

inline
posixx::linux::tipc::subscription::subscription(nameseq s, __u32 t, __u32 f,
		const char* uh) throw ()
{
	seq = s;
	timeout = t;
	filter = f;
	std::memcpy(usr_handle, uh, sizeof(usr_handle));
}

inline
posixx::linux::tipc::sockaddr::sockaddr() throw ()
{
}

inline
posixx::linux::tipc::sockaddr::sockaddr(portid port, scope_t s) throw ()
{
	family = AF_TIPC;
	addrtype = TIPC_ADDR_ID;
	scope = s;
	addr.id = port;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(name name, scope_t s, tipc::addr d)
		throw ()
{
	family = AF_TIPC;
	addrtype = TIPC_ADDR_NAME;
	scope = s;
	addr.name.name = name;
	addr.name.domain = d;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(nameseq nameseq, scope_t s) throw ()
{
	family = AF_TIPC;
	addrtype = TIPC_ADDR_NAMESEQ;
	scope = s;
	addr.nameseq = nameseq;
}

inline
posixx::linux::tipc::sockaddr::sockaddr(multicast mcast, scope_t s) throw ()
{
	family = AF_TIPC;
	addrtype = TIPC_ADDR_MCAST;
	scope = s;
	addr.nameseq = mcast;
}

inline
posixx::linux::tipc::sockaddr::type_t posixx::linux::tipc::sockaddr::type()
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
	return !memcmp(this, &other, sizeof(*this));
}

#endif // POSIXX_LINUX_TIPC_HPP_
