#ifndef POSIXX_SOCKET_BASIC_SOCKET_HPP_
#define POSIXX_SOCKET_BASIC_SOCKET_HPP_

#include "../error.hpp" // posixx::error
#include "../static_assert.hpp" // static_assert

#include <string> // std::string
#include <utility> // std::pair
#include <sys/socket.h> // socket, send, recv, etc.

/// @file

/// POSIX thin wrapper
namespace posixx {

/// BSD socket API
namespace socket {

/**
 * Socket type.
 *
 * @see socket().
 * @see socket(7).
 */
enum type
{
	/**
	 * Connectionless, unreliable messages of a fixed maximum
	 * length.
	 */
	DGRAM = SOCK_DGRAM,
	/**
	 * Reliable datagram layer that does not guarantee ordering.
	 */
	RDM = SOCK_RDM,
	/**
	 * Sequenced, reliable, two-way connection-based datagram
	 * transmission.
	 *
	 * Sequenced, reliable, two-way connection-based data
	 * transmission path for datagrams of fixed maximum length;
	 * a consumer is required to read an entire packet with each
	 * read system call.
	 */
	SEQPACKET = SOCK_SEQPACKET,
	/**
	 * Sequenced, reliable, two-way, connection-based byte streams.
	 *
	 * An out-of-band data transmission mechanism may be supported.
	 */
	STREAM = SOCK_STREAM,
	/**
	 * Raw network protocol access.
	 */
	RAW = SOCK_RAW
};

/**
 * Shutdown mode.
 *
 * @see socket::shutdown().
 * @see shutdown(2).
 */
enum shutdown_mode
{
	RD = SHUT_RD,    ///< Further receptions will be disallowed.
	WR = SHUT_WR,    ///< Further transmissions will be disallowed.
	RDWR = SHUT_RDWR ///< Both will be disallowed.
};

/**
 * Generic socket interface.
 *
 * This class is thread-safe as it just stores a file descriptor for the socket.
 *
 * @see socket(7)
 */
template < typename TSockTraits >
struct basic_socket
{

	/// Traits used by this socket
	typedef TSockTraits traits;

	/**
	 * Create an endpoint for communication.
	 *
	 * @param type Type of socket.
	 * @param protocol Protocol number.
	 *
	 * @see socket(2), socket(7)
	 */
	basic_socket(type type, int protocol = 0) throw (error);

	/**
	 * Create a socket instance from a file descriptor.
	 *
	 * This is mainly used by the accept() method and pair() function and
	 * should be used with care because it takes "ownership" of the file
	 * descriptor (it's closed by the destructor).
	 *
	 * Use with care.
	 *
	 * @param fd Socket file descriptor.
	 */
	explicit basic_socket(int fd) throw ();

	/**
	 * Bind a name to the socket.
	 *
	 * @param addr Protocol-dependant name to bind on.
	 *
	 * @see bind(2)
	 */
	void bind(const typename TSockTraits::sockaddr& addr) throw (error);

	/**
	 * Initiate a connection on the socket.
	 *
	 * @param addr Protocol-dependant name to connect to.
	 *
	 * @see connect(2)
	 */
	void connect(const typename TSockTraits::sockaddr& addr) throw (error);

	/**
	 * Get the socket name.
	 *
	 * @return Protocol-dependant name binded.
	 *
	 * @see getsockname(2)
	 */
	typename TSockTraits::sockaddr name() const throw (error);

	/**
	 * Get the name of connected peer socket.
	 *
	 * @return Protocol-dependant name of the peer.
	 *
	 * @see getpeername(2)
	 */
	typename TSockTraits::sockaddr peer_name() const throw (error);

	/**
	 * Send a message on the socket.
	 *
	 * @param buf Message buffer.
	 * @param n Message length.
	 * @param flags Sending options.
	 *
	 * @return The number of characters sent.
	 *
	 * @see send(2), connect()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	ssize_t send(const void* buf, size_t n, int flags = 0) throw (error);

	/**
	 * Receive a message on the socket.
	 *
	 * @param buf Message buffer.
	 * @param n Maximum message length.
	 * @param flags Receiving options.
	 *
	 * @return The number of characters received.
	 *
	 * @see recv(2), connect()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	ssize_t recv(void* buf, size_t n, int flags = 0) throw (error);

	/**
	 * Send a message on the socket to a specific name.
	 *
	 * @param buf Message buffer.
	 * @param n Message length.
	 * @param to Name to send the message to.
	 * @param flags Sending options.
	 *
	 * @return The number of characters sent.
	 *
	 * @see sendto(2)
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	ssize_t send(const void* buf, size_t n,
			const typename TSockTraits::sockaddr& to,
			int flags = 0) throw (error);

	/**
	 * Receive a message on the socket from a specific name.
	 *
	 * @param buf Message buffer.
	 * @param n Maximum message length.
	 * @param from Name to receive the message from.
	 * @param flags Receiving options.
	 *
	 * @return The number of characters received.
	 *
	 * @see recvfrom(2)
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	ssize_t recv(void* buf, size_t n, typename TSockTraits::sockaddr& from,
			int flags = 0) throw (error);

	/**
	 * Get options on the socket.
	 *
	 * @param level Level at which the option reside.
	 * @param optname Name of the option.
	 * @param opt Where to store the option value.
	 *
	 * @see getsockopt(2)
	 */
	template< typename TSockOpt >
	void getsockopt(int level, int optname, TSockOpt& opt) const
			throw (error);

