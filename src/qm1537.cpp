//======================================================================
// File:        qm1537.cpp
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

#include <qm1537.h>

#include <iostream>

QM1537::QM1537() :
  DMMClass()
{
}

QM1537::~QM1537()
{
  m_port.close();
}

Port::Error QM1537::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error QM1537::close()
{
  return m_port.close();
}

// TODO
void QM1537::run()
{
  unsigned char buffer[64];
  
  while (m_run)
  {
    int cnt = readData( buffer );
  
    if (-1 != cnt)
    {
      unsigned char *byte = buffer+cnt-14;
      
      m_mutex.lock();
      
      m_overflow[0] = false;
      
      // assemble digits
      //
      std::string digits;
      digits.resize(5);
      
      digits[0] = byte[1];
      digits[1] = byte[2];
      digits[2] = byte[3];
      digits[3] = byte[4];
      digits[4] = byte[5];
            
      if (digits.substr( 1,4 ) == ";0:;") m_overflow[0] = true;
      
      // has leading sign
      switch (byte[7])
      {
        case 31:
          digits = insertDecimalPoint( digits, 2 );
          break;
        case 32:
          digits = insertDecimalPoint( digits, 3 );
          break;
        case 34:
          digits = insertDecimalPoint( digits, 4 );
          break;
      }

      double scale = 1.0;
      if (byte[10] & 0x08) 
      {
        scale = 1e6;
        m_prefix[0] = "M";
      }
      else if (byte[10] & 0x10) 
      {
        scale = 1e3;
        m_prefix[0] = "k";
      }
      else if (byte[10] & 0x20) 
      {
        scale = 1e-3;
        m_prefix[0] = "m";
      }
      else if (byte[10] & 0x40) 
      {
        scale = 1e-6;
        m_prefix[0] = "u";
      }

      double value = atof(digits.c_str()) * scale;

      setValue( 0, digits );
      
      if (byte[11] & 0x04)
      {
        m_unit[0] = "F";
      }
      else if (byte[11] & 0x08)
      {
        m_unit[0] = "Hz";
      }
      else if (byte[11] & 0x10)
      {
        m_unit[0] = "Ohm";
      }
      else if (byte[11] & 0x20)
      {
        m_unit[0] = "A";
      }
      else if (byte[11] & 0x40)
      {
        m_unit[0] = "V";
      }
      
      m_mode[0] = "";
      
      if (byte[8] & 0x04)
      {
        m_mode[0] = "AC";
      }
      else if (byte[8] & 0x08)
      {
        m_mode[0] = "DC";
      }
      else if (byte[10] & 0x02)
      {
        m_mode[0] = "DU";
      }
      else if (byte[10] & 0x04)
      {
        m_mode[0] = "DI";
      }
      else if (byte[11] & 0x04)
      {
        m_mode[0] = "CA";
      }
      else if (byte[11] & 0x08)
      {
        m_mode[0] = "FR";
      }
      else if (byte[11] & 0x10)
      {
        m_mode[0] = "OH";
      }
      
      setHasValue();
      m_mutex.unlock();
    }
    else
    {
      m_hasValue = false;
    }
  }
}

int QM1537::readData( unsigned char *buffer )
{
  int cnt=0;
  int byte;
  
  do
  {
    if (m_port.readByte( &byte ) != Port::Ok)
    {
      return -1;
    }
    if (byte == 0x0a) cnt=0;
    buffer[cnt++] = byte;
    
    if (m_consoleLogging) writeLog( byte );
    
    if (cnt >= 14 && buffer[12] == 0x00 && buffer[13] == 0x0d) return cnt;
  }
  while (cnt != 14 && byte != 0x0d);
  
  return -1;
}
