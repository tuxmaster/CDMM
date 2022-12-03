//======================================================================
// File:        idm73.cpp
// Author:      Matthias Toussaint
// Created:     Mon Aug 20 21:52:55 CEST 2007
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

#include "idm73.h"

#include <iostream>
#include <stdio.h>

IDM73::IDM73() :
  DMMClass()
{
}

IDM73::~IDM73()
{
  m_port.close();
}

Port::Error IDM73::open_impl( const std::string & config )
{
  Port::Error err = m_port.open( config );
  if (Port::Ok == err) m_port.writeByte( 'H' );

  return err;
}

Port::Error IDM73::close()
{
  return m_port.close();
}

void IDM73::run()
{
  unsigned char buffer[64];

  while (m_run)
  {
	int cnt = readData( buffer );
	//fprintf( stderr, "read %d bytes\n", cnt );

	if (-1 != cnt)
	{
	  unsigned char *byte = buffer+cnt-11;

	  m_mutex.lock();

	  // assemble digits
	  //
	  std::string digits;
	  digits  = byte[1];
	  digits += byte[2];
	  digits += byte[3];
	  digits += byte[4];
#if 1
	  fprintf( stderr, "range    = %s%s%s%s%s%s%s %02X\n",
			   ((byte[0] & 0x40) ? "1":"0"),
			   ((byte[0] & 0x20) ? "1":"0"),
			   ((byte[0] & 0x10) ? "1":"0"),
			   ((byte[0] & 0x08) ? "1":"0"),
			   ((byte[0] & 0x04) ? "1":"0"),
			   ((byte[0] & 0x02) ? "1":"0"),
			   ((byte[0] & 0x01) ? "1":"0"), byte[0] );
#endif

#if 1
	  fprintf( stderr, "function = %s%s%s%s%s%s%s %02X\n",
			   ((byte[5] & 0x40) ? "1":"0"),
			   ((byte[5] & 0x20) ? "1":"0"),
			   ((byte[5] & 0x10) ? "1":"0"),
			   ((byte[5] & 0x08) ? "1":"0"),
			   ((byte[5] & 0x04) ? "1":"0"),
			   ((byte[5] & 0x02) ? "1":"0"),
			   ((byte[5] & 0x01) ? "1":"0"), byte[5] );
#endif

#if 1
	  fprintf( stderr, "status   = %s%s%s%s%s%s%s %02X\n",
			   ((byte[6] & 0x40) ? "1":"0"),
			   ((byte[6] & 0x20) ? "1":"0"),
			   ((byte[6] & 0x10) ? "1":"0"),
			   ((byte[6] & 0x08) ? "1":"0"),
			   ((byte[6] & 0x04) ? "1":"0"),
			   ((byte[6] & 0x02) ? "1":"0"),
			   ((byte[6] & 0x01) ? "1":"0"), byte[6] );
#endif

#if 1
	  fprintf( stderr, "option1  = %s%s%s%s%s%s%s %02X\n",
			   ((byte[7] & 0x40) ? "1":"0"),
			   ((byte[7] & 0x20) ? "1":"0"),
			   ((byte[7] & 0x10) ? "1":"0"),
			   ((byte[7] & 0x08) ? "1":"0"),
			   ((byte[7] & 0x04) ? "1":"0"),
			   ((byte[7] & 0x02) ? "1":"0"),
			   ((byte[7] & 0x01) ? "1":"0"), byte[7] );
#endif

#if 1
	  fprintf( stderr, "option2  = %s%s%s%s%s%s%s %02X\n\n",
			   ((byte[8] & 0x40) ? "1":"0"),
			   ((byte[8] & 0x20) ? "1":"0"),
			   ((byte[8] & 0x10) ? "1":"0"),
			   ((byte[8] & 0x08) ? "1":"0"),
			   ((byte[8] & 0x04) ? "1":"0"),
			   ((byte[8] & 0x02) ? "1":"0"),
			   ((byte[8] & 0x01) ? "1":"0"), byte[8] );
#endif

	  if (byte[6] & 0x01) m_overflow[0] = true;
	  else                m_overflow[0] = false;

	  if (byte[6] & 0x04) digits = "-" + digits;

	  m_prefix[0] = "";
	  double scale = 1.0;

	  if (byte[5] == 0x3b)
	  {
		if (byte[8] & 0x04) m_mode[0] = "AC";
		else                m_mode[0] = "DC";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "V";
		  m_prefix[0] = "m";
		  scale = 1e-3;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "V";
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x32)
		{
		  m_unit[0] = "V";
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x33)
		{
		  m_unit[0] = "V";
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x34)
		{
		  m_unit[0] = "V";
		}
	  }
	  else if (byte[5] == 0x33)
	  {
		m_mode[0] = "OH";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "Ohm";
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "Ohm";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x32)
		{
		  m_unit[0] = "Ohm";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x33)
		{
		  m_unit[0] = "Ohm";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x34)
		{
		  m_unit[0] = "Ohm";
		  m_prefix[0] = "M";
		  scale = 1e6;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x35)
		{
		  m_unit[0] = "Ohm";
		  m_prefix[0] = "M";
		  scale = 1e6;
		  digits = insertDecimalPoint( digits, 2 );
		}
	  }
	  else if (byte[5] == 0x3d)
	  {
		if (byte[8] & 0x04) m_mode[0] = "AC";
		else                m_mode[0] = "DC";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "A";
		  m_prefix[0] = "u";
		  scale = 1e-6;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "A";
		  m_prefix[0] = "u";
		  scale = 1e-6;
		}
	  }
	  else if (byte[5] == 0x30)
	  {
		if (byte[8] & 0x04) m_mode[0] = "AC";
		else                m_mode[0] = "DC";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "A";
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "A";
		  digits = insertDecimalPoint( digits, 2 );
		}
	  }
	  else if (byte[5] == 0x32)
	  {
		m_mode[0] = "FR";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x32)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "k";
		  scale = 1e3;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x33)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "M";
		  scale = 1e6;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x34)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "M";
		  scale = 1e6;
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x35)
		{
		  m_unit[0] = "Hz";
		  m_prefix[0] = "M";
		  scale = 1e6;
		  digits = insertDecimalPoint( digits, 3 );
		}
	  }
	  else if (byte[5] == 0x36)
	  {
		m_mode[0] = "CA";

		if (byte[0] == 0x30)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "n";
		  scale = 1e-9;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x31)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "n";
		  scale = 1e-9;
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x32)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "n";
		  scale = 1e-9;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x33)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "u";
		  scale = 1e-6;
		  digits = insertDecimalPoint( digits, 1 );
		}
		else if (byte[0] == 0x34)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "u";
		  scale = 1e-6;
		  digits = insertDecimalPoint( digits, 2 );
		}
		else if (byte[0] == 0x35)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "u";
		  scale = 1e-6;
		  digits = insertDecimalPoint( digits, 3 );
		}
		else if (byte[0] == 0x36)
		{
		  m_unit[0] = "F";
		  m_prefix[0] = "m";
		  scale = 1e-3;
		  digits = insertDecimalPoint( digits, 1 );
		}
	  }
	  else if (byte[5] == 0x31)
	  {
		m_mode[0] = "DI";
	  }

	  // low battery
	  //
	  if (byte[6] & 0x02) m_lowBat = true;
	  else                m_lowBat = false;

	  setValue( 0, digits );
	  m_realValue[0] = atof( digits.c_str() ) * scale;

	  //fprintf( stderr, "value=%s unit=%s mode=%s\n",
	  //         digits.c_str(), m_unit[0].c_str(), m_mode[0].c_str() );
	  setHasValue();
	  m_mutex.unlock();
	}
	else
	{
	  //fprintf( stderr, "setting to false\n" );
	  //m_hasValue = false;
	}
  }
}

int IDM73::readData( unsigned char *buffer ) const
{
  int cnt=0;
  int byte;

  do
  {
	if (m_port.readByte( &byte ) != Port::Ok)
	{
	  return -1;
	}
	byte &= 0x7f;

	//fprintf( stderr, "(%d) %02X ", cnt, (unsigned)byte );
	buffer[cnt++] = byte;

	//if (byte == 0x0a && cnt >= 11 && buffer[cnt-2] == 0x0d) fprintf( stderr, "\n" );

	if (byte == 0x0a && cnt >= 11 && buffer[cnt-2] == 0x0d) return cnt;
  }
  while (cnt < 11 && byte != 0x0a);

  return -1;
}
