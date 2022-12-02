//======================================================================
// File:        udpserver.h
// Author:      Mark Sheahan
// Created:     26 Jan 2012
// Project:     QtDMM
// Description: Encapsulates a udp connection
//----------------------------------------------------------------------
// This file  may  be used under  the terms of  the GNU  General Public
// License  version 2.0 as published   by the Free Software  Foundation
// and appearing  in the file LICENSE.GPL included  in the packaging of
// this file.
// 
// This file is provided AS IS with  NO WARRANTY OF ANY KIND, INCLUDING 
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE.
//----------------------------------------------------------------------
// Copyright 2012 Mark Sheahan
//======================================================================

#ifndef UDPSERVER_HH
#define UDPSERVER_HH

#include <serversocket.h>
#include <thread.h>
#include <dmmclass.h>

#define UDP_BUFSIZE	2048

class UdpServer : public Thread
{
	public:
		UdpServer( DMMClass *dmm, int port );
		virtual ~UdpServer();

	protected:
		DMMClass     *m_dmm;
		int m_port;
		int m_socket_fd;
		char m_rxbuf[UDP_BUFSIZE];
		char m_txbuf[UDP_BUFSIZE];

		virtual void run();

};

#endif // UDPSERVER_HH


