//======================================================================
// File:        escort97.cpp
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 18:53:43 CET 2006
// Project:     QtDMM
// Description: Encapsulates a DMM class (protokoll)
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
// Copyright 2006 Matthias Toussaint
//======================================================================

#include <escort97.h>

#include <iostream>

Escort97::Escort97() :
  DMMClass()
{
  setNumValues( 2 );
}

Escort97::~Escort97()
{
}

Port::Error Escort97::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error Escort97::close()
{
  return m_port.close();
}

// TODO
void Escort97::run()
{
  unsigned char data[128];
  
  while (m_run)
  {
    int cnt = readData( &m_port, (unsigned char *)data, 
                         0x00, 21, 127 );

  }
}

std::string Escort97::byteToDigit( unsigned char byte )
{
  std::string retval;
  
  switch (byte)
  {
    case 0x20:
      retval = "0";
      break;
    case 0x21:
      retval = "1";
      break;
    case 0x22:
      retval = "2";
      break;
    case 0x23:
      retval = "3";
      break;
    case 0x24:
      retval = "4";
      break;
    case 0x25:
      retval = "5";
      break;
    case 0x26:
      retval = "6";
      break;
    case 0x27:
      retval = "7";
      break;
    case 0x28:
      retval = "8";
      break;
    case 0x2a:
      retval = "9";
      break;
  }

  return retval;
}
