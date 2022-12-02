//======================================================================
// File:        socketexception.h
// Author:      Matthias Toussaint
// Created:     Sun Sep  9 10:11:13 CEST 2007
// Project:     QtDMM
// Description: Exceptions for TCP socket
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

#ifndef SOCKETEXCEPTION_HH
#define SOCKETEXCEPTION_HH

#include <cstring>

class SocketException
{
public:
  SocketException( const char *s ) { m_s = new char [strlen(s)]; strcpy( m_s, s ); }
  ~SocketException() { delete [] m_s; };

  const char *description() { return m_s; }

private:
  char *m_s;
   
};

#endif // SOCKETEXCEPTION_HH
