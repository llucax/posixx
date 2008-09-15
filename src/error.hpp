#ifndef POSIXX_ERROR_HPP_
#define POSIXX_ERROR_HPP_

#include <cstring>
#include <stdexcept>
#include <cerrno>
#include <string>

/// @file

namespace posixx {

/// Posix error.
struct error: std::runtime_error
{

	/// Default constructor, gets the error number and string from errno.
	error() throw();

	/**
	 * Constructor with a more detailed message.
	 *
	 * @param where Description about where the error was thrown.
	 */
	explicit error(const std::string& where) throw ();

	/// Destructor
	~error() throw();

	/// Error number, as of errno.
	int no;

};

} // namespace posixx


inline
posixx::error::error() throw():
    std::runtime_error(strerror(errno)), no(errno)
{
}

inline
posixx::error::error(const std::string& where) throw ():
    std::runtime_error(where + ": " + strerror(errno)), no(errno)
{
}

inline
posixx::error::~error() throw ()
{
}

#endif // POSIXX_ERROR_HPP_
