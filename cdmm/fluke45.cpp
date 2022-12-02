//======================================================================
// File:        fluke45.cpp
// Author:      Matthias Toussaint
// Created:     Tue Dec 20 00:09:48 CET 2011
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

#include <fluke45.h>
#include <string.h>

#include <iostream>
#include <stdio.h>

Fluke45::Fluke45() :
  DMMClass()
{
  m_numCounts = 100000;
}

Fluke45::~Fluke45()
{
  m_port.close();
}

Port::Error Fluke45::open_impl( const std::string & config )
{
  Port::Error err = m_port.open( config );
  int byte;
  
  // empty buffers
  while (m_port.readByte( &byte, 0 ) == Port::Ok);
  
  if (err == Port::Ok) err = m_port.writeString( "LOCS\n", 5 );
  
  return err;
}

Port::Error Fluke45::close()
{
  return m_port.close();
}

void Fluke45::run()
{
  std::string buffer;
  
  while (m_run)
  {
    Port::Error err = readData( buffer );
  
    m_mutex.lock();
    
    m_overflow[0] = m_overflow[1] = false;
    
    // RATE;FUNC1;RANGE1;VAL1 or
    // RATE;FUNC1;RANGE1;FUNC2;RANGE2;VAL1;VAL2
    if (err == Port::Ok)
    {
      TokenList token = Util::tokenize( buffer, ";," );
        
      //printf( "TOKEN %d ", token.size() );
      //for (int i=0; i<token.size(); ++i) printf( "(%s) ", token[i].c_str() );
      //printf( "\n" );
        
      bool has_two = true;
      bool ok = true;
      
      std::string val1;
      std::string val2;
      
      if (token.size() == 2)  // Only values. My Fluke does this to me sometimes
      {
        val1 = token[0];
        val2 = token[1];
      }
      else if (token.size() == 4)
      {
        has_two = false;
        m_rate = token[0];
        m_func1 = token[1];
        m_range1 = token[2];
        val1 = token[3];
        if (val1 == "+1E+9") m_overflow[0] = true;
      }
      else if (token.size() == 7)
      {
        m_rate = token[0];
        m_func1 = token[1];
        m_range1 = token[2];
        m_func2 = token[3];
        m_range2 = token[4];
        val1 = token[5];
        if (val1 == "+1E+9") m_overflow[0] = true;
        val2 = token[6];
        if (val2 == "+1E+9") m_overflow[1] = true;
      }
      else ok = false;
      
      if (ok)
      {
        Util::split_MEAS( val1, m_value[0], m_prefix[0] );
        
        checkFunc( m_func1, 0 );
        checkRange( m_range1, 0 );
        
        if (has_two)
        {
          Util::split_MEAS( val2, m_value[1], m_prefix[1] );
          
          checkFunc( m_func2, 1 );
          checkRange( m_range2, 1 );
        }
        
        
        setHasValue();
      }
      
#ifndef __WIN32
      usleep( 1 );
#else
      Sleep( 1 );
#endif
    }
    else
    {
      m_hasValue = false;
    }
    
    m_mutex.unlock();
  }
}

void Fluke45::checkFunc( const std::string & func, int index )
{
  if (func == "VDC")        { m_mode[index] = "DC";    m_unit[index] = "V"; }
  else if (func == "VAC")   { m_mode[index] = "AC";    m_unit[index] = "V"; }
  else if (func == "VACDC") { m_mode[index] = "AC+DC"; m_unit[index] = "V"; }
  else if (func == "ADC")   { m_mode[index] = "DC";    m_unit[index] = "A"; }
  else if (func == "AAC")   { m_mode[index] = "AC";    m_unit[index] = "A"; }
  else if (func == "AACDC") { m_mode[index] = "AC+DC"; m_unit[index] = "A"; }
  else if (func == "FREQ")  { m_mode[index] = "FR";    m_unit[index] = "Hz"; }
  else if (func == "OHMS")  { m_mode[index] = "OH";    m_unit[index] = "Ohm"; }
  else if (func == "DIODE") { m_mode[index] = "DI";    m_unit[index] = "V";  }
  else if (func == "CONT")  { m_mode[index] = "DI";    m_unit[index] = "V";  }
}
void Fluke45::checkRange( const std::string & range, int index )
{
  if (range == "1")
  {
    if (m_unit[index] == "V")
    {
      m_rangeMax[index] = (m_rate == "S") ? 100 : 300;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 100 : 300;
    }
    else if (m_unit[index] == "A") 
    {
      m_rangeMax[index] = (m_rate == "S") ? 10 : 30;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Hz") 
    {
      m_rangeMax[index] = 1000;
      m_rangeMin[index] = 0;
    }
  }
  else if (range == "2")
  {
    if (m_unit[index] == "V")
    {
      m_rangeMax[index] = (m_rate == "S") ? 1000 : 3;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 1000 : 3;
    }
    else if (m_unit[index] == "A") 
    {
      m_rangeMax[index] = 100;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Hz") 
    {
      m_rangeMax[index] = 10;
      m_rangeMin[index] = 0;
    }
  }
  else if (range == "3")
  {
    if (m_unit[index] == "V")
    {
      m_rangeMax[index] = (m_rate == "S") ? 10 : 30;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 10 : 30;
    }
    else if (m_unit[index] == "A") 
    {
      m_rangeMax[index] = 10;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Hz") 
    {
      m_rangeMax[index] = 100;
      m_rangeMin[index] = 0;
    }
  }
  else if (range == "4")
  {
    if (m_unit[index] == "V")
    {
      m_rangeMax[index] = (m_rate == "S") ? 100 : 300;
      if (m_mode[index] != "DC") m_rangeMin[index] = 0;
      else                       m_rangeMin[index] = -m_rangeMax[index];
    }
    else if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 100 : 300;
    }
    else if (m_unit[index] == "Hz") 
    {
      m_rangeMax[index] = 1000;
      m_rangeMin[index] = 0;
    }
  }
  else if (range == "5")
  {
    if (m_unit[index] == "V")
    {
      if (m_mode[index] != "DC") 
      {
        m_rangeMin[index] = 0;
        m_rangeMax[index] = 750;
      }
      else
      {
        m_rangeMax[index] = 1000;
        m_rangeMin[index] = -m_rangeMax[index];
      }
    }
    else if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 1000 : 3;
    }
    else if (m_unit[index] == "Hz") 
    {
      m_rangeMax[index] = 1;
      m_rangeMin[index] = 0;
    }
  }
  else if (range == "6")
  {
    if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 10 : 30;
    }
  }
  else if (range == "7")
  {
    if (m_unit[index] == "Ohm") 
    {
      m_rangeMin[index] = 0;
      m_rangeMax[index] = (m_rate == "S") ? 100 : 300;
    }
  }
}

bool Fluke45::findOverflow( const char *byte )
{
  return false;
}

Port::Error Fluke45::readData( std::string & buffer )
{
  Port::Error err;
  static const char *poll = "RATE?;FUNC1?;RANGE1?;FUNC2?;RANGE2?;MEAS?\n";
 
  err = m_port.writeString( poll, strlen(poll) );
  if (err != Port::Ok) return err;
  
  do
  {
    err = m_port.readLine( buffer, 127, 20 );
    if (err != Port::Ok) return err;
    buffer = Util::strip_whitespace( buffer );
  }
  while (buffer.size() == 0 || buffer[0] == '=' || buffer[0] == '?' || buffer[0] == '!');
  
  return Port::Ok;
}
