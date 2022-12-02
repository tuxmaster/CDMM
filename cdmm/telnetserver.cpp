//======================================================================
// File:        telnetserver.cpp
// Author:      Matthias Toussaint
// Created:     Sun Sep  9 10:11:13 CEST 2007
// Project:     QtDMM
// Description: Encapsulates a threaded telnet server
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

#include <telnetserver.h>
#include <iostream>

TelnetServer::TelnetServer( DMMClass *dmm, int port, int maxConnect ) :
  m_dmm( dmm ),
  m_port( port ),
  m_maxConnect( maxConnect )
{
  m_connection = new TelnetConnection * [ m_maxConnect ];
  for (int i=0; i<m_maxConnect; ++i) m_connection[i] =  0;
}

TelnetServer::~TelnetServer()
{
  for (int i=0; i<m_maxConnect; ++i) delete m_connection[i];
  
  delete [] m_connection;
}

TelnetConnection *TelnetServer::createConnection( ServerSocket *socket )
{
  for (int i=0; i<m_maxConnect; ++i)
  {
    if (0 == m_connection[i] || !m_connection[i]->running())
    {
      delete m_connection[i];
      m_connection[i] = new TelnetConnection( m_dmm, socket );
      m_connection[i]->start();
      return m_connection[i];
    }
  }
  
  return 0;
}

void TelnetServer::run()
{
  ServerSocket server( m_port );
  
  while (m_run)
  {
    ServerSocket *socket = new ServerSocket;
    server.accept( *socket );
    
    TelnetConnection *connection = createConnection( socket );
    
    if (!connection) 
    {
      *socket << "Too many connections\r\n";
      delete socket;
    }
  }
}
