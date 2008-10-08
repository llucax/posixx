#ifndef POSIXX_BUFFER_HPP_
#define POSIXX_BUFFER_HPP_

#include "basic_buffer.hpp" // posixx::basic_buffer

#include <cstdlib> // std::realloc()

namespace posixx {

/// A buffer that uses realloc(3) for memory management.
typedef basic_buffer< &std::realloc > buffer;

} // namespace posixx

#endif // POSIXX_BUFFER_HPP_