	/**
	 * Set options on the socket.
	 *
	 * @param level Level at which the option reside.
	 * @param optname Name of the option.
	 * @param opt New option value.
	 *
	 * @see setsockopt(2)
	 */
	template< typename TSockOpt >
	void setsockopt(int level, int optname, const TSockOpt& opt)
			throw (error);

	/**
	 * Get option on the socket (type-safe).
	 *
	 * Note that template argument must be specified explicitly, and the
	 * template argument must be a valid socket option, which is a struct
	 * with 5 members: level, optname, read, write and type. level and
	 * optname must be the corresponding SOL_* and SO_* constants. read and
	 * write must be boolean constants indicating if the options can be
	 * read or written. type should be the type of the option's argument.
	 *
	 * See sockopt namespaces for available options.
	 *
	 * @return Option value
	 *
	 * @see getsockopt()
	 */
	template< typename TSockOpt >
	typename TSockOpt::type opt() const throw (error);

	/**
	 * Set option on the socket (type-safe).
	 *
	 * See getsockopt() for details on how options can be constructed.
	 *
	 * @param opt Option value
	 *
	 * @see setsockopt()
	 */
	template< typename TSockOpt >
	void opt(const typename TSockOpt::type& opt) throw (error);

	/**
	 * Listen for connections on the socket.
	 *
	 * @param backlog Maximum length the queue of pending
	 *                        connections may grow to.
	 *
	 * @see accept(), listen(2)
	 */
	void listen(int backlog = 5) throw (error);

	/**
	 * Accept a connection on the socket.
	 *
	 * @see listen(), accept(2)
	 */
	basic_socket* accept() throw (error);

	/**
	 * Accept a connection on the socket.
	 *
	 * @param addr Address of the peer socket, as known to the
	 *             communications layer.
	 *
	 * @see listen(), accept(2)
	 */
	basic_socket* accept(typename TSockTraits::sockaddr& addr)
			throw (error);

	/**
	 * Shut down part of a full-duplex connection.
	 *
	 * @param how How the connection is shutted down.
	 *
	 * @see shutdown(2)
	 */
	void shutdown(shutdown_mode how = RDWR) throw (error);

	/**
	 * Close the socket file descriptor.
	 *
	 * @see close(2)
	 */
	void close() throw (error);

	/**
	 * Destructor.
	 *
	 * Closes the socket.
	 *
	 * @see shutdown(), close()
	 */
	~basic_socket() throw ();

	// Higher level API

