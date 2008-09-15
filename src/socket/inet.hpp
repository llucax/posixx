#ifndef POSIXX_SOCKET_INET_HPP_
#define POSIXX_SOCKET_INET_HPP_

#include "../basic_socket.hpp" // posixx::socket

#include <netinet/in.h> // sockaddr_in, htonl, htons, {PF,AF}_INET, INADDR_ANY
#include <arpa/inet.h> // inet_addr
#include <cstring> // memset
#include <string> // std::string

/// @file

namespace posixx { namespace socket {

/// Internet Protocol sockets
namespace inet {

/// IP socket address
struct sockaddr: sockaddr_in
{

	/**
	 * Create an IP socket address.
	 */
	sockaddr(u_int16_t port = 0, const std::string& ip = "0.0.0.0")
			throw ();

	/// Length of this IP socket address
	socklen_t length() const throw ();

	/// Compare two IP socket addresses
	bool operator == (const sockaddr& other) const throw ();

	/// Get the port number
	uint16_t port() const throw ();

	/// Get the IP address
	std::string addr() const throw ();

}; // struct sockaddr

/// IP socket traits
struct traits
{
	/// Socket address type.
	typedef inet::sockaddr sockaddr;
	/// Protocol family.
	enum { PF = PF_INET };
};

/// IP socket
typedef posixx::socket::basic_socket< traits > socket;

} } } // namespace posixx::socket::inet




inline
posixx::socket::inet::sockaddr::sockaddr(u_int16_t port, const std::string& ip)
		throw ()
{
	memset(this, 0, sizeof(struct sockaddr_in));
	sin_family = AF_INET;
	if (ip == "0.0.0.0") {
		sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		sin_addr.s_addr = inet_addr(ip.c_str());
	}
	sin_port = htons(port);
}

inline
socklen_t posixx::socket::inet::sockaddr::length() const throw ()
{
	return sizeof(sockaddr_in);
}

inline
bool posixx::socket::inet::sockaddr::operator == (const sockaddr& other) const
		throw ()
{
	return !memcmp(this, &other, sizeof(*this));
}

inline
uint16_t posixx::socket::inet::sockaddr::port() const throw ()
{
	return ntohs(sin_port);
}

inline
std::string posixx::socket::inet::sockaddr::addr() const throw ()
{
	return inet_ntoa(sin_addr);
}

#endif // POSIXX_SOCKET_INET_HPP_
