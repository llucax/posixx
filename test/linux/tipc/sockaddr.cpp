// Copyright Leandro Lucarella 2008 - 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)



#include "common.hpp" // PTYPE, INST{1,2}, test_address{1,2}
#include <posixx/linux/tipc.hpp> // posixx::linux::tipc
#include <boost/test/unit_test.hpp> // unit testing stuff
#include <stdint.h>

using namespace ::posixx::linux::tipc;
namespace tipc = ::posixx::linux::tipc;

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_addr_test )
{

	addr a(1, 2, 3);
	BOOST_CHECK_EQUAL( a.zone(), 1 );
	BOOST_CHECK_EQUAL( a.cluster(), 2 );
	BOOST_CHECK_EQUAL( a.node(), 3 );

	a.zone(3);
	BOOST_CHECK_EQUAL( a.zone(), 3 );
	BOOST_CHECK_EQUAL( a.cluster(), 2 );
	BOOST_CHECK_EQUAL( a.node(), 3 );

	a.cluster(1);
	BOOST_CHECK_EQUAL( a.zone(), 3 );
	BOOST_CHECK_EQUAL( a.cluster(), 1 );
	BOOST_CHECK_EQUAL( a.node(), 3 );

	a.node(2);
	BOOST_CHECK_EQUAL( a.zone(), 3 );
	BOOST_CHECK_EQUAL( a.cluster(), 1 );
	BOOST_CHECK_EQUAL( a.node(), 2 );

	addr a2(3, 1, 2);
	BOOST_CHECK_EQUAL( a, a2 );

	addr a3(static_cast<uint32_t>(a2));
	BOOST_CHECK_EQUAL( a3, a2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_addr_test )
{

	const addr a(1, 2, 3);
	BOOST_CHECK_EQUAL( a.zone(), 1 );
	BOOST_CHECK_EQUAL( a.cluster(), 2 );
	BOOST_CHECK_EQUAL( a.node(), 3 );

	const addr a2(1, 2, 3);
	BOOST_CHECK_EQUAL( a, a2 );

	const addr a3(static_cast<uint32_t>(a2));
	BOOST_CHECK_EQUAL( a3, a2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_portid_test )
{

	portid id(55, addr(1, 2, 3));
	BOOST_CHECK_EQUAL( id.ref, 55 );
	BOOST_CHECK_EQUAL( id.node, addr(1, 2, 3) );
	BOOST_CHECK_EQUAL( id.node_addr().zone(), 1 );
	BOOST_CHECK_EQUAL( id.node_addr().cluster(), 2 );
	BOOST_CHECK_EQUAL( id.node_addr().node(), 3 );

	id.node_addr().zone(3);
	BOOST_CHECK_EQUAL( id.node_addr().zone(), 3 );
	BOOST_CHECK_EQUAL( id.node_addr().cluster(), 2 );
	BOOST_CHECK_EQUAL( id.node_addr().node(), 3 );

	portid id2(55, addr(3, 2, 3));
	BOOST_CHECK_EQUAL( id, id2 );

	portid id3(id);
	BOOST_CHECK_EQUAL( id3, id2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_portid_test )
{

	const portid id(55, addr(1, 2, 3));
	BOOST_CHECK_EQUAL( id.ref, 55 );
	BOOST_CHECK_EQUAL( id.node, addr(1, 2, 3) );
	BOOST_CHECK_EQUAL( id.node_addr().zone(), 1 );
	BOOST_CHECK_EQUAL( id.node_addr().cluster(), 2 );
	BOOST_CHECK_EQUAL( id.node_addr().node(), 3 );

	const portid id2(55, addr(1, 2, 3));
	BOOST_CHECK_EQUAL( id, id2 );

	portid id3(id);
	BOOST_CHECK_EQUAL( id3, id2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_name_test )
{

	name n(598, 2937);
	BOOST_CHECK_EQUAL( n.type, 598 );
	BOOST_CHECK_EQUAL( n.instance, 2937 );

	n.type = 9834;
	BOOST_CHECK_EQUAL( n.type, 9834 );
	BOOST_CHECK_EQUAL( n.instance, 2937 );

	n.instance = 3324;
	BOOST_CHECK_EQUAL( n.type, 9834 );
	BOOST_CHECK_EQUAL( n.instance, 3324 );

	name n2(9834, 3324);
	BOOST_CHECK_EQUAL( n, n2 );

	name n3(n);
	BOOST_CHECK_EQUAL( n3, n2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_name_test )
{

	const name n(598, 2937);
	BOOST_CHECK_EQUAL( n.type, 598 );
	BOOST_CHECK_EQUAL( n.instance, 2937 );

	const name n2(598, 2937);
	BOOST_CHECK_EQUAL( n, n2 );

	const name n3(n);
	BOOST_CHECK_EQUAL( n3, n2 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_nameseq_test )
{

	nameseq n(598, 2937, 666);
	BOOST_CHECK_EQUAL( n.type, 598 );
	BOOST_CHECK_EQUAL( n.lower, 2937 );
	BOOST_CHECK_EQUAL( n.upper, 666 );

	n.type = 9834;
	BOOST_CHECK_EQUAL( n.type, 9834 );
	BOOST_CHECK_EQUAL( n.lower, 2937 );
	BOOST_CHECK_EQUAL( n.upper, 666 );

	n.lower = 3324;
	BOOST_CHECK_EQUAL( n.type, 9834 );
	BOOST_CHECK_EQUAL( n.lower, 3324 );
	BOOST_CHECK_EQUAL( n.upper, 666 );

	n.upper = 9999;
	BOOST_CHECK_EQUAL( n.type, 9834 );
	BOOST_CHECK_EQUAL( n.lower, 3324 );
	BOOST_CHECK_EQUAL( n.upper, 9999 );

	nameseq n2(9834, 3324, 9999);
	BOOST_CHECK_EQUAL( n, n2 );

	nameseq n3(n);
	BOOST_CHECK_EQUAL( n3, n2 );

	nameseq n4(9834, 3324);
	BOOST_CHECK_EQUAL( n4.type, 9834 );
	BOOST_CHECK_EQUAL( n4.lower, 3324 );
	BOOST_CHECK_EQUAL( n4.upper, 3324 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_nameseq_test )
{

	const nameseq n(598, 2937, 666);
	BOOST_CHECK_EQUAL( n.type, 598 );
	BOOST_CHECK_EQUAL( n.lower, 2937 );
	BOOST_CHECK_EQUAL( n.upper, 666 );

	const nameseq n2(598, 2937, 666);
	BOOST_CHECK_EQUAL( n, n2 );

	const nameseq n3(n);
	BOOST_CHECK_EQUAL( n3, n2 );

	const nameseq n4(9834, 3324);
	BOOST_CHECK_EQUAL( n4.type, 9834 );
	BOOST_CHECK_EQUAL( n4.lower, 3324 );
	BOOST_CHECK_EQUAL( n4.upper, 3324 );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_subscr_test )
{

	subscr s(nameseq(999, 888), 12345, SUB_PORTS, "handle");
	BOOST_CHECK_EQUAL( s.seq.type, 999 );
	BOOST_CHECK_EQUAL( s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( s.seq.upper, 888 );
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( s.timeout, 12345 );
	BOOST_CHECK_EQUAL( s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s.usr_handle, "handle" );

	s.name_seq().upper = 2888;
	BOOST_CHECK_EQUAL( s.seq.type, 999 );
	BOOST_CHECK_EQUAL( s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( s.seq.upper, 2888 );
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888, 2888) );
	BOOST_CHECK_EQUAL( s.timeout, 12345 );
	BOOST_CHECK_EQUAL( s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s.usr_handle, "handle" );

	s.timeout = 9876;
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888, 2888) );
	BOOST_CHECK_EQUAL( s.timeout, 9876 );
	BOOST_CHECK_EQUAL( s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s.usr_handle, "handle" );

	s.filter = SUB_SERVICE;
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888, 2888) );
	BOOST_CHECK_EQUAL( s.timeout, 9876 );
	BOOST_CHECK_EQUAL( s.filter, SUB_SERVICE );
	BOOST_CHECK_EQUAL( s.usr_handle, "handle" );

	s.handle("other");
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888, 2888) );
	BOOST_CHECK_EQUAL( s.timeout, 9876 );
	BOOST_CHECK_EQUAL( s.filter, SUB_SERVICE );
	BOOST_CHECK_EQUAL( s.usr_handle, "other" );

	subscr s2(nameseq(999, 888, 2888), 9876, SUB_SERVICE, "other");
	BOOST_CHECK_EQUAL( s, s2 );

	subscr s3(s);
	BOOST_CHECK_EQUAL( s3, s2 );

	subscr s4(nameseq(999, 888), 12345, SUB_PORTS);
	BOOST_CHECK_EQUAL( s4.seq.type, 999 );
	BOOST_CHECK_EQUAL( s4.seq.lower, 888 );
	BOOST_CHECK_EQUAL( s4.seq.upper, 888 );
	BOOST_CHECK_EQUAL( s4.name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( s4.timeout, 12345 );
	BOOST_CHECK_EQUAL( s4.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s4.usr_handle, "" );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_subscr_test )
{

	const subscr s(nameseq(999, 888), 12345, SUB_PORTS, "handle");
	BOOST_CHECK_EQUAL( s.seq.type, 999 );
	BOOST_CHECK_EQUAL( s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( s.seq.upper, 888 );
	BOOST_CHECK_EQUAL( s.name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( s.timeout, 12345 );
	BOOST_CHECK_EQUAL( s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s.usr_handle, "handle" );

	const subscr s2(nameseq(999, 888), 12345, SUB_PORTS, "handle");
	BOOST_CHECK_EQUAL( s, s2 );

	const subscr s3(s);
	BOOST_CHECK_EQUAL( s3, s2 );

	const subscr s4(nameseq(999, 888), 12345, SUB_PORTS);
	BOOST_CHECK_EQUAL( s4.seq.type, 999 );
	BOOST_CHECK_EQUAL( s4.seq.lower, 888 );
	BOOST_CHECK_EQUAL( s4.seq.upper, 888 );
	BOOST_CHECK_EQUAL( s4.name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( s4.timeout, 12345 );
	BOOST_CHECK_EQUAL( s4.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( s4.usr_handle, "" );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_subscr_event_test )
{

	subscr_event e;
	e.event = PUBLISHED;
	e.found_lower = 888;
	e.found_upper = 1888;
	e.port.ref = 12345;
	e.port.node = addr(0, 2, 340);
	e.s.seq.type = 999;
	e.s.seq.lower = 888;
	e.s.seq.upper = 888;
	e.s.timeout = 12345;
	e.s.filter = SUB_PORTS;
	strcpy(e.s.usr_handle, "handle");
	BOOST_CHECK_EQUAL( e.event, PUBLISHED );
	BOOST_CHECK_EQUAL( e.found_lower, 888 );
	BOOST_CHECK_EQUAL( e.found_upper, 1888 );
	BOOST_CHECK_EQUAL( e.port.ref, 12345 );
	BOOST_CHECK_EQUAL( e.port.node, 8532 );
	BOOST_CHECK_EQUAL( e.port_id(), portid(12345, addr(0, 2, 340)) );
	BOOST_CHECK_EQUAL( e.s.seq.type, 999 );
	BOOST_CHECK_EQUAL( e.s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( e.s.seq.upper, 888 );
	BOOST_CHECK_EQUAL( e.subscription().name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( e.s.timeout, 12345 );
	BOOST_CHECK_EQUAL( e.s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( e.s.usr_handle, "handle" );

	e.port_id().node_addr().zone(1);
	BOOST_CHECK_EQUAL( e.event, PUBLISHED );
	BOOST_CHECK_EQUAL( e.found_lower, 888 );
	BOOST_CHECK_EQUAL( e.found_upper, 1888 );
	BOOST_CHECK_EQUAL( e.port.ref, 12345 );
	BOOST_CHECK_EQUAL( e.port.node, 16785748 );
	BOOST_CHECK_EQUAL( e.port_id(), portid(12345, addr(1, 2, 340)) );
	BOOST_CHECK_EQUAL( e.s.seq.type, 999 );
	BOOST_CHECK_EQUAL( e.s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( e.s.seq.upper, 888 );
	BOOST_CHECK_EQUAL( e.subscription().name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( e.s.timeout, 12345 );
	BOOST_CHECK_EQUAL( e.s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( e.s.usr_handle, "handle" );

	e.subscription().name_seq().upper = 1888;
	BOOST_CHECK_EQUAL( e.event, PUBLISHED );
	BOOST_CHECK_EQUAL( e.found_lower, 888 );
	BOOST_CHECK_EQUAL( e.found_upper, 1888 );
	BOOST_CHECK_EQUAL( e.port.ref, 12345 );
	BOOST_CHECK_EQUAL( e.port.node, 16785748 );
	BOOST_CHECK_EQUAL( e.port_id(), portid(12345, addr(1, 2, 340)) );
	BOOST_CHECK_EQUAL( e.s.seq.type, 999 );
	BOOST_CHECK_EQUAL( e.s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( e.s.seq.upper, 1888 );
	BOOST_CHECK_EQUAL( e.subscription().name_seq(),
				nameseq(999, 888, 1888) );
	BOOST_CHECK_EQUAL( e.s.timeout, 12345 );
	BOOST_CHECK_EQUAL( e.s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( e.s.usr_handle, "handle" );

	e.subscription().handle("lala");
	BOOST_CHECK_EQUAL( e.event, PUBLISHED );
	BOOST_CHECK_EQUAL( e.found_lower, 888 );
	BOOST_CHECK_EQUAL( e.found_upper, 1888 );
	BOOST_CHECK_EQUAL( e.port.ref, 12345 );
	BOOST_CHECK_EQUAL( e.port.node, 16785748 );
	BOOST_CHECK_EQUAL( e.port_id(), portid(12345, addr(1, 2, 340)) );
	BOOST_CHECK_EQUAL( e.s.seq.type, 999 );
	BOOST_CHECK_EQUAL( e.s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( e.s.seq.upper, 1888 );
	BOOST_CHECK_EQUAL( e.subscription().name_seq(),
				nameseq(999, 888, 1888) );
	BOOST_CHECK_EQUAL( e.s.timeout, 12345 );
	BOOST_CHECK_EQUAL( e.s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( e.s.usr_handle, "lala" );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_const_event_test )
{

	subscr_event e;
	e.event = PUBLISHED;
	e.found_lower = 888;
	e.found_upper = 1888;
	e.port.ref = 12345;
	e.port.node = 8532;
	e.s.seq.type = 999;
	e.s.seq.lower = 888;
	e.s.seq.upper = 888;
	e.s.timeout = 12345;
	e.s.filter = SUB_PORTS;
	strcpy(e.s.usr_handle, "handle");
	const subscr_event& ee = e;
	BOOST_CHECK_EQUAL( ee.event, PUBLISHED );
	BOOST_CHECK_EQUAL( ee.found_lower, 888 );
	BOOST_CHECK_EQUAL( ee.found_upper, 1888 );
	BOOST_CHECK_EQUAL( ee.port.ref, 12345 );
	BOOST_CHECK_EQUAL( ee.port.node, 8532 );
	BOOST_CHECK_EQUAL( ee.port_id(), portid(12345, addr(0, 2, 340)) );
	BOOST_CHECK_EQUAL( ee.s.seq.type, 999 );
	BOOST_CHECK_EQUAL( ee.s.seq.lower, 888 );
	BOOST_CHECK_EQUAL( ee.s.seq.upper, 888 );
	BOOST_CHECK_EQUAL( ee.subscription().name_seq(), nameseq(999, 888) );
	BOOST_CHECK_EQUAL( ee.s.timeout, 12345 );
	BOOST_CHECK_EQUAL( ee.s.filter, SUB_PORTS );
	BOOST_CHECK_EQUAL( ee.s.usr_handle, "handle" );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_testaddr_test )
{
	BOOST_CHECK_EQUAL(test_address1.addr.name.name.type, PTYPE);
	BOOST_CHECK_EQUAL(test_address1.addr.name.name.instance, INST1);
	BOOST_CHECK_EQUAL(test_address2.addr.name.name.type, PTYPE);
	BOOST_CHECK_EQUAL(test_address2.addr.name.name.instance, INST2);
}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_default_ctor_test )
{
	tipc::sockaddr sa;
	BOOST_CHECK_EQUAL( sa.family, AF_TIPC );
	BOOST_CHECK_EQUAL( sa.addrtype, 0 );
	BOOST_CHECK_EQUAL( sa.scope, 0 );
	BOOST_CHECK_EQUAL( sa.name_seq(), nameseq(0, 0, 0) );
	BOOST_CHECK_EQUAL( sa.length(), sizeof(sockaddr_tipc) );
}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_id_ctor_test )
{

	portid id(12313, addr(1, 1, 77));
	tipc::sockaddr sp(id);
	BOOST_CHECK_EQUAL( sp.family, AF_TIPC );
	BOOST_CHECK_EQUAL( sp.addrtype, ID );
	BOOST_CHECK_EQUAL( sp.type(), ID );
	BOOST_CHECK_EQUAL( sp.scope, ZONE );
	BOOST_CHECK_EQUAL( sp.port_id(), id );
	BOOST_CHECK_EQUAL( sp.addr.id.ref, id.ref );
	BOOST_CHECK_EQUAL( sp.addr.id.node, id.node );

	sp.port_id().node_addr().zone(2);
	BOOST_CHECK_EQUAL( sp.port_id(), portid(12313, addr(2, 1, 77)) );

	BOOST_CHECK_EQUAL( sp, tipc::sockaddr(portid(12313, addr(2, 1, 77))) );

	tipc::sockaddr sp2(id, NODE);
	BOOST_CHECK_EQUAL( sp2.family, AF_TIPC );
	BOOST_CHECK_EQUAL( sp2.addrtype, ID );
	BOOST_CHECK_EQUAL( sp2.type(), ID );
	BOOST_CHECK_EQUAL( sp2.scope, NODE );
	BOOST_CHECK_EQUAL( sp2.port_id(), id );
	BOOST_CHECK_EQUAL( sp2.addr.id.ref, id.ref );
	BOOST_CHECK_EQUAL( sp2.addr.id.node, id.node );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_name_ctor_test )
{

	name n(12313, 77);
	tipc::sockaddr sn(n);
	BOOST_CHECK_EQUAL( sn.family, AF_TIPC );
	BOOST_CHECK_EQUAL( sn.addrtype, NAME );
	BOOST_CHECK_EQUAL( sn.type(), NAME );
	BOOST_CHECK_EQUAL( sn.scope, ZONE );
	BOOST_CHECK_EQUAL( sn.port_name(), n );
	BOOST_CHECK_EQUAL( sn.addr.name.name.type, n.type );
	BOOST_CHECK_EQUAL( sn.addr.name.name.instance, n.instance );
	BOOST_CHECK_EQUAL( sn.name_domain(), addr(0u) );
	BOOST_CHECK_EQUAL( sn.addr.name.domain, 0u );

	sn.port_name().type = 999;
	BOOST_CHECK_EQUAL( sn.port_name(), name(999, 77) );

	sn.name_domain().node(999);
	BOOST_CHECK_EQUAL( sn.name_domain(), addr(0, 0, 999) );

	BOOST_CHECK_EQUAL( sn, tipc::sockaddr(name(999, 77), ZONE,
				addr(0, 0, 999)) );

}

BOOST_AUTO_TEST_CASE( linux_tipc_sockaddr_nameseq_ctor_test )
{

	nameseq ns(12313, 77);
	tipc::sockaddr sns(ns);
	BOOST_CHECK_EQUAL( sns.family, AF_TIPC );
	BOOST_CHECK_EQUAL( sns.addrtype, NAMESEQ );
	BOOST_CHECK_EQUAL( sns.type(), NAMESEQ );
	BOOST_CHECK_EQUAL( sns.scope, ZONE );
	BOOST_CHECK_EQUAL( sns.name_seq(), ns );
	BOOST_CHECK_EQUAL( sns.addr.nameseq.type, ns.type );
	BOOST_CHECK_EQUAL( sns.addr.nameseq.lower, ns.lower );
	BOOST_CHECK_EQUAL( sns.addr.nameseq.upper, ns.upper );

	sns.name_seq().upper = 1999;
	BOOST_CHECK_EQUAL( sns.name_seq(), nameseq(12313, 77, 1999) );

	BOOST_CHECK_EQUAL( sns, tipc::sockaddr(nameseq(12313, 77, 1999)) );

}

