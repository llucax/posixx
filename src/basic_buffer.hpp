#ifndef POSIXX_BASIC_BUFFER_HPP_
#define POSIXX_BASIC_BUFFER_HPP_

#include <stdexcept> // std::bad_alloc, std::out_of_range
#include <limits> // std::numeric_limits
#include <tr1/type_traits> // std::tr1::is_integral, true_type, false_type
#include <cstring> // std::memcpy(), memset(), memcmp()
#include <cassert> // assert()

namespace posixx {

/**
 * A low-level buffer.
 *
 * This is pretty much like using a plain C-style array using dynamic memory,
 * with a STL-ish interface.
 *
 * The buffer will use Allocator (which should be a function with realloc(3)
 * semantics) for all storage management.
 */
template< void* (*Allocator)(void*, size_t) >
struct basic_buffer {

	// Types
	//////////////////////////////////////////////////////////////////////

	///
	typedef unsigned char value_type; // TODO: use char to be more
	                                  //       std::string-friendly?

	///
	typedef value_type& reference;

	///
	typedef const value_type& const_reference;

	///
	typedef value_type* iterator; // TODO: make a real iterator

	///
	typedef const value_type* const_iterator;

	///
	typedef size_t size_type;

	///
	typedef ptrdiff_t difference_type;

	///
	typedef value_type* pointer;

	///
	typedef const pointer const_pointer;

	///
	// TODO: reverse iterator
	//typedef typename std::reverse_iterator< iterator > reverse_iterator;

	///
	//typedef typename std::reverse_iterator< const iterator >
	//		const_reverse_iterator;


	// Construct/Copy/Destroy
	//////////////////////////////////////////////////////////////////////

	/**
	 * Creates a buffer of length zero (the default constructor).
	 */
	explicit basic_buffer(): _data(NULL), _size(0) {}

	/**
	 * Creates a buffer with a capacity() of n (uninitialized) elements.
	 *
	 * @throw std::bad_alloc
	 */
	explicit basic_buffer(size_type n):
		_data(_allocate(NULL, n * sizeof(value_type))), _size(n)
	{
	}

	/**
	 * Creates a buffer of length n, containing n copies of value.
	 *
	 * @throw std::bad_alloc
	 */
	basic_buffer(size_type n, const_reference value):
		_data(NULL), _size(0)
	{
		assign(n, value);
		std::memset(_data, value, n);
	}

	/**
	 * Creates a copy of x.
	 *
	 * @throw std::bad_alloc
	 */
	basic_buffer(const basic_buffer< Allocator >& x):
			_data(_allocate(NULL, x.size() * sizeof(value_type))),
			_size(x.size())
	{
		std::memcpy(_data, x.c_array(), x.size());
	}

	/**
	 * Creates a buffer of length finish - start, filled with all values
	 * obtained by dereferencing the InputIterators on the range [start,
	 * finish).
	 *
	 * @throw std::bad_alloc
	 */
	template < typename InputIterator >
	basic_buffer(InputIterator start, InputIterator finish):
			_data(NULL), _size(0)
	{
		assign(start, finish);
	}

	/**
	 * Erases all elements in self then inserts into self a copy of each
	 * element in x.
	 *
	 * Invalidates all references and iterators.
	 *
	 * @returns a reference to self.
	 *
	 * @throw std::bad_alloc
	 */
	basic_buffer< Allocator >& operator = (
			const basic_buffer< Allocator >& x)
	{
		if (this != &x) {
			resize(x.size());
			std::memcpy(_data, x.c_array(), x.size());
		}
		return *this;
	}

	/**
	 * Frees the memory holded by the buffer
	 */
	~basic_buffer()
	{ clear(); }


	// Iterators
	//////////////////////////////////////////////////////////////////////

	/**
	 * Returns a random access iterator that points to the first element.
	 */
	iterator begin()
	{ return _data; }

	/**
	 * Returns a random access const_iterator that points to the first
	 * element.
	 */
	const_iterator begin() const
	{ return _data; }

	/**
	 * Returns a random access iterator that points to the past-the-end
	 * value.
	 */
	iterator end()
	{ return _data ? _data + _size : NULL; }

	/**
	 * Returns a random access const_iterator that points to the
	 * past-the-end value.
	 */
	const_iterator end() const
	{ return _data ? _data + _size : NULL; }

	/**
	 * Returns a random access reverse_iterator that points to the
	 * past-the-end value.
	 */
	//reverse_iterator rbegin()
	//{ return std::reverse_iterator< iterator >(end()); }

	/**
	 * Returns a random access const_reverse_iterator that points to the
	 * past-the-end value.
	 */
	//const_reverse_iterator rbegin() const
	//{ return std::reverse_iterator< const iterator >(end()); }

	/**
	 * Returns a random access reverse_iterator that points to the first
	 * element.
	 */
	//reverse_iterator rend()
	//{ return std::reverse_iterator< iterator >(begin()); }

