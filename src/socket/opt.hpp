#ifndef POSIXX_SOCKET_OPT_HPP_
#define POSIXX_SOCKET_OPT_HPP_

#include <sys/socket.h>
#include <linux/if.h> // TODO: split POSIX and Linux options

/// @file

namespace posixx { namespace socket {

/**
 * Type-safe socket options
 *
 * @see posixx::socket::setsockopt(), posixx::socket::getsockopt()
 */
namespace opt {

#define MKSOLOPT(O, T, R, W) \
	struct O { \
		enum { \
			level   = SOL_SOCKET, \
			optname = SO_ ## O, \
			read    = R, \
			write   = W }; \
		typedef T; }
#define MKSOLOPT_R(O, T) MKSOLOPT(O, T type, true, false)
#define MKSOLOPT_RW(O, T) MKSOLOPT(O, T type, true, true)
#define MKSOLOPT_ARRAY(O, T, S) MKSOLOPT(O, T type[S], true, true)

MKSOLOPT_R(ACCEPTCONN, int);
MKSOLOPT_ARRAY(BINDTODEVICE, char, IFNAMSIZ);
MKSOLOPT_RW(BROADCAST, int);
MKSOLOPT_RW(BSDCOMPAT, int);
//XXX DEBUG is replaced by the preprocessor in debug builds MKSOLOPT_RW(DEBUG, int);
MKSOLOPT_R(ERROR, int);
MKSOLOPT_RW(DONTROUTE, int);
MKSOLOPT_RW(KEEPALIVE, int);
MKSOLOPT_RW(LINGER, linger);
MKSOLOPT_RW(OOBINLINE, int);
MKSOLOPT_RW(PASSCRED, int);
MKSOLOPT_R(PEERCRED, ucred);
MKSOLOPT_RW(PRIORITY, int);
MKSOLOPT_RW(RCVBUF, size_t);
MKSOLOPT_RW(RCVBUFFORCE, size_t);
MKSOLOPT_RW(RCVLOWAT, size_t);
MKSOLOPT_R(SNDLOWAT, size_t);
MKSOLOPT_RW(RCVTIMEO, timeval);
MKSOLOPT_RW(SNDTIMEO, timeval);
MKSOLOPT_RW(REUSEADDR, int);
MKSOLOPT_RW(SNDBUF, size_t);
MKSOLOPT_RW(SNDBUFFORCE, size_t);
MKSOLOPT_RW(TIMESTAMP, int);
MKSOLOPT_R(TYPE, int);

#undef MKSOLOPT
#undef MKSOLOPT_R
#undef MKSOLOPT_RW
#undef MKSOLOPT_ARRAY

} } } // namespace posixx::socket::opt

#endif // POSIXX_SOCKET_OPT_HPP_
