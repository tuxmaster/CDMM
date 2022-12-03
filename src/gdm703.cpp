//======================================================================
// File:        gdm703.cpp
// Author:      Matthias Toussaint
// Created:     Tue Nov 28 08:24:21 CET 2006
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

#include "gdm703.h"

#include <iostream>

GDM703::GDM703() :
  DMMClass()
{
  setNumValues( 2 );
}

GDM703::~GDM703()
{
  m_port.close();
}

Port::Error GDM703::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error GDM703::close()
{
  return m_port.close();
}

void GDM703::setAscii( int start, int modeLen,
						   const std::string & str, int index )
{
  // TODO: overflow
  m_mutex.lock();

  setValue( index, Util::strip_whitespace( str.substr( start+2, 6 )) );
  m_mode[index]   = Util::strip_whitespace( str.substr( start, modeLen ));
  m_prefix[index] = Util::strip_whitespace( str.substr( start+modeLen+6, 1 ));
  m_unit[index]   = Util::strip_whitespace( str.substr( start+modeLen+7, 3 ));

  m_hasValue = true;
  m_overflow[index] = false;

  m_mutex.unlock();
}

// TODO
void GDM703::run()
{
  char data[26*3+1];

  while (m_run)
  {
	int cnt = readData( &m_port, (unsigned char *)data,
						0x03, 26, 3*26 );

	if (-1 != cnt)
	{
	  int start = cnt-26;
	  data[cnt-1] = 0;
	  std::string strData = data+start;
	  //std::cerr << strData << std::endl;

	  setAscii( 1, 2, strData, 0 );
	  setAscii( 14, 1, strData, 1 );
	}
	else m_hasValue = false;
  }
}
