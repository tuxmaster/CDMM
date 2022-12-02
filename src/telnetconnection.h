//======================================================================
// File:        telnetconnection.h
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

#ifndef TELNETCONNECTION_HH
#define TELNETCONNECTION_HH

#include <serversocket.h>
#include <thread.h>
#include <dmmclass.h>

class TelnetConnection : public Thread
{
  public:
    TelnetConnection( DMMClass *dmm, ServerSocket *socket );
    virtual ~TelnetConnection();
    
  protected:
    DMMClass     *m_dmm;
    ServerSocket *m_socket;
    
    virtual void run();
    
};

#endif // TELNETCONNECTION_HH