	/**
	 * Returns a random access const_reverse_iterator that points to the
	 * first element.
	 */
	//const_reverse_iterator rend() const
	//{ return std::reverse_iterator< const iterator >(begin()); }


	// Capacity
	//////////////////////////////////////////////////////////////////////

	/**
	 * Returns the number of elements.
	 */
	size_type size() const
	{ return _size; }

	/**
	 * Returns size() of the largest possible buffer.
	 */
	size_type max_size() const
	{ return std::numeric_limits< size_type >::max(); }

	/**
	 * Alters the size of self.
	 *
	 * If the new size (sz) is greater than the current size, then
	 * sz-size() (uninitialized) elements are inserted at the end of the
	 * buffer. If the new size is smaller than the current capacity, then
	 * the buffer is truncated by erasing size()-sz elements off the end.
	 * If sz is equal to capacity then no action is taken.
	 *
	 * Invalidates all references and iterators.
	 *
	 * @throw std::bad_alloc
	 */
	void resize(size_type sz)
	{
		pointer n_data = _allocate(_data, sz * sizeof(value_type));
		_data = n_data;
		_size = sz;
	}

	/**
	 * Alters the size of self.
	 *
	 * If the new size (sz) is greater than the current size, then
	 * sz-size() copies of value are inserted at the end of the buffer. If
	 * the new size is smaller than the current capacity, then the buffer
	 * is truncated by erasing size()-sz elements off the end.  If sz is
	 * equal to capacity then no action is taken.
	 *
	 * Invalidates all references and iterators.
	 *
	 * @throw std::bad_alloc
	 */
	void resize(size_type sz, value_type value)
	{
		size_type old_size = size();
		resize(sz);
		if (old_size < sz)
			std::memset(_data + old_size, value, sz - old_size);
	}

	/**
	 * Same as size().
	 */
	size_type capacity() const
	{ return _size; }

	/**
	 * Returns true if the size is zero.
	 */
	bool empty() const
	{ return !_size; }

	/**
	 * If sz > size() call resize(sz), otherwise do nothing.
	 *
	 * Invalidates all references and iterators in the first case.
	 *
	 * @throw std::bad_alloc
	 */
	void reserve(size_type sz)
	{
		if (sz > size())
			resize(sz);
	}


	// Element Access
	//////////////////////////////////////////////////////////////////////

	/**
	 * Returns a reference to element n of self.
	 *
	 * The result can be used as an lvalue. The index n must be between
	 * 0 and the size less one.
	 */
	reference operator[](size_type n)
	{ assert(n < _size); return _data[n]; }

	/**
	 * Returns a constant reference to element n of self.
	 *
	 * The index n must be between 0 and the size less one.
	 */
	const_reference operator[](size_type n) const
	{ assert(n < _size); return _data[n]; }

	/**
	 * Returns a reference to element n of self.
	 *
	 * The result can be used as an lvalue. If index n is not between
	 * 0 and the size less one, a std::out_of_range exception.
	 */
	reference at(size_type n)
	{
		if (n >= _size)
			throw std::out_of_range("posixx::basic_buffer::at(n)");
		return _data[n];
	}

	/**
	 * Returns a constant reference to element n of self.
	 *
	 * If index n is not between 0 and the size less one,
	 * a std::out_of_range exception.
	 */
	const_reference at(size_type n) const
	{
		if (n >= _size)
			throw std::out_of_range("posixx::basic_buffer::at(n)");
		return _data[n];
	}

	/**
	 * Returns a reference to the first element.
	 */
	reference front()
	{ assert(_size); return _data[0]; }

	/**
	 * Returns a constant reference to the first element.
	 */
	const_reference front() const
	{ assert(_size); return _data[0]; }

	/**
	 * Returns a reference to the last element.
	 */
	reference back()
	{ assert(_size); return _data[_size - 1]; }

	/**
	 * Returns a constant reference to the last element.
	 */
	const_reference back() const
	{ assert(_size); return _data[_size - 1]; }

	/**
	 * Returns a pointer to a C-style array.
	 */
	pointer c_array()
	{ return _data; }

	/**
	 * Returns a pointer to a read-only C-style array.
	 */
	const_pointer c_array() const
	{ return _data; }


	// Modifiers
	//////////////////////////////////////////////////////////////////////

	/**
	 * Replaces elements with copies of those in the range [start, finish).
	 *
	 * The function invalidates all iterators and references to elements in
	 * *this.
	 *
	 * Invalidates all references and iterators.
	 */
	template < typename InputIterator >
	void assign(InputIterator start, InputIterator finish)
	{
		// Check whether it's an integral type.  If so, it's not an
		// iterator.
		typename std::tr1::is_integral< InputIterator >::type is_int;
		_assign_dispatch(start, finish, is_int);
	}

