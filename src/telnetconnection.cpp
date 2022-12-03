//======================================================================
// File:        telnetconnection.cpp
// Author:      Matthias Toussaint
// Created:     Sun Sep  9 10:11:13 CEST 2007
// Project:     QtDMM
// Description: Encapsulates a telnet connection
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

#include "telnetconnection.h"
#include "socketexception.h"
#include <iostream>
#include "util.h"
#include <stdlib.h>

TelnetConnection::TelnetConnection( DMMClass *dmm, ServerSocket *socket ) :
  m_dmm( dmm ),
  m_socket( socket )
{
}

TelnetConnection::~TelnetConnection()
{
  delete m_socket;
}

void TelnetConnection::run()
{
  bool output = false;
  int interval = 100000;

  try
  {
	while (m_run)
	{
#ifndef __WIN32
	  fd_set readset;
	  FD_ZERO( &readset );
	  FD_SET( m_socket->socket(), &readset );
	  struct timeval timeout;
	  timeout.tv_sec = 0;
	  timeout.tv_usec = interval;
	  int selret;

	  if ((selret=select( m_socket->socket() + 1, &readset, NULL, NULL, &timeout )) > 0)
	  {
		std::string input = m_socket->readLine();

		TokenList token = Util::tokenize( input, " " );

		// silly parser (tm)
		if (token.size())
		{
		  if (token[0] == "quit" || token[0] == "bye")
		  {
			throw SocketException( "Quit" );
		  }
		  else if (token[0] == "stop")
		  {
			output = false;
		  }
		  else if (token[0] == "start")
		  {
			int idt = 1;
			if (token.size() > 1)
			{
			  double dt = atof( token[1].c_str() );
			  idt = (int)(dt*10.0);
			  if (idt < 1) idt = 1;
			}

			interval = idt * 100000;
			output = true;
		  }
		  else if (token[0] == "help")
		  {
			*m_socket << "Available commands\n";
			*m_socket << "  start [interval in seconds (multiple of 0.1s)]\n";
			*m_socket << "  stop\n";
			*m_socket << "  bye|quit\n";
			*m_socket << "  info\n";
			*m_socket << "  help\n";
		  }
		}
	  }
#endif

	  if (output && m_dmm->hasValue())
	  {
		for (int i=0; i<m_dmm->numValues(); ++i)
		{
		  *m_socket << "V";
		  *m_socket << i;
		  *m_socket << ": ";

		  //if (showDate) printDate( utc );
		  //if (showTime) printTime( utc );

		  /*if (showRange)
		  {
			*m_socket << dmm->minValue(i) << " " << dmm->maxValue(i) << " ";
		}*/

		  if (!m_dmm->overflow(i))
		  {
			*m_socket << m_dmm->value(i);
		  }
		  else
		  {
			*m_socket << "Overflow";
		  }

		  *m_socket << " ";
		  *m_socket << m_dmm->unit(i);
		  *m_socket << " ";
		  *m_socket << m_dmm->mode(i);
		  *m_socket << "\n";
		}
	  }
	}
  }
  catch (...)
  {
	delete m_socket;
	m_socket = 0;
	m_run = false;
  }
}
