//======================================================================
// File:        udpserver.cpp
// Author:      Mark Sheahan
// Created:     Thu Jan 26 22:03:33 EST 2012
// Project:     QtDMM
// Description: Encapsulates a threaded udp server
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

#include "udpserver.h"
#include <iostream>
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

UdpServer::UdpServer( DMMClass *dmm, int port ) :
  m_dmm( dmm ),
  m_port( port )
{
	// open a socket on port
	struct sockaddr_in saddr_in;
		socklen_t socklen = sizeof(struct sockaddr_in);
		memset(&saddr_in, 0, socklen);
		saddr_in.sin_family = AF_INET;
		saddr_in.sin_addr.s_addr = INADDR_ANY;
		saddr_in.sin_port = htons(port);
		int errors = 0;
		if (!errors && (m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				fprintf(stderr, "%s Error opening socket: %d %s\n", __FUNCTION__, errno, strerror(errno));
				errors = 1;
		}

	if (!errors && bind(m_socket_fd, (struct sockaddr*)&saddr_in, socklen) < 0) {
		fprintf(stderr, "%s Error binding socket: %d %s\n", __FUNCTION__, errno, strerror(errno));
		errors = 1;
	}
}

UdpServer::~UdpServer()
{
	// close socket
	close(m_socket_fd);
}

void UdpServer::run()
{
	try
	{
		while (m_run)
		{
			struct sockaddr_in client_saddr;	// client calling us
			socklen_t addrlen = sizeof(struct sockaddr_in);
			int ret;

			ret = recvfrom(m_socket_fd, m_rxbuf, UDP_BUFSIZE, 0, (struct sockaddr*)&client_saddr, &addrlen);
			if (ret < 0) {
				fprintf(stderr, "Error calling recvfrom: %d %s\n", errno, strerror(errno));
			} else {
				char *p = m_txbuf;
				if (m_dmm->hasValue()) {
					if (m_dmm->overflow(0)) {
						p += sprintf(p, "Overflow");
					} else {
						p += sprintf(p, "%s", m_dmm->value(0).c_str());
					}
					p += sprintf(p, " %s %s\n", m_dmm->unit(0).c_str(), m_dmm->mode(0).c_str());
				}
				int sendlen = strlen(m_txbuf) + 1;
				ret = sendto(m_socket_fd, m_txbuf, sendlen, 0, (struct sockaddr*)&client_saddr, addrlen);
				if (ret < 0) {
					fprintf(stderr, "Error calling sendto: %d %s\n", errno, strerror(errno));
				}
			}
		}
	}
	catch (...)
	{
		fprintf(stderr, "%s:%s caught exception\n", __FILE__, __FUNCTION__);
		m_run = false;
	}
}
