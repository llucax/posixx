#ifndef POSIXX_BUFFER_HPP_
#define POSIXX_BUFFER_HPP_

#include "basic_buffer.hpp" // posixx::basic_buffer

namespace posixx {

/// A buffer that uses realloc(3) for memory management.
typedef basic_buffer< unsigned char > buffer;

} // namespace posixx

#endif // POSIXX_BUFFER_HPP_
