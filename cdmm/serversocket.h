//======================================================================
// File:        serversocket.h
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

#ifndef SERVERSOCKET_HH
#define SERVERSOCKET_HH

#include "socket.h"
#include <string>

class ServerSocket : private Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const std::string & data ) const;
  const ServerSocket& operator << ( const char *data ) const;
  const ServerSocket& operator << ( int ) const;
  
  const ServerSocket& operator >> ( std::string & data ) const;
  const ServerSocket& operator >> ( char *data ) const;

  void accept ( ServerSocket& );

  int socket() { return Socket::socket(); }
  
  std::string readLine();
  
};


#endif // SERVERSOCKET_HH
