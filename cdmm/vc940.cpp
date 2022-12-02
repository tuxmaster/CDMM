//======================================================================
// File:        vc940.cpp
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

#include <vc940.h>
#include <stdio.h>

#include <iostream>

VC940::VC940() :
  DMMClass(),
  m_packetLen( 11 )
{
  // TODO: should come from settings
  //setNumCounts( 40000 );
  //setLimits( 1000, 750, 10, 10, false );
}

VC940::~VC940()
{
  m_port.close();
}

Port::Error VC940::open_impl( const std::string & config )
{
	//fprintf( stderr, "port open\n" );
  return m_port.open( config );
}

Port::Error VC940::close()
{
  return m_port.close();
}

bool VC940::findOverflow( unsigned char *byte ) const
{
  return (byte[0] == ':' && byte[1] == ':' && byte[2] == '0' && byte[3] == '<');
}

void VC940::run()
{
  unsigned char buffer[64];
  //fprintf( stderr, "run\n" );
	
  while (m_run)
  {
    int cnt = readData( buffer );
  
    if (-1 != cnt)
    {
      unsigned char *byte = buffer+cnt-m_packetLen;
      
      m_mutex.lock();
      
      // assemble digits
      //
      std::string digits;
      
      int function = byte[6] & 0x0f;
      int range    = byte[5] & 0x0f;

      // is that a bug in the meter?
      if (function != 12 && byte[8] & 0x04) digits = "-";
      for (int i=0; i<4; ++i) digits += byte[i];
      // check if 4000 counts mode
      if ((byte[4]) != 'A') digits += byte[4];
      
      m_overflow[0] = findOverflow( byte );
      
      double scale = 1.0;
  
			//fprintf( stderr, "function=%d range=%d byte7=%d byte8=%d digits=%s\n", 
      //         function, range, byte[7]&0x0f, byte[8]&0x0f, digits.c_str() );
			
      m_prefix[0] = "";
      
      switch (function)
      {
        case 0:
          digits = insertDecimalPoint( digits, 3 );
          m_mode[0] = "AC";
          m_prefix[0] = "m";
          m_unit[0] = "V";
          scale = 1e-3;
          break;
        case 1:
          digits = insertDecimalPoint( digits, range );
          m_mode[0] = "DC";
          m_unit[0] = "V";
          break;
        case 2:
          digits = insertDecimalPoint( digits, range );
          m_mode[0] = "AC";
          m_unit[0] = "V";
          break;
        case 3:
          digits = insertDecimalPoint( digits, 3 );
          m_mode[0] = "DC";
          m_prefix[0] = "m";
          m_unit[0] = "V";
          scale = 1e-3;
          break;
        case 4:
          m_unit[0] = "Ohm";
          switch (range)
          {
            case 1:
              digits = insertDecimalPoint( digits, 3 );
              break;
            case 2:
              digits = insertDecimalPoint( digits, 1 );
              m_prefix[0] = "k";
              break;
            case 3:
              digits = insertDecimalPoint( digits, 2);
              m_prefix[0] = "k";
              break;
            case 4:
              digits = insertDecimalPoint( digits, 3);
              m_prefix[0] = "k";
              scale = 1e3;
              break;
            case 5:
              digits = insertDecimalPoint( digits, 1 );
              m_prefix[0] = "M";
              scale = 1e6;
              break;
            case 6:
              digits = insertDecimalPoint( digits, 2 );
              m_prefix[0] = "M";
              scale = 1e6;
              break;
          }
          m_mode[0] = "OH";
          break;
        case 5:
          m_unit[0] = "F";
          switch (range)
          {
            case 1:
              digits = insertDecimalPoint( digits, 2 );
              m_prefix[0] = "n";
              scale = 1e-9;
              break;
            case 2:
              digits = insertDecimalPoint( digits, 3 );
              m_prefix[0] = "n";
              scale = 1e-9;
              break;
            case 3:
              digits = insertDecimalPoint( digits, 1 );
              m_prefix[0] = "u";
              scale = 1e-6;
              break;
            case 4:
              digits = insertDecimalPoint( digits, 2 );
              m_prefix[0] = "u";
              scale = 1e-6;
              break;
            case 5:
              digits = insertDecimalPoint( digits, 3 );
              m_prefix[0] = "u";
              scale = 1e-6;
              break;
            case 6:
              digits = insertDecimalPoint( digits, 4 );
              m_prefix[0] = "u";
              scale = 1e-6;
              break;
            case 7:
              digits = insertDecimalPoint( digits, 2 );
              m_prefix[0] = "m";
              scale = 1e-3;
              break;
          }
          m_mode[0] = "CA";
          break;
        case 6:
          m_mode[0] = "TE";
          m_unit[0] = "C";
          digits = insertDecimalPoint( digits, 4 );
          break;
        case 7:
          if (byte[7] & 0x01) 
          {
            if (byte[7] & 0x02) m_mode[0] = "AC+DC";
            else                m_mode[0] = "AC";
          }
          else m_mode[0] = "DC";
          switch (range)
          {
            case 0:
              digits = insertDecimalPoint( digits, 3 );
              break;
            case 1:
              digits = insertDecimalPoint( digits, 4 );
              break;
          }
          m_unit[0] = "A";
          m_prefix[0] = "u";
          scale = 1e-6;
          break;
        case 8:
          if (byte[7] & 0x01) 
          {
            if (byte[7] & 0x02) m_mode[0] = "AC+DC";
            else                m_mode[0] = "AC";
          }
          else m_mode[0] = "DC";
          switch (range)
          {
            case 0:
              digits = insertDecimalPoint( digits, 2 );
              break;
            case 1:
              digits = insertDecimalPoint( digits, 3 );
              break;
          }
          m_unit[0] = "A";
          m_prefix[0] = "m";
          scale = 1e-3;
          break;
        case 9:
          if (byte[7] & 0x01) 
          {
            if (byte[7] & 0x02) m_mode[0] = "AC+DC";
            else                m_mode[0] = "AC";
          }
          else m_mode[0] = "DC";
          digits = insertDecimalPoint( digits, 2 );
          m_unit[0] = "A";
          break;
        case 10:
            m_mode[0] = "OH";
            m_unit[0] = "Ohm";
            digits = insertDecimalPoint( digits, 3 );
            break;
        case 11:
          m_mode[0] = "DI";
          m_unit[0] = "V";
          digits = insertDecimalPoint( digits, 1 );
          break;
        case 12:
          if (byte[8] & 0x04)
          {
            m_mode[0] = "DU";
            m_unit[0] = "%";
            digits = insertDecimalPoint( digits, 3 );
          }
          else
          {
            m_mode[0] = "FR";
            m_unit[0] = "Hz";
            switch (range)
            {
              case 0:
                digits = insertDecimalPoint( digits, 2 );
                break;
              case 1:
                digits = insertDecimalPoint( digits, 3 );
                break;
              case 2:
                digits = insertDecimalPoint( digits, 1 );
                m_prefix[0] = "k";
                scale = 1e3;
                break;
              case 3:
                digits = insertDecimalPoint( digits, 2 );
                m_prefix[0] = "k";
                scale = 1e3;
                break;
              case 4:
                digits = insertDecimalPoint( digits, 3 );
                m_prefix[0] = "k";
                scale = 1e3;
                break;
              case 5:
                digits = insertDecimalPoint( digits, 1 );
                m_prefix[0] = "M";
                scale = 1e6;
                break;
              case 6:
                digits = insertDecimalPoint( digits, 2 );
                m_prefix[0] = "M";
                scale = 1e6;
                break;
              case 7:
                digits = insertDecimalPoint( digits, 3 );
                m_prefix[0] = "M";
                scale = 1e6;
                break;
            }
          }
          break;
        case 13:
          m_mode[0] = "TE";
          m_unit[0] = "F";
          digits = insertDecimalPoint( digits, 4 );
          break;

      }
  
      //fprintf( stderr, "%s %s%s (%d,%d)\n", digits.c_str(), m_prefix[0].c_str(), m_unit[0].c_str(), m_rangeMin[0], m_rangeMax[0] );
      
		  //fprintf( stderr, "setValue %s\n", digits.c_str() );
      setValue( 0, digits );
      m_realValue[0] = atof( digits.c_str() ) * scale;
      setRange(0);
      
      // VC920 only has 4 digits in Ohm (which sux)
      if (m_mode[0] == "OH") m_rangeMax[0] /= 10.0;
      
      setHasValue();
      m_mutex.unlock();
    }
    else
    {
      m_hasValue = false;
    }
  }
}

int VC940::readData( unsigned char *buffer ) const
{
  int cnt=0;
  int byte;
  
	//fprintf( stderr, "readData\n" );
	
  do
  {
		Port::Error err;
    if ((err=m_port.readByte( &byte )) != Port::Ok)
    {
			//fprintf( stderr, "readByte failed! (%s)\n", Port::errorString( err ) ); 
      return -1;
    }
    buffer[cnt++] = byte;
    /*if (m_consoleLogging)*/ //fprintf( stderr, "0x%02X ", (int)byte );
		
    if (byte == 0x0a && cnt >= m_packetLen && buffer[cnt-2] == 0x0d) 
		{
			//fprintf( stderr, "\npacket sync ok\n" );
			//if (m_consoleLogging) printf( "\n" );
			return cnt;
		}
  }
  while (cnt < m_packetLen && byte != 0x0a);
  
	//fprintf( stderr, "syncing failed\n " );
	//if (m_consoleLogging) fprintf( stderr, "\n" );
	
  return -1;
}