	/**
	 * Send a message on the socket.
	 *
	 * The message could be any struct and is sent as binary data without
	 * any concerns about the byte-order.
	 *
	 * This is a shortcut for the low-level send() method, where the
	 * message length is calculated as sizeof(TPacket).
	 *
	 * Of course, TPacket should be a POD type, without pointers or
	 * references.
	 *
	 * This method loops on the low-level send() until all the data is
	 * sent, or throws an exception if the send() can't be completed.
	 *
	 * @param packet Message struct to send.
	 * @param flags Sending options.
	 *
	 * @see send()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	template< typename TPacket >
	void send_struct(const TPacket& packet, int flags = MSG_NOSIGNAL)
			throw (error);

	/**
	 * Receive a message on the socket.
	 *
	 * The message could be any struct and is received as binary data
	 * without any concerns about the byte-order.
	 *
	 * This is a shortcut for the low-level recv() method, where the
	 * message length is calculated as sizeof(TPacket).
	 *
	 * Of course, TPacket should be a POD type, without pointers or
	 * references.
	 *
	 * This method loops on the low-level recv() until all the data is
	 * received, or throws an exception if the recv() can't be completed.
	 *
	 * @param packet Message struct to receive on.
	 * @param flags Sending options.
	 *
	 * @see recv()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	template< typename TPacket >
	void recv_struct(TPacket& packet,
			int flags = MSG_NOSIGNAL | MSG_WAITALL) throw (error);

	/**
	 * Send a message on the socket to a specific name.
	 *
	 * This method behaves the same as send(const TPacket& packet, int
	 * flags) but sending to a specific name.
	 *
	 * @param packet Message struct to send.
	 * @param to Name to send the message to.
	 * @param flags Sending options.
	 *
	 * @see send()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	template< typename TPacket >
	void send_struct(const TPacket& packet,
			const typename TSockTraits::sockaddr& to,
			int flags = MSG_NOSIGNAL) throw (error);

	/**
	 * Receive a message on the socket from a specific name.
	 *
	 * This method behaves the same as recv(TPacket& packet, int flags) but
	 * receiving from a specific name.
	 *
	 * @param packet Message struct to receive to.
	 * @param from Name to receive the message from.
	 * @param flags Receiving options.
	 *
	 * @see recv()
	 *
	 * @todo make an enum for the flags (are they protocol-dependant?)
	 */
	template< typename TPacket >
	void recv_struct(TPacket& packet, typename TSockTraits::sockaddr& from,
			int flags = MSG_NOSIGNAL | MSG_WAITALL) throw (error);

	/**
	 * Get the socket file descriptor.
	 *
	 * You should be careful when using the socket file descriptor, and you
	 * should not close it since it's closed by the destructor.
	 *
	 * @return Socket file descriptor.
	 */
	int fd() const throw ();

	/**
	 * Convert the socket to a file descriptor.
	 *
	 * This is really a shortcut to fd(), useful to pass a socket to a low
	 * level C function.
	 *
	 * @return Socket file descriptor.
	 */
	operator int () const throw ();

private:

	/// Hidden copy constructor (it has non-copiable behavior).
	basic_socket(const basic_socket& s);

	/// Hidden assign operator (it has non-assignable behavior).
	basic_socket& operator=(const basic_socket& s);

	/// Socket file descriptor.
	int _fd;

};

/**
 * Create a pair of connected sockets.
 *
 * @param type Type of socket.
 * @param protocol Protocol number.
 *
 * @return The new connected pair of sockets.
 *
 * @see socketpair(2)
 */
template < typename TSock >
inline
std::pair< TSock*, TSock* > pair(type type, int protocol = 0) throw (error);

} } // namespace posixx::socket




template < typename TSock >
std::pair< TSock*, TSock* > posixx::socket::pair(type type, int protocol)
		throw (posixx::error)
{
	int fds[2];
	if (::socketpair(TSock::traits::PF, type, protocol, fds) == -1)
		throw error("socketpair");
	return std::make_pair(new TSock(fds[0]), new TSock(fds[1]));
}


template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >::basic_socket(int fd) throw ():
		_fd(fd)
{
}

template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >::basic_socket(type type,
		int protocol) throw (posixx::error)
{
	_fd = ::socket(TSockTraits::PF, type, protocol);
	if (_fd == -1)
		throw error("socket");
}

template< typename TSockTraits >
inline
void posixx::socket::basic_socket< TSockTraits >::bind(
		const typename TSockTraits::sockaddr& addr)
		throw (posixx::error)
{
	if (::bind(_fd, reinterpret_cast< const ::sockaddr* >(&addr),
				addr.length()) == -1)
		throw error("bind");
}

template< typename TSockTraits >
inline
void posixx::socket::basic_socket< TSockTraits >::connect(
		const typename TSockTraits::sockaddr& addr)
		throw (posixx::error)
{
	if (::connect(_fd, reinterpret_cast< const ::sockaddr* >(&addr),
				addr.length()) == -1)
		throw error("connect");
}

template< typename TSockTraits >
inline
typename TSockTraits::sockaddr posixx::socket::basic_socket< TSockTraits >::name()
		const throw (posixx::error)
{
	typename TSockTraits::sockaddr addr;
	socklen_t len = sizeof(typename TSockTraits::sockaddr);
	// TODO assert len == sizeof(typename TSockTraits::sockaddr)
	if (::getsockname(_fd, reinterpret_cast< ::sockaddr* >(&addr), &len) == -1)
		throw error("getsockname");
	return addr;
}

