//======================================================================
// File:        vc870.cpp
// Author:      Matthias Toussaint
// Created:     Sat Dec 10 20:06:02 CET 2011
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
// Copyright 2011 Matthias Toussaint
//======================================================================

#include "vc870.h"

#include <iostream>
#include <stdio.h>

VC870::VC870() :
  DMMClass()
{
  m_numCounts = 40000;
}

VC870::~VC870()
{
  m_port.close();
}

Port::Error VC870::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error VC870::close()
{
  return m_port.close();
}

void VC870::run()
{
  unsigned char buffer[64];

  while (m_run)
  {
	int cnt = readData( buffer );

	if (-1 != cnt)
	{
	  unsigned char *byte = buffer+cnt-14;

	  m_mutex.lock();

	  // assemble digits
	  //
	  std::string digits;

	  m_overflow[0] = findOverflow( byte );

	  m_rangeMax[0] = 4000;
	  double freqMax = 10000;


	  // low battery
	  //
	  if (byte[12] & 0x01) m_lowBat = true;
	  else                 m_lowBat = false;

	  double scale = 1.0;

	  // prefix
	  //
	  if      (byte[9] & 0x02)
	  {
		m_prefix[0] = "k";
		scale = 1e3;
	  }
	  else if (byte[9] & 0x04)
	  {
		m_prefix[0] = "n";
		scale = 1e-9;
	  }
	  else if (byte[9] & 0x08)
	  {
		m_prefix[0] = "u";
		scale = 1e-6;
	  }
	  else if (byte[10] & 0x02)
	  {
		m_prefix[0] = "M";
		scale = 1e6;
	  }
	  else if (byte[10] & 0x08)
	  {
		m_prefix[0] = "m";
		scale = 1e-3;
	  }
	  else m_prefix[0] = "";

	  // unit
	  //
	  if      (byte[11] & 0x04) m_unit[0] = "Ohm";
	  else if (byte[11] & 0x08) m_unit[0] = "F";
	  else if (byte[12] & 0x02) m_unit[0] = "Hz";
	  else if (byte[12] & 0x04)
	  {
		m_unit[0] = "V";
		if (m_prefix[0] == "")
		{
		  if (byte[0] & 0x08) // AC
		  {
			m_rangeMax[0] = std::min( m_rangeMax[0], 750.0 );
		  }
		  else  // DC
		  {
			m_rangeMax[0] = std::min( m_rangeMax[0], 1000.0 );
		  }
		}
	  }
	  else if (byte[12] & 0x08)
	  {
		m_unit[0] = "A";
		if (m_prefix[0] == "")
		{
		  m_rangeMax[0] = std::min( m_rangeMax[0], 20.0 );
		}
	  }
	  else if (byte[13] & 0x01) m_unit[0] = "C";
	  else if (byte[10] & 0x04) m_unit[0] = "%";
	  else                      m_unit[0] = "";

	  //printf( "byte9=%02x byte10=%02x byte11=%02x byte12=%02x\n", byte[9], byte[10], byte[11], byte[12] );

	  //std::cout<<(int)byte[13]<<std::endl;

	  // mode
	  //
	  m_rangeMin[0] = 0;

	  if (byte[9] & 0x01)
	  {
		m_rangeMax[0] = 4;
		m_mode[0] = "DI";
	  }
	  else if (byte[12] & 0x0c)
	  {
		if   (byte[0] & 0x08)
		{
		  m_mode[0] = "AC";
		}
		else
		{
		  m_rangeMin[0] = -m_rangeMax[0];
		  m_mode[0] = "DC";
		}
	  }
	  else if (byte[11] & 0x04) m_mode[0] = "OH";
	  else if (byte[11] & 0x08) m_mode[0] = "CA";
	  else if (byte[12] & 0x02)
	  {
		m_mode[0] = "FR";
		m_rangeMax[0] = freqMax;
	  }
	  else if (byte[13] & 0x01)
	  {
		m_mode[0] = "TE";
		m_rangeMin[0] = -50;
		m_rangeMax[0] = 250;
	  }
	  else
	  {
		m_rangeMax[0] = 100;
		m_mode[0] = "DU";
	  }

	  setValue( 0, digits );
	  setRange( 0 );
	  m_realValue[0] = atof( digits.c_str() ) * scale;

	  setHasValue();
	  m_mutex.unlock();
	}
	else
	{
	  m_hasValue = false;
	}
  }
}

bool VC870::findOverflow( const unsigned char *byte )
{
  return false;
}

int VC870::readData( unsigned char *buffer )
{
  int cnt=0;
  int byte;

  do
  {
	if (m_port.readByte( &byte ) != Port::Ok)
	{
	  return -1;
	}
	buffer[cnt++] = byte;

	if (m_consoleLogging) writeLog( byte );

	if ((byte & 0xf0) == 0xe0 && cnt >= 14) return cnt;
  }
  while (cnt < 14 && (byte & 0xf0) != 0xe0);

  return -1;
}
