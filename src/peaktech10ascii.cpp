//======================================================================
// File:        peaktech10ascii.cpp
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 14:46:41 CET 2006
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

#include "peaktech10ascii.h"

#include <iostream>

PeakTech10Ascii::PeakTech10Ascii() :
  DMMClass()
{
}

PeakTech10Ascii::~PeakTech10Ascii()
{
  m_port.close();
}

Port::Error PeakTech10Ascii::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error PeakTech10Ascii::close()
{
  return m_port.close();
}

// TODO
void PeakTech10Ascii::run()
{
  char data[64];

  while (m_run)
  {
	int cnt = readData( data );

	if (-1 != cnt)
	{
	  char *bytes = data+cnt-11;
	  bytes[11] = 0;
	  std::string dataStr = bytes;
	  std::string value = dataStr.substr( 1, 6 );

	  m_mutex.lock();
	  setValue( 0, Util::strip_whitespace( value ) );
	  m_unit[0] = Util::strip_whitespace( dataStr.substr( 7, 4 ) );
	  m_mode[0] = "";

	  m_hasValue = true;
	  m_mutex.unlock();
	}
	else m_hasValue = false;
  }
}

int PeakTech10Ascii::readData( char *data ) const
{
  int cnt=0;
  int byte;
  bool gotcha = false;

  do
  {
	if (m_port.readByte( &byte ) != Port::Ok)
	{
	  return -1;
	}
	data[cnt++] = byte;

	if (byte == '#')
	{
	  gotcha = true;

	  for (int i=0; i<11; ++i)
	  {
		if (m_port.readByte( &byte ) != Port::Ok)
		{
		  return -1;
		}
		data[cnt++] = byte;
	  }

	  return cnt;
	}
  }
  while (cnt < 63 && !gotcha);

  return -1;
}
