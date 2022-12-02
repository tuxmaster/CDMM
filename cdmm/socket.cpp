//======================================================================
// File:        socket.cpp
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

//#include <netinet/tcp.h>
#include "socket.h"
#include "string.h"
#include <cstring>
#include <errno.h>
#include <fcntl.h>

#include <iostream>

Socket::Socket() :
  m_sock ( -1 )
{
#ifndef __WIN32
  memset( &m_addr, 0, sizeof ( m_addr ) );
#endif
}

Socket::~Socket()
{
  if ( is_valid() ) ::close ( m_sock );
}

bool Socket::create()
{
#ifndef __WIN32
  m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );

  if ( !is_valid() ) return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, 
                   ( const char* ) &on, sizeof ( on ) ) == -1 )
  {
    return false;
  }
#endif
  return true;
}

bool Socket::bind( const int port )
{
  if ( !is_valid() ) return false;

#ifndef __WIN32
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 ) return false;
#endif

  return true;
}

bool Socket::listen() const
{
  if ( !is_valid() ) return false;

#ifndef __WIN32
  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 ) return false;
#endif

  return true;
}

bool Socket::accept ( Socket &new_socket ) const
{
#ifndef __WIN32
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, 
                                 ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
  {
    return false;
  }
#else
#endif

  return true;
}

bool Socket::send( const char *data, int len ) const
{
#ifndef __WIN32
  int status = ::send( m_sock, data, len, MSG_NOSIGNAL );
  
  if ( status == -1 )
  {
    return false;
  }
  else
  {
    return true;
  }
#else
  return true;
#endif
}

int Socket::recv( char *buf ) const
{
#ifndef __WIN32
  memset( buf, 0, MAXRECV+1 );
  int selResult = ::recv( m_sock, buf, MAXRECV, 0 );
  
  return selResult;
#else
  return 42;
#endif
}

int Socket::readByte( char *byte ) const
{
#ifndef __WIN32
  *byte = 0;
  int selResult = ::recv( m_sock, byte, 1, 0 );
  
  return selResult;
#else
  return 42;
#endif
}

bool Socket::connect( const char *host, const int port )
{
  if ( !is_valid() ) return false;

#ifndef __WIN32
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons( port );

  int status = inet_pton( AF_INET, host, &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect( m_sock, (sockaddr *)&m_addr, sizeof( m_addr ) );

  if ( status == 0 )
  {
    //set_non_blocking( true );
    return true;
  }
  else
  {
    return false;
  }
#else
  return true;
#endif
}

void Socket::set_non_blocking ( const bool b )
{
#ifndef __WIN32
  int opts;

  opts = ::fcntl( m_sock, F_GETFL );

  if ( opts < 0 ) return;

  if ( b )
  {
    opts = ( opts | O_NONBLOCK );
  }
  else
  {
    opts = ( opts & ~O_NONBLOCK );
  }

  fcntl ( m_sock, F_SETFL, opts );
#else
#endif
}
