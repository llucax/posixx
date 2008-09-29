#ifndef POSIXX_SOCKET_UNIX_HPP_
#define POSIXX_SOCKET_UNIX_HPP_

#include "basic_socket.hpp" // posixx::socket

#include <sys/un.h> // sockaddr_un
#include <string> // std::string
#include <utility> // std::pair
#include <cstring> // memset, memcpy, memcmp

/// @file

namespace posixx { namespace socket {

/// Unix (local) sockets
namespace unix {

/// Unix socket address
struct sockaddr: sockaddr_un
{

	/// Create an empty unix socket address.
	sockaddr() throw ();

	/**
	 * Create a unix socket address.
	 *
	 * @param path Path to the socket (should be less than ::PATH_MAX
	 *             characters long).
	 */
	sockaddr(const std::string& path) throw (std::logic_error);

	/// Length of this unix socket address
	socklen_t length() const throw ();

	/// Get the path of this unix socket address
	std::string path() const throw ();

	/// Compare two unix socket addresses
	bool operator == (const sockaddr& other) const throw ();

}; // struct sockaddr

/// Unix socket traits
struct traits
{
	/// Socket address type.
	typedef posixx::socket::unix::sockaddr sockaddr;
	/// Protocol family.
	enum { PF = PF_UNIX };
};

/// Unix socket
typedef posixx::socket::basic_socket< traits > socket;

/// Pair of unix sockets
typedef std::pair< socket*, socket* > pair_type;

/// Create a pair of connected unix sockets
pair_type pair(type type, int protocol = 0) throw (posixx::error);

} } } // namespace posixx::socket::unix



inline
posixx::socket::unix::sockaddr::sockaddr() throw ()
{
	sun_family = AF_UNIX;
}

inline
posixx::socket::unix::sockaddr::sockaddr(const std::string& path)
		throw (std::logic_error)
{
	if (path.size() >= sizeof(sun_path))
		throw std::logic_error("path too long");
	sun_family = AF_UNIX;
	memcpy(sun_path, path.c_str(), path.size() + 1); // \0
}

inline
socklen_t posixx::socket::unix::sockaddr::length() const throw ()
{
	return sizeof(sun_family) + strlen(sun_path);
}

inline
std::string posixx::socket::unix::sockaddr::path() const throw ()
{
	return std::string(sun_path);
}

inline
bool posixx::socket::unix::sockaddr::operator == (const sockaddr& other)
		const throw ()
{
	return sun_family == other.sun_family
			&& !strncmp(sun_path, other.sun_path, sizeof(sun_path));
}

inline
posixx::socket::unix::pair_type posixx::socket::unix::pair(type type,
		int protocol) throw (posixx::error)
{
	return posixx::socket::pair< socket >(type, protocol);
}

#endif // POSIXX_SOCKET_UNIX_HPP_