template< typename TSockTraits >
inline
typename TSockTraits::sockaddr
posixx::socket::basic_socket< TSockTraits >::peer_name() const throw (posixx::error)
{
	typename TSockTraits::sockaddr addr;
	socklen_t len = sizeof(typename TSockTraits::sockaddr);
	// TODO assert len == sizeof(typename TSockTraits::sockaddr)
	if (::getpeername(_fd, reinterpret_cast< ::sockaddr* >(&addr), &len) == -1)
		throw error("getpeername");
	return addr;
}

template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::send(const void* buf,
		size_t n, int flags) throw (posixx::error)
{
	ssize_t s = ::send(_fd, buf, n, flags);
	if (s == -1)
		throw error("send");
	if (s == 0)
		throw error("send connection shutdown"); // XXX
	return s;
}

template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::recv(void* buf, size_t n, int flags)
		throw (posixx::error)
{
	ssize_t s = ::recv(_fd, buf, n, flags);
	if (s == -1)
		throw error("recv");
	if (s == 0)
		throw error("recv connection shutdown"); // XXX
	return s;
}

template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::send(const void* buf,
		size_t n, const typename TSockTraits::sockaddr& to, int flags)
		throw (posixx::error)
{
	ssize_t s = ::sendto(_fd, buf, n, flags,
			reinterpret_cast< const ::sockaddr* >(&to),
			to.length());
	if (s == -1)
		throw error("sendto");
	if (s == 0) {
		error e("sendto client shutdown"); // XXX
		e.no = 0;
		throw e;
	}
	return s;
}

template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::recv(void* buf, size_t n,
		typename TSockTraits::sockaddr& from, int flags)
		throw (posixx::error)
{
	socklen_t len = sizeof(typename TSockTraits::sockaddr);
	ssize_t s = ::recvfrom(_fd, buf, n, flags,
			reinterpret_cast< ::sockaddr* >(&from), &len);
	if (s == -1)
		throw error("recvfrom");
	if (s == 0) {
		error e("recvfrom client shutdown"); // XXX
		e.no = 0;
		throw e;
	}
	return s;
}

/*
template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::send(const msghdr* msg, int flags)
{
	ssize_t s = ::sendmsg(_fd, msg, flags);
	if (s == -1)
		throw error("sendmsg");
	if (s == 0)
		throw error("send");
	return s;
}

template< typename TSockTraits >
inline
ssize_t posixx::socket::basic_socket< TSockTraits >::recv(struct msghdr* msg, int flags)
{
	ssize_t s = ::recvmsg(_fd, msg, flags);
	if (s == -1)
		throw error("recvmsg");
	if (s == 0)
		throw error("send");
	return s;
}
*/

template< typename TSockTraits >
template< typename TSockOpt >
inline
void posixx::socket::basic_socket< TSockTraits >::getsockopt(int level, int optname,
		TSockOpt& opt) const throw (posixx::error)
{
	socklen_t len = sizeof(TSockOpt);
	if (::getsockopt(_fd, level, optname, &opt, &len) == -1) // TODO assert len == sizeof(TSockOpt)
		throw error("getsockopt");
}

template< typename TSockTraits >
template< typename TSockOpt >
inline
void posixx::socket::basic_socket< TSockTraits >::setsockopt(int level, int optname,
		const TSockOpt& opt) throw (posixx::error)
{
	if (::setsockopt(_fd, level, optname, &opt, sizeof(TSockOpt)) == -1)
		throw error("setsockopt");
}

template< typename TSockTraits >
template< typename TSockOpt >
inline
typename TSockOpt::type posixx::socket::basic_socket< TSockTraits >::opt() const
		throw (posixx::error)
{
	static_assert(TSockOpt::read, "Option is not readable");
	typename TSockOpt::type opt;
	socklen_t len = sizeof(opt);
	// TODO assert len == sizeof(TSockOpt)
	if (::getsockopt(_fd, TSockOpt::level, TSockOpt::optname, &opt, &len) == -1)
		throw error("getsockopt");
	return opt;
}

template< typename TSockTraits >
template< typename TSockOpt >
inline
void posixx::socket::basic_socket< TSockTraits >::opt(
		const typename TSockOpt::type& opt) throw (posixx::error)
{
	static_assert(TSockOpt::write, "Option is not writable");
	if (::setsockopt(_fd, TSockOpt::level, TSockOpt::optname, &opt,
				sizeof(typename TSockOpt::type)) == -1)
		throw error("setsockopt");
}

template< typename TSockTraits >
inline
void posixx::socket::basic_socket< TSockTraits >::listen(int backlog) throw (posixx::error)
{
	if (::listen(_fd, backlog) == -1)
		throw error("listen");
}

template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >* posixx::socket::basic_socket< TSockTraits >::accept()
		throw (posixx::error)
{
	int fd = ::accept(_fd, 0, 0);
	if (fd == -1)
		throw error("accept");
	return new basic_socket(fd);
}

