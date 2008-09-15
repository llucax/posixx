
/*
 * This file should be included from others, and the next symbols are expected
 * to be defined:
 *
 * Preprocessor constants:
 * - One of TEST_[socket type]
 * - TEST_NS should be set to the namespace the socket family that will be used
 * - TEST_PROTOCOL should be set to the socket's protocol to be tested
 * - TEST_HAVE_PAIR should be defined if the socket family have a "pair()"
 *   function.
 * - TEST_CHECK_ADDR should be defined if name() should return the same
 *   sockaddr used when bind()ing.
 *
 * Global variables:
 * - test_address1 and test_address2 should be valid sockaddrs.
 *
 * Global functions:
 * - void clean_test_address(TEST_NS::sock& socket, const TEST_NS::sockaddr&)
 *   should be a function. It will be called before binding a socket to do any
 *   cleanup task for a particular address to be used. Can be a macro.
 * - std::ostream& operator << (std::ostream& os, const TEST_NS::sockaddr&)
 *   should be defined.
 */


#ifdef TEST_PF_UNIX
#undef TEST_PF_UNIX
#define TEST_PF_UNIX 1
#else
#define TEST_PF_UNIX 0
#endif
#ifdef TEST_PF_INET
#undef TEST_PF_INET
#define TEST_PF_INET 1
#else
#define TEST_PF_INET 0
#endif
#ifdef TEST_PF_TIPC
#undef TEST_PF_TIPC
#define TEST_PF_TIPC 1
#else
#define TEST_PF_TIPC 0
#endif

#ifdef TEST_DGRAM
#undef TEST_DGRAM
#define TEST_DGRAM 1
#else
#define TEST_DGRAM 0
#endif
#ifdef TEST_RDM
#undef TEST_RDM
#define TEST_RDM 1
#else
#define TEST_RDM 0
#endif
#ifdef TEST_STREAM
#undef TEST_STREAM
#define TEST_STREAM 1
#else
#define TEST_STREAM 0
#endif
#ifdef TEST_SEQPACKET
#undef TEST_SEQPACKET
#define TEST_SEQPACKET 1
#else
#define TEST_SEQPACKET 0
#endif
#ifdef TEST_RAW
#undef TEST_RAW
#define TEST_RAW 1
#else
#define TEST_RAW 0
#endif

#if TEST_DGRAM
#define TEST_TYPE ::posixx::socket::DGRAM
#endif
#if TEST_RDM
#define TEST_TYPE ::posixx::socket::RDM
#endif
#if TEST_STREAM
#define TEST_TYPE ::posixx::socket::STREAM
#endif
#if TEST_SEQPACKET
#define TEST_TYPE ::posixx::socket::SEQPACKET
#endif
#if TEST_RAW
#define TEST_TYPE ::posixx::socket::RAW
#endif


#if TEST_PF_UNIX // unix sockets are the only that provides pairs

BOOST_AUTO_TEST_CASE( pair_test )
{
	TEST_NS::pair_type p = TEST_NS::pair(TEST_TYPE, TEST_PROTOCOL);
	BOOST_CHECK_GE( p.first->fd(), 0 );
	BOOST_CHECK_GE( p.second->fd(), 0 );
	delete p.first;
	delete p.second;
}

#endif // TEST_PF_UNIX

BOOST_AUTO_TEST_CASE( constructor_test )
{
	TEST_NS::socket s(TEST_TYPE, TEST_PROTOCOL);
	BOOST_CHECK_GE( s.fd(), 0 );
}

BOOST_AUTO_TEST_CASE( close_test )
{
	TEST_NS::socket s(TEST_TYPE, TEST_PROTOCOL);
	s.close();
	BOOST_CHECK_EQUAL( s.fd(), -1 );
}

BOOST_AUTO_TEST_CASE( options_test )
{
	using ::posixx::socket::opt::REUSEADDR;
	TEST_NS::socket s(TEST_TYPE, TEST_PROTOCOL);
	s.opt< REUSEADDR >(true);
	BOOST_CHECK_GE( s.opt< REUSEADDR >(), true );
}

