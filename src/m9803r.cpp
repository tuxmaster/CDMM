//======================================================================
// File:        m9803r.cpp
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 12:18:18 CET 2006
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

#include "m9803r.h"

#include <iostream>

M9803R::M9803R() :
  DMMClass()
{
}

M9803R::~M9803R()
{
  m_port.close();
}

Port::Error M9803R::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error M9803R::close()
{
  return m_port.close();
}

void M9803R::run()
{
  char data[64];

  while (m_run)
  {
	int cnt = readData( &m_port, (unsigned char *)data,
						 0x0a, 11, 3*11 );

	if (-1 != cnt)
	{
	  char *byte = data+cnt-11;

	  m_mutex.lock();

	  if (byte[0] & 0x01) m_overflow[0] = true;
	  else                m_overflow[1] = false;

	  // assemble value display
	  std::string digits;
	  // sign
	  if (byte[0] & 0x08) digits = "-";
	  // digits
	  digits += (char)('0'+byte[4]);
	  digits += (char)('0'+byte[3]);
	  digits += (char)('0'+byte[2]);
	  digits += (char)('0'+byte[1]);

	  double scale = 1.0;

	  // find decimal point, mode and unit
	  switch (byte[5])
	  {
		case 0x00:
		  m_mode[0] = "DC";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "V";
			  m_prefix[0] = "m";
			  digits = insertDecimalPoint( digits, 3 );
			  scale = 1e-3;
			  break;
			case 0x01:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x02:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x03:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x04:
			  m_unit[0] = "V";
			  break;
		  }
		  break;
		case 0x01:
		  m_mode[0] = "AC";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "V";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x01:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x02:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x03:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x04:
			  m_unit[0] = "V";
			  m_prefix[0] = "";
			  break;
		  }
		  break;
		case 0x02:
		  m_mode[0] = "DC";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x01:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x02:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 3 );
			  break;
		  }
		  break;
		case 0x03:
		  m_mode[0] = "AC";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x01:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  scale = 1e-3;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x02:
			  m_unit[0] = "A";
			  m_prefix[0] = "m";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
		  }
		  break;
		case 0x04:
		  m_mode[0] = "OH";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x01:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x02:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x03:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x04:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  break;
			case 0x05:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "M";
			  scale = 1e6;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
		  }
		  break;
		case 0x05:
		  m_mode[0] = "OH";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "Ohm";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
		  }
		  break;
		case 0x06:
		  m_mode[0] = "DI";
		  m_unit[0] = "V";
		  m_prefix[0] = "";
		  digits = insertDecimalPoint( digits, 1 );
		  break;
		case 0x08:
		  m_mode[0] = "DC";
		  m_unit[0] = "A";
		  m_prefix[0] = "";
		  digits = insertDecimalPoint( digits, 2 );
		  break;
		case 0x09:
		  m_mode[0] = "AC";
		  m_unit[0] = "A";
		  m_prefix[0] = "";
		  digits = insertDecimalPoint( digits, 2 );
		  break;
		case 0x0A:
		  m_mode[0] = "FR";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "Hz";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x01:
			  // TODO: CHECK THIS
			  m_unit[0] = "Hz";
			  m_prefix[0] = "k";
			  scale = 1e3;
			  break;
			case 0x05:
			  m_unit[0] = "Hz";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x06:
			  m_unit[0] = "Hz";
			  m_prefix[0] = "";
			  digits = insertDecimalPoint( digits, 3 );
			  break;
		  }
		  break;
		case 0x0C:
		  m_mode[0] = "CA";
		  switch (byte[6])
		  {
			case 0x00:
			  m_unit[0] = "F";
			  m_prefix[0] = "n";
			  scale = 1e-9;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x01:
			  m_unit[0] = "F";
			  m_prefix[0] = "n";
			  scale = 1e-9;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
			case 0x02:
			  m_unit[0] = "F";
			  m_prefix[0] = "n";
			  scale = 1e-9;
			  digits = insertDecimalPoint( digits, 3 );
			  break;
			case 0x03:
			  m_unit[0] = "uF";
			  scale = 1e-6;
			  digits = insertDecimalPoint( digits, 1 );
			  break;
			case 0x04:
			  m_unit[0] = "F";
			  m_prefix[0] = "u";
			  scale = 1e-6;
			  digits = insertDecimalPoint( digits, 2 );
			  break;
		  }
		  break;
	  }

	  setValue( 0, digits );
	  m_realValue[0] = atof( digits.c_str() ) * scale;

	  setHasValue();
	  m_mutex.unlock();
	}
	else m_hasValue = false;
  }
}
