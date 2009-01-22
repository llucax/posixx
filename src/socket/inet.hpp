#ifndef POSIXX_SOCKET_INET_HPP_
#define POSIXX_SOCKET_INET_HPP_

#include "basic_socket.hpp" // posixx::socket

#include <netinet/in.h> // sockaddr_in, htonl, htons, {PF,AF}_INET, INADDR_ANY
#include <arpa/inet.h> // inet_addr
#include <cstring> // memset
#include <string> // std::string

/// @file

namespace posixx { namespace socket {

/// Internet Protocol sockets
namespace inet {


/// Address to accept any incoming messages
const uint32_t any = INADDR_ANY;

/// Address to send to all hosts
const uint32_t broadcast = INADDR_BROADCAST;


/// IP socket address
struct sockaddr: sockaddr_in
{

	/**
	 * Create an IP socket address.
	 *
	 * @note The IP address is expected in network byte order but the
	 *       port is expected in host byte order. This is because this
	 *       constructor is expected to be used with predefined IP
	 *       addresses, like any, none, loopback, etc.
	 *
	 * @param addr IP address in network byte order
	 * @param port IP port in host byte order
	 *
	 * @see any, none, etc. constants
	 */
	explicit sockaddr(uint32_t addr = any, uint16_t port = 0) throw ();

	/**
	 * Create an IP socket address.
	 *
	 * @param addr String representation of the IP address
	 * @param port IP port in host byte order
	 */
	explicit sockaddr(const std::string& addr, uint16_t port) throw ();

	/**
	 * Set the IP address and port.
	 *
	 * @param addr IP address in network byte order
	 * @param port IP port in host byte order
	 *
	 * @see sockaddr(uint32_t, uint16_t)
	 */
	void set(uint32_t addr, uint16_t port) throw();

	/**
	 * Set the IP address and port.
	 *
	 * @param addr String representation of the IP address
	 * @param port IP port in host byte order
	 *
	 * @see sockaddr(const std::string&, uint16_t)
	 */
	void set(const std::string& addr, uint16_t port) throw();

	/// Get the IP address as a string
	std::string addr() const throw ();

	/// Set the IP address (in network byte order)
	void addr(uint32_t addr) throw ();

	/// Set the IP address from a string
	void addr(const std::string& addr) throw ();

	/// Get the port number
	uint16_t port() const throw ();

	/// Set the port number (port is expected to be in host byte order)
	void port(uint16_t port) throw ();

	/// Length of this IP socket address
	socklen_t length() const throw ();

	/// Compare two IP socket addresses
	bool operator == (const sockaddr& other) const throw ();

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
posixx::socket::inet::sockaddr::sockaddr(uint32_t addr, uint16_t port)
		throw ()
{
	memset(this, 0, sizeof(struct sockaddr_in));
	sin_family = AF_INET;
	set(addr, port);
}

inline
posixx::socket::inet::sockaddr::sockaddr(const std::string& addr, uint16_t port)
		throw ()
{
	memset(this, 0, sizeof(struct sockaddr_in));
	sin_family = AF_INET;
	set(addr, port);
}

inline
void posixx::socket::inet::sockaddr::set(uint32_t addr, uint16_t port)
		throw ()
{
	sin_addr.s_addr = addr;
	this->port(port);
}

inline
void posixx::socket::inet::sockaddr::set(const std::string& addr, uint16_t port)
		throw ()
{
	this->addr(addr);
	this->port(port);
}

inline
std::string posixx::socket::inet::sockaddr::addr() const throw ()
{
	return inet_ntoa(sin_addr);
}

inline
void posixx::socket::inet::sockaddr::addr(uint32_t addr) throw ()
{
	sin_addr.s_addr = addr;
}

inline
void posixx::socket::inet::sockaddr::addr(const std::string& addr) throw ()
{
	sin_addr.s_addr = inet_addr(addr.c_str());
}

inline
uint16_t posixx::socket::inet::sockaddr::port() const throw ()
{
	return ntohs(sin_port);
}

inline
void posixx::socket::inet::sockaddr::port(uint16_t port) throw ()
{
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

#endif // POSIXX_SOCKET_INET_HPP_
