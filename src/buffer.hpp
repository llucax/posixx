// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef POSIXX_BUFFER_HPP_
#define POSIXX_BUFFER_HPP_

#include "basic_buffer.hpp" // posixx::basic_buffer

namespace posixx {

/// A buffer that uses realloc(3) for memory management.
typedef basic_buffer< unsigned char > buffer;

} // namespace posixx

#endif // POSIXX_BUFFER_HPP_
