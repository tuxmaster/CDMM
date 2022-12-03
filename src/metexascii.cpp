//======================================================================
// File:        metexascii.cpp
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 18:12:23 CET 2006
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

#include "metexascii.h"

#include <iostream>
#include <stdio.h>

MetexAscii::MetexAscii( const std::string & format,
						const std::string & overflowStr,
						bool poll, const std::string & pollString,
						int pollTime ) :
  DMMClass(),
  m_overflowStr( overflowStr ),
  m_poll( poll ),
  m_pollString( pollString ),
#ifndef __WIN32
  m_pollTime( pollTime*1000 )
#else
  m_pollTime( pollTime )
#endif
{
  // parse format
  //
  int byte = 0;
  m_length = format.size();
  m_syncByte = format[m_length-1];

  int *start=0;
  int *len=0;

  for (int i=0; i<m_length; ++i)
  {
	if (byte != format[i])
	{
	  if (len)
	  {
		*len = i-(*start);
	  }

	  if (format[i] == 'M')
	  {
		start = &m_modeStart;
		len   = &m_modeLen;
	  }
	  else if (format[i] == 'V')
	  {
		start = &m_valueStart;
		len   = &m_valueLen;
	  }
	  else if (format[i] == 'U')
	  {
		start = &m_unitStart;
		len   = &m_unitLen;
	  }
	  else
	  {
		start = 0;
		len = 0;
	  }

	  if (start)
	  {
		*start = i;
	  }

	  byte = format[i];
	}
  }
}

MetexAscii::~MetexAscii()
{
  m_port.close();
}

Port::Error MetexAscii::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error MetexAscii::close()
{
  return m_port.close();
}

void MetexAscii::setMetexAscii( const std::string & str, int index )
{
  m_mutex.lock();

  setValue( index, Util::strip_whitespace( str.substr( m_valueStart, m_valueLen )) );
  m_mode[index] = Util::strip_whitespace( str.substr( m_modeStart, m_modeLen ));
  m_unit[index] = Util::strip_whitespace( str.substr( m_unitStart, m_unitLen ));

  if (m_unit[0][0] == 'n' || m_unit[0][0] == 'u' || m_unit[0][0] == 'm' ||
	  m_unit[0][0] == 'k' || m_unit[0][0] == 'M')
  {
	m_prefix[0] = m_unit[0].substr( 0, 1 );
	m_unit[0] = m_unit[0].substr( 1, m_unit[0].size()-1 );
  }
  else m_prefix[0] = "";

  double value = atof( m_value[index].c_str() );
  if (m_prefix[index] == "p") value *= 1e-12;
  if (m_prefix[index] == "n") value *= 1e-9;
  if (m_prefix[index] == "u") value *= 1e-6;
  if (m_prefix[index] == "m") value *= 1e-3;
  if (m_prefix[index] == "k") value *= 1e3;
  if (m_prefix[index] == "M") value *= 1e6;
  if (m_prefix[index] == "G") value *= 1e9;
  m_realValue[index] = value;

  setHasValue();

  if (str.substr(m_valueStart,m_valueLen).find(m_overflowStr)<(unsigned)m_valueLen)
  {
	m_overflow[index]=true;
  }
  else m_overflow[index]=false;

  m_mutex.unlock();
}

void MetexAscii::run()
{
  char data[64];

  while (m_run)
  {
	if (m_poll)
	{
	  //fprintf( stderr, "Poll\n" );
	  m_port.writeString( m_pollString.c_str(), m_pollString.size() );
	}

	for (int i=0; i<m_numValues; ++i)
	{
	  int cnt = readData( &m_port, (unsigned char *)data,
						  m_syncByte, m_length, 3*m_length, 10 );

	  if (-1 != cnt)
	  {
		int start = cnt-m_length;
		data[cnt-1] = 0;
		std::string strData = data+start;
		//std::cerr << strData << std::endl;

		//fprintf( stderr, "set Data\n" );
		setMetexAscii( strData, i );
		m_mutex.lock();
		setRange(i);
		m_mutex.unlock();
	  }
	  else
	  {
		m_hasValue = false;
	  }
	}

	if (m_poll)
	{
#ifndef __WIN32
	  usleep( m_pollTime );
#else
	  Sleep( m_pollTime );
#endif
	}
  }
}
