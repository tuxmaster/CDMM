//======================================================================
// File:        socket.h
// Author:      Matthias Toussaint
// Created:     Sun Sep  9 10:11:13 CEST 2007
// Project:     QtDMM
// Description: Encapsulates of a TCP socket
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
// Copyright 2007 Matthias Toussaint
//======================================================================

#ifndef SOCKET_HH
#define SOCKET_HH

#ifdef __WIN32
	#include <winsock2.h>
	#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

#ifndef __WIN32
/** \brief Simple encapsulation of a socket (uses exceptions!)
*/
class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const char *host, const int port );

  // Data Transimission
  bool send( const char *data, int len ) const;
  /// data needs to be initialized with at least MAXRECV+1 bytes
  int recv( char *data ) const;
  int readByte( char * ) const;
  
  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }
  int socket() const { return m_sock; }
  
 private:

  int m_sock;
  sockaddr_in m_addr;

};
#else
/** \brief Simple encapsulation of a socket (uses exceptions!)
*/
class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const char *host, const int port );

  // Data Transimission
  bool send( const char *data, int len ) const;
  /// data needs to be initialized with at least MAXRECV+1 bytes
  int recv( char *data ) const;
  int readByte( char * ) const;

  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }
  int socket() const { return m_sock; }

 private:

  int m_sock;
  //sockaddr_in m_addr;

};

#endif

#endif // SOCKET_HH