template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >*
posixx::socket::basic_socket< TSockTraits >::accept(typename TSockTraits::sockaddr& addr)
		throw (posixx::error)
{
	socklen_t len = sizeof(typename TSockTraits::sockaddr);
	// TODO assert len = sizeof(typename TSockTraits::sockaddr)
	int fd = ::accept(_fd, reinterpret_cast< ::sockaddr* >(&addr), &len);
	if (fd == -1)
		throw error("accept");
	return new basic_socket(fd);
}

template< typename TSockTraits >
inline
void posixx::socket::basic_socket< TSockTraits >::shutdown(shutdown_mode how)
		throw (posixx::error)
{
	if (::shutdown(_fd, how) == -1)
		throw error("shutdown");
}

template< typename TSockTraits >
inline
void posixx::socket::basic_socket< TSockTraits >::close() throw (posixx::error)
{
	if (::close(_fd) == -1)
		throw error("close");
	_fd = -1;
}

template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >::~basic_socket() throw ()
{
	if (_fd != -1)
		close();
}

template< typename TSockTraits >
template< typename TPacket >
void posixx::socket::basic_socket< TSockTraits >::send_struct(
		const TPacket& packet, int flags) throw (posixx::error)
{
	ssize_t s = 0;
	while (static_cast< unsigned >(s) < sizeof(TPacket))
	{
		try
		{
			s += send(&packet + s, sizeof(TPacket) - s, flags);
		}
		catch (const error& e)
		{
			if (e.no != EINTR && e.no != EAGAIN)
				throw;
		}
	}
	if (static_cast< unsigned >(s) != sizeof(TPacket))
		throw error("send size not match");
}

template< typename TSockTraits >
template< typename TPacket >
inline
void posixx::socket::basic_socket< TSockTraits >::recv_struct(TPacket& packet, int flags)
		throw (posixx::error)
{
	ssize_t s = 0;
	while (static_cast< unsigned >(s) < sizeof(TPacket))
	{
		try
		{
			s += recv(&packet + s, sizeof(TPacket) - s, flags);
		}
		catch (const error& e)
		{
			if (e.no != EINTR && e.no != EAGAIN && e.no != EWOULDBLOCK)
				throw;
		}
	}
	if (static_cast< unsigned >(s) != sizeof(TPacket))
		throw error("recv size not match");
}

template< typename TSockTraits >
template< typename TPacket >
inline
void posixx::socket::basic_socket< TSockTraits >::send_struct(
		const TPacket& packet,
		const typename TSockTraits::sockaddr& to,
		int flags) throw (posixx::error)
{
	ssize_t s = 0;
	while (static_cast< unsigned >(s) < sizeof(TPacket))
	{
		try
		{
			s += send(&packet + s, sizeof(TPacket) - s, to, flags);
		}
		catch (const error& e)
		{
			if (e.no != EINTR && e.no != EAGAIN)
				throw;
		}
	}
	if (static_cast< unsigned >(s) != sizeof(TPacket))
		throw error("send size not match");
}

template< typename TSockTraits >
template< typename TPacket >
inline
void posixx::socket::basic_socket< TSockTraits >::recv_struct(TPacket& packet,
		typename TSockTraits::sockaddr& from, int flags)
		throw (posixx::error)
{
	ssize_t s = 0;
	while (static_cast< unsigned >(s) < sizeof(TPacket))
	{
		try
		{
			s += recv(&packet + s, sizeof(TPacket) - s, from, flags);
		}
		catch (const error& e)
		{
			if (e.no != EINTR && e.no != EAGAIN && e.no != EWOULDBLOCK)
				throw;
		}
	}
	if (static_cast< unsigned >(s) != sizeof(TPacket))
		throw error("recv size not match");
}

template< typename TSockTraits >
inline
int posixx::socket::basic_socket< TSockTraits >::fd() const throw ()
{
	return _fd;
}

template< typename TSockTraits >
inline
posixx::socket::basic_socket< TSockTraits >::operator int () const throw ()
{
	return _fd;
}

#if 0

template< typename TSockTraits >
template< typename TPacket >
inline
void send(const TPacket& packet) // TODO specialize with std::string
{
}

template< typename TSockTraits >
template< typename TPacket >
inline
void recv(TPacket& packet) // TODO specialize with std::string
{
}

template< typename TSockTraits >
inline
void recv(std::string& buf, size_t n)
{
}
#endif

#endif // _POSIXX_SOCKET_BASIC_SOCKET_HPP_
