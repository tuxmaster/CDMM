//======================================================================
// File:        serversocket.cpp
// Author:      Matthias Toussaint
// Created:     Sun Sep  9 10:11:13 CEST 2007
// Project:     QtDMM
// Description: Encapsulates a server socket
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

#include "serversocket.h"
#include "socketexception.h"
#include <errno.h>
#include <stdio.h>

#include <iostream>

ServerSocket::ServerSocket( int port )
{
  if ( !Socket::create() )
  {
	throw SocketException( "Could not create server socket." );
  }

  if ( !Socket::bind( port ) )
  {
	throw SocketException( "Could not bind to port." );
  }

  if ( !Socket::listen() )
  {
	throw SocketException( "Could not listen to socket." );
  }
}

ServerSocket::~ServerSocket()
{
}

const ServerSocket& ServerSocket::operator <<( int i ) const
{
  char data[256];
  sprintf( data, "%d", i );

  if ( !Socket::send( data, strlen( data ) ))
  {
	throw SocketException( "Could not write to socket." );
  }

  return *this;
}

const ServerSocket& ServerSocket::operator <<( const std::string & data ) const
{
  if ( !Socket::send( data.c_str(), data.size() ))
  {
	throw SocketException( "Could not write to socket." );
  }

  return *this;
}

const ServerSocket& ServerSocket::operator <<( const char *data ) const
{
  if ( !Socket::send( data, strlen( data ) ))
  {
	throw SocketException( "Could not write to socket." );
  }

  return *this;
}

std::string ServerSocket::readLine()
{
  std::string str;
  char c;
  int result;

  do
  {
	result = Socket::readByte( &c );

	if (result > 0 &&  c != '\n' && c != '\r') str += c;
  }
  while (result > 0 && c != '\n' && c != '\r');

  return str;
}

const ServerSocket& ServerSocket::operator >>( std::string & data ) const
{
  char tmpData[MAXRECV];

  int recv_return = Socket::recv( tmpData );

  if ( recv_return <= 0 )
  {
	throw SocketException( "Could not read from socket." );
  }

  tmpData[recv_return] = 0;

  data = tmpData;

  return *this;
}

const ServerSocket& ServerSocket::operator >>( char *data ) const
{
  int recv_return = Socket::recv( data );

  if ( recv_return <= 0 )
  {
	throw SocketException( "Could not read from socket." );
  }

  data[recv_return] = 0;

  return *this;
}

void ServerSocket::accept( ServerSocket& sock )
{
  if ( !Socket::accept ( sock ) )
  {
	throw SocketException( "Could not accept socket." );
  }
}
