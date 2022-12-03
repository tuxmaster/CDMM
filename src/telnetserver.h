//======================================================================
// File:        telnetserver.h
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

#ifndef TELNETSERVER_HH
#define TELNETSERVER_HH

#include "dmmclass.h"
#include "telnetconnection.h"

class TelnetServer : public Thread
{
  public:
	TelnetServer( DMMClass *dmm, int port, int maxConnect=3 );
	virtual ~TelnetServer();

  protected:
	DMMClass          *m_dmm;
	int                m_port;
	int                m_maxConnect;
	TelnetConnection **m_connection;

	virtual void run();

	TelnetConnection *createConnection( ServerSocket * );

};

#endif // TELNETSERVER_HH