	/**
	 * Replaces elements in *this with n copies of value.
	 *
	 * The function invalidates all iterators and references to elements in
	 * *this.
	 *
	 * Invalidates all references and iterators.
	 */
	void assign(size_type n, const_reference value)
	{
		resize(n);
		std::memset(_data, value, n);
	}

	/**
	 * Efficiently swaps the contents of x and y.
	 *
	 * Invalidates all references and iterators.
	 */
	void swap(basic_buffer< Allocator >& x)
	{
		pointer tmp_data = x._data;
		size_type tmp_size = x._size;
		x._data = _data;
		x._size = _size;
		_data = tmp_data;
		_size = tmp_size;
	}

	/**
	 * Deletes all elements from the buffer.
	 *
	 * Invalidates all references and iterators.
	 */
	void clear()
	{
		_data = _allocate(_data, 0);
		_size = 0;
	}

protected:

	// The underlying data
	pointer _data;

	// Size in number of items
	size_t _size;

	// Allocator wrapper for automatic casting
	static pointer _allocate(void* ptr, size_t sz)
	{
		if (ptr == NULL && sz == 0)
			return NULL;
		void* new_ptr = Allocator(ptr, sz);
		if (sz != 0 && new_ptr == NULL)
			throw std::bad_alloc();
		return static_cast< pointer >(new_ptr);
	}


	/*
	 * Helper assign functions to disambiguate the Iterator based and the
	 * N-value copy assign() methods. The last arguments indicates if
	 * InputIterator is an integral type.
	 */

	// This is the version for the integral types, we should use the
	// "regular" N-value copy assign().
	template < typename InputIterator >
	void _assign_dispatch(InputIterator start, InputIterator finish,
			std::tr1::true_type)
	{
		assign(static_cast< size_type >(start),
				static_cast< value_type >(finish));
	}

	// This is the version for the real iterators.
	template < typename InputIterator >
	void _assign_dispatch(InputIterator start, InputIterator finish,
			std::tr1::false_type)
	{
		// TODO: provide an efficient version for random iterators
		while (start != finish) {
			resize(size() + 1);
			(*this)[size() - 1] = *start;
			++start;
		}
	}

};

// Nonmember Operators
//////////////////////////////////////////////////////////////////////

/**
 * Returns true if x hass the same contents as y.
 */
template < void* (*Allocator)(void*, size_t) >
bool operator == (const basic_buffer< Allocator >& x, const basic_buffer < Allocator >& y)
{
	if (&x == &y)
		return true;
	if (x.size() != y.size())
		return false;
	if (x.empty() && y.empty())
		return true;
	if (x.empty() || y.empty())
		return false;
	int r = std::memcmp(x.c_array(), y.c_array(),
			(x.size() < y.size()) ? x.size() : y.size());
	if (r == 0)
		return x.size() == y.size();
	return r == 0;
}

/**
 * Returns !(x==y).
 */
template < void* (*Allocator)(void*, size_t) >
bool operator != (const basic_buffer<Allocator>& x, const basic_buffer <Allocator>& y)
{
	return !(x == y);
}

/**
 * Returns true if the elements contained in x are lexicographically less than
 * the elements contained in y.
 */
template < void* (*Allocator)(void*, size_t) >
bool operator < (const basic_buffer< Allocator >& x, const basic_buffer< Allocator >& y)
{
	if (&x == &y)
		return false;
	if (x.empty() && y.empty())
		return false;
	if (x.empty())
		return true;
	if (y.empty())
		return false;
	int r = std::memcmp(x.c_array(), y.c_array(),
			(x.size() < y.size()) ? x.size() : y.size());
	if (r == 0)
		return x.size() < y.size();
	return r < 0;
}

/**
 * Returns y < x.
 */
template < void* (*Allocator)(void*, size_t) >
bool operator > (const basic_buffer< Allocator >& x, const basic_buffer< Allocator >& y)
{
	return y < x;
}

/**
 * Returns !(y < x).
 */
template < void* (*Allocator)(void*, size_t) >
bool operator <= (const basic_buffer< Allocator >& x, const basic_buffer< Allocator >& y)
{
	return !(y < x);
}

/**
 * Returns !(x < y).
 */
template < void* (*Allocator)(void*, size_t) >
bool operator >= (const basic_buffer< Allocator >& x, const basic_buffer< Allocator >& y)
{
	return !(x < y);
}


// Specialized Algorithms
//////////////////////////////////////////////////////////////////////

/**
 * Exchanges self with x, by swapping all elements.
 *
 * Invalidates all references and iterators.
 */
template < void* (*Allocator)(void*, size_t) >
void swap(basic_buffer< Allocator >& x, basic_buffer< Allocator >& y)
{
	x.swap(y);
}

} // namespace posixx

#endif // POSIXX_BASIC_BUFFER_HPP_
