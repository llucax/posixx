#ifndef POSIXX_LINUX_TIPC_OPT_HPP_
#define POSIXX_LINUX_TIPC_OPT_HPP_

#include <linux/tipc.h>

/// @file

namespace posixx { namespace linux { namespace tipc {

/**
 * Specific options for TIPC sockets.
 *
 * @see tipc::socket::opt() and setsockopt(2) in TIPC documentation for more
 * details.
 */
namespace opt {

#define MKSOLOPT(O, T) \
	struct O { \
		enum { \
			level   = SOL_TIPC, \
			optname = TIPC_ ## O, \
			read    = true, \
			write   = true }; \
		typedef T type; }

/**
 * How to handle messages sent by the socket if link congestion occurs.
 *
 * If enabled, the message is discarded; otherwise the system queues the
 * message for later transmission.
 *
 * By default, this option is disabled for socket::SEQPACKET, socket::STREAM,
 * and socket::RDM socket types (resulting in "reliable" data transfer), and
 * enabled for socket::DGRAM (resulting in "unreliable" data transfer).
 */
MKSOLOPT(SRC_DROPPABLE, int);

/**
 * How to handle messages sent by the socket if they cannot be delivered.
 *
 * This option governs the handling of messages sent by the socket if the
 * message cannot be delivered to its destination, either because the receiver
 * is congested or because the specified receiver does not exist. If enabled,
 * the message is discarded; otherwise the message is returned to the sender.
 *
 * By default, this option is disabled for socket::SEQPACKET and socket::STREAM
 * socket types, and enabled for socket::RDM and socket::DGRAM, This
 * arrangement ensures proper teardown of failed connections when
 * connection-oriented data transfer is used, without increasing the complexity
 * of connectionless data transfer.
 */
MKSOLOPT(DEST_DROPPABLE, int);

/**
 * Number of ms connect() will wait before aborting a connection attempt.
 *
 * This option specifies the number of milliseconds socket::connect() will wait
 * before aborting a connection attempt because the destination has not
 * responded. By default, 8000 (i.e.  8 seconds) is used.
 *
 * This option has no effect when establishing connections using
 * socket::sendto().
 */
MKSOLOPT(CONN_TIMEOUT, int);

#undef MKSOLOPT

/// @see IMPORTANCE
enum importance
{
	LOW      = TIPC_LOW_IMPORTANCE,      ///< Low importance
	MEDIUM   = TIPC_MEDIUM_IMPORTANCE,   ///< Medium importance
	HIGH     = TIPC_HIGH_IMPORTANCE,     ///< High importance
	CRITICAL = TIPC_CRITICAL_IMPORTANCE  ///< Critical importance
};

/**
 * How likely a message sent by the socket is to be affected by congestion.
 *
 * A message with higher importance is less likely to be delayed or dropped due
 * to link congestion, and also less likely to be rejected due to receiver
 * congestion. The following values are defined: ::LOW, ::MEDIUM, ::HIGH, and
 * ::CRITICAL.
 *
 * By default, ::LOW is used for all TIPC socket types.
 */
struct IMPORTANCE
{
	typedef importance type;
	enum
	{
		level   = SOL_TIPC,
		optname = TIPC_IMPORTANCE,
		read    = true,
		write   = true
	};
};

} } } } // namespace posixx::linux::tipc::opt

#endif // POSIXX_LINUX_TIPC_OPT_HPP_
