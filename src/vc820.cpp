//======================================================================
// File:        vc820.cpp
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 11:38:20 CET 2006
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

#include <vc820.h>

#include <iostream>
#include <stdio.h>

VC820::VC820() :
  DMMClass()
{
  m_numCounts = 4000;
}

VC820::~VC820()
{
  m_port.close();
}

Port::Error VC820::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error VC820::close()
{
  return m_port.close();
}

void VC820::run()
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
      
      if (byte[1] & 0x08) digits = "-";
      digits += digitFromSegments( byte[1], byte[2] );
      if (byte[3] & 0x08) 
      {
        m_rangeMax[0] = 4;
        freqMax = 10;
        digits += ".";
      }
      digits += digitFromSegments( byte[3], byte[4] );
      if (byte[5] & 0x08) 
      {
        m_rangeMax[0] = 40;
        freqMax = 100;
        digits += ".";
      }
      digits += digitFromSegments( byte[5], byte[6] );
      if (byte[7] & 0x08) 
      {
        m_rangeMax[0] = 400;
        freqMax = 1000;
        digits += ".";
      }
      digits += digitFromSegments( byte[7], byte[8] );
      
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

bool VC820::findOverflow( const unsigned char *byte )
{
  /*fprintf( stderr, "%02x %02x %02x %02x\n",
           (byte[2] & 0x0f) | ((byte[1] & 0x07) << 4),
           (byte[4] & 0x0f) | ((byte[3] & 0x07) << 4),
           (byte[6] & 0x0f) | ((byte[5] & 0x07) << 4),
           (byte[8] & 0x0f) | ((byte[7] & 0x07) << 4) );
  */
  for (int i=0; i<4; ++i)
  {
    if (((byte[2+2*i] & 0x0f) | ((byte[1+2*i] & 0x07) << 4)) == 0x68) return true;
  }
  
  return false;
}

std::string VC820::digitFromSegments( unsigned char a, 
                                      unsigned char b ) const
{
  std::string digit;
  
  unsigned test = (b & 0x0f) | ((a & 0x07) << 4);
  
  switch (test)
  {
    case 0x7d:
      digit = "0";
      break;
    case 0x05:
      digit = "1";
      break;
    case 0x5b:
      digit = "2";
      break;
    case 0x1f:
      digit = "3";
      break;
    case 0x27:
      digit = "4";
      break;
    case 0x3e:
      digit = "5";
      break;
    case 0x7e:
      digit = "6";
      break;
    case 0x15:
      digit = "7";
      break;
    case 0x7f:
      digit = "8";
      break;
    case 0x3f:
      digit = "9";
      break;
  }
  
  return digit;
}

int VC820::readData( unsigned char *buffer )
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