BOOST_AUTO_TEST_CASE( bind_test )
{
	TEST_NS::socket s(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s, test_address1);
	s.bind(test_address1);
	s.name();
#if !TEST_PF_TIPC // TIPC returns a Port ID (and we use Port names)
	BOOST_CHECK_EQUAL( s.name(), test_address1 );
#endif
}


#if TEST_SEQPACKET || TEST_STREAM

// TODO: test different flavors of send/recv.

BOOST_AUTO_TEST_CASE( listen_test )
{
	TEST_NS::socket s(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s, test_address1);
	s.bind(test_address1);
	s.listen();
}

static inline
void set_blocking(int fd, bool blocking)
{
	int flags = fcntl(fd, F_GETFL, 0);
	assert(flags != -1);
	if (blocking)
		fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
	else
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

#if !TEST_PF_TIPC // TIPC can't connect a non-blocking socket
BOOST_AUTO_TEST_CASE( connect_accept_test )
{
	TEST_NS::socket ss(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(ss, test_address1);
	ss.bind(test_address1);
	ss.listen();
	TEST_NS::socket sc(TEST_TYPE, TEST_PROTOCOL);
	set_blocking(sc, false);
	try {
		sc.connect(test_address1);
	} catch (const posixx::error& e) {
		BOOST_REQUIRE_EQUAL( e.no, EINPROGRESS );
	}
	set_blocking(sc, true);
	TEST_NS::socket* sa = ss.accept();
	BOOST_CHECK_EQUAL( sc.peer_name(), test_address1 );
	BOOST_CHECK_EQUAL( sc.peer_name(), sa->name() );
#if !TEST_PF_UNIX // Unix sockets has garbage in this addresses
	BOOST_CHECK_EQUAL( sc.name(), sa->peer_name() );
#endif // TEST_PF_UNIX
	delete sa;
}
#endif // !TEST_PF_TIPC

#endif // TEST_SEQPACKET || TEST_STREAM


#if TEST_DGRAM || TEST_RDM || TEST_RAW

BOOST_AUTO_TEST_CASE( dgram_sendto_recvfrom_test )
{
	// socket 1
	TEST_NS::socket s1(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s1, test_address1);
	s1.bind(test_address1);
	// socket 2
	TEST_NS::socket s2(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s2, test_address2);
	s2.bind(test_address2);
	// socket 1 send
	char buffer[] = "hello world!";
	BOOST_CHECK_EQUAL( s1.send(buffer, sizeof(buffer), test_address2),
			sizeof(buffer) );
	memset(buffer, 0, sizeof(buffer));
	// socket 2 receive
	TEST_NS::sockaddr addr;
	BOOST_CHECK_EQUAL( s2.recv(buffer, sizeof(buffer), addr),
			sizeof(buffer) );
	BOOST_CHECK_EQUAL( buffer, "hello world!" );
#if !TEST_PF_TIPC // TIPC returns a Port ID (and we use Port names)
	BOOST_CHECK_EQUAL( addr, test_address1 );
#endif
}

struct data
{
	char msg[20];
	int number;
};

BOOST_AUTO_TEST_CASE( dgram_sendto_recvfrom_struct_test )
{
	// socket 1
	TEST_NS::socket s1(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s1, test_address1);
	s1.bind(test_address1);
	// socket 2
	TEST_NS::socket s2(TEST_TYPE, TEST_PROTOCOL);
	clean_test_address(s2, test_address2);
	s2.bind(test_address2);
	// socket 1 send
	data d;
	strcpy(d.msg, "hello world!");
	d.number = -32495813;
	s1.send_struct(d, test_address2);
	memset(&d, 0, sizeof(data));
	// socket 2 receive
	TEST_NS::sockaddr addr;
	s2.recv_struct(d, addr);
	BOOST_CHECK_EQUAL( d.msg, "hello world!" );
	BOOST_CHECK_EQUAL( d.number, -32495813 );
}

#endif // TEST_DGRAM || TEST_RDM || TEST_RAW

