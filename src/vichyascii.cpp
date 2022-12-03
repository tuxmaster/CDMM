//======================================================================
// File:        vichyascii.cpp
// Author:      John Sheahan
// Created:     23 Jan 2012
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
// Copyright 2007 John Sheahan
//======================================================================

#include "vichyascii.h"

#include <iostream>
#include <stdio.h>

VichyAscii::VichyAscii( const std::string & format,
						const std::string & overflowStr,
						bool poll, const std::string & pollString,
						int pollTime ) :
  DMMClass(),
  m_overflowStr( overflowStr ),
  m_poll( poll ),
  m_pollString( pollString ),
  m_pollTime( pollTime*1000 )
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
	  else if (format[i] == 'X')
	  {
		start = &m_csumStart;
		len   = &m_csumLen;
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

VichyAscii::~VichyAscii()
{
  m_port.close();
}

Port::Error VichyAscii::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error VichyAscii::close()
{
  return m_port.close();
}

//void VichyAscii::setVichyAscii( const std::string & str, int index )
//{
//  m_mutex.lock();
//
//  setValue( index, Util::strip_whitespace( str.substr( m_valueStart, m_valueLen )) );
//  m_mode[index] = Util::strip_whitespace( str.substr( m_modeStart, m_modeLen ));
//  m_unit[index] = Util::strip_whitespace( str.substr( m_unitStart, m_unitLen ));
//
//  m_hasValue = true;
//  if (str.substr(m_valueStart,m_valueLen).find(m_overflowStr)<(unsigned)m_valueLen)
//  {
//    m_overflow[index]=true;
//  }
//  else m_overflow[index]=false;
//
//  m_mutex.unlock();
//}
//
//char hexmap (char in)
//{
//  if (in <= 0x09)
//	{
//	  return in + '0';
//	}
//  else
//	{  return in + 'A' - 10;
//	}
//}
void VichyAscii::run()
{
  char data[64];
  m_hasValue = false;

  while (m_run)
	{
	  if (m_poll)
		{
		  m_port.writeString( m_pollString.c_str(), m_pollString.size() );
		}

	  for (int i=0; i<m_numValues; ++i)
		{
		  int cnt = readData( &m_port, (unsigned char *)data,
							  m_syncByte, m_length, 3*m_length );

		  bool mapped = false;
		  if (-1 != cnt)
			{
			  int start = cnt-m_length;
			  data[cnt-1] = 0;
			  std::string strData = data+start;
			  char *byte = data+cnt-11;

			  //std::cerr << strData << std::endl;

			  //        setVichyAscii( strData, i );
			  m_mutex.lock();

			  // assemble value display
			  std::string digits;
			  digits = ' ';

			  if (byte[3] == 0x50)
				{ digits += '-';
				}
			  else
				{
				  digits += ' ';
				}
			  // digits
			  digits += (char)(byte[4]);
			  digits += (char)(byte[5]);
			  digits += (char)(byte[6]);
			  digits += (char)(byte[7]);
			  digits += (char)(byte[8]);

			  // double d_val;
			  // Util::fromString( &d_val, digits );
			  // printf("string made %f\n", d_val);
			  // std::cout << digits;

			  // setValue( 0, digits );
			  unsigned char ubyte_0 = byte[0];
			  unsigned char ubyte_1 = byte[1];
			  unsigned char ubyte_1a = byte[1] ;
			  std::string local_mode = "";
			  std::string local_unit = "";
			  // printf(" first two bytes are %d %x %u %d %x\n", ubyte_0, ubyte_0, ubyte_0, ubyte_1, ubyte_1);

			  // setValue( i, Util::strip_whitespace( strData.substr( m_valueStart, m_valueLen )) );
			  // m_mode[i] = Util::strip_whitespace( strData.substr( m_modeStart, m_modeLen ));
			  //m_unit[i] = Util::strip_whitespace( strData.substr( m_unitStart, m_unitLen ));
			  switch (ubyte_0)
				{
				  case 0xe8:
				  local_unit = "mV";
					// case -0x18:
					if ((ubyte_1 & 0x40 ) == 0x40)
					  {
						local_mode = "auto";
					  }
					ubyte_1a = ubyte_1 & 0xbf ; // ignore auto bit
					switch (ubyte_1a)
					{
					case 0x80:
					  digits = insertDecimalPoint( digits, 4);
					  mapped = true;
					  break;
					case 0x88:
					  digits = insertDecimalPoint( digits, 5);
					  mapped = true;
					  break;
					case 0x90:
					  digits = insertDecimalPoint( digits, 6);
					  mapped = true;
					  break;
					case 0x98:
					  digits = insertDecimalPoint( digits, 7);
					  mapped = true;
					  break;
					default :
					  digits = insertDecimalPoint( digits, 4);

					}
				  break;
				case 0xf0:
				  local_unit = "V";
				  local_mode = "";
				  if ((ubyte_1 & 0x40 ) == 0x40)
					{
					  local_mode = "auto";
					}
				  ubyte_1a = ubyte_1 & 0xbf ; // ignore auto bit
				  switch (ubyte_1a)
					{
					case 0x80:
					  digits = insertDecimalPoint( digits, 3);
					  mapped = true;
					  break;
					case 0x88:
					  digits = insertDecimalPoint( digits, 4);
					  mapped = true;
					  break;
					case 0x90:
					  digits = insertDecimalPoint( digits, 5);
					  mapped = true;
					  break;
					case 0x98:
					  digits = insertDecimalPoint( digits, 6);
					  mapped = true;
					  break;
					}
				  break;
				case 0xb0:
				  local_unit = "mA";
				  local_mode = "";
				  if ((ubyte_1 & 0x40 ) == 0x40)
					{
					  local_mode = "auto";
					}
				  ubyte_1a = ubyte_1 & 0xbf ; // ignore auto bit
				  switch (ubyte_1a)
					{
					case 0x80:
					  digits = insertDecimalPoint( digits, 4);
					  mapped = true;
					  break;
					case 0x88:
					  digits = insertDecimalPoint( digits, 5);
					  mapped = true;
					  break;
					case 0x90:
					  digits = insertDecimalPoint( digits, 6);
					  mapped = true;
					  break;
					case 0x98:
					  digits = insertDecimalPoint( digits, 7);
					  mapped = true;
					  break;
					}
				  break;
				case 0xf8:
				  local_unit = "Vac";
				  break;
				case 0xa8:
				  local_unit = "A";
				  local_mode = "";
				  if ((ubyte_1 & 0x40 ) == 0x40)
					{
					  local_mode = "auto";
					}
				  ubyte_1a = ubyte_1 & 0xbf ; // ignore auto bit
				  switch (ubyte_1a)
					{
					case 0x80:
					  digits = insertDecimalPoint( digits, 3);
					  mapped = true;
					  break;
					case 0x88:
					  digits = insertDecimalPoint( digits, 4);
					  mapped = true;
					  break;
					case 0x90:
					  digits = insertDecimalPoint( digits, 5);
					  mapped = true;
					  break;
					case 0x98:
					  digits = insertDecimalPoint( digits, 6);
					  mapped = true;
					  break;
					}
				  break;
				case 0xc0:
				  local_unit = "C";
				  break;
				case 0xc8:
				  local_unit = "F";
				  break;
				case 0xd8:
				  local_unit = "diode";
				  break;
				case 0xd0:
				  local_unit = "Hz";
				  break;

				  // default :
				}

			  if (mapped)
				{
				  m_mode[0] = local_mode;
				  m_unit[0] = local_unit;
				  setValue( 0, digits );
				  m_hasValue = true;
				  m_lastValueTime = time(0);
				  // printf("mapped\n");
				}
			  // else
			  //	{
			  //	  printf("Not mapped\n");
			  //	}
			  //	if (strData.substr(m_valueStart,m_valueLen).find(m_overflowStr)<(unsigned)m_valueLen)
			  //  {
			  //	m_overflow[i]=true;
			  // }
			  //else m_overflow[i]=false;

			  m_mutex.unlock();

			}
		  else m_hasValue = false;
		}

	  usleep(m_pollTime);
	}
}
