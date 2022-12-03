//======================================================================
// File:        dmmclass.cpp
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

#include <vector>
#include "dmmclass.h"
#include "metex14bytespolling.h"
#include "metex14bytescontinuous.h"
#include "metex15bytescontinuous.h"
#include "m9803r.h"
#include "peaktech10ascii.h"
#include "iocbuvmonitor.h"
#include "gdm703.h"
#include "escort97.h"
#include "qm1537.h"
#include "vc820.h"
#include "vc870.h"
#include "vc940.h"
#include "idm73.h"
#include "pt506.h"
#include "fluke45.h"
#include "logger.h"
#include "vichy12bytepolling.h"
#include <stdio.h>

#include <iostream>

// list of all known protocols
// just add new lines in any order
DMMClass::Protocol DMMClass::s_protocol[] =
{
  Protocol( &Metex14BytesPolling::create, "M14P", "Metex 14 bytes, ASCII, polling" ),
  Protocol( &Metex14BytesContinuous::create, "M14C", "Metex 14 bytes, ASCII" ),
  Protocol( &Metex15BytesContinuous::create, "M15C", "Metex 15 bytes, ASCII" ),
  Protocol( &VC820::create, "VC820", "VC820 14 bytes, binary" ),
  Protocol( &M9803R::create, "M9803R", "M9803R 12 bytes, binary" ),
  Protocol( &IOCBUVMonitor::create, "IOCB", "IOCB UV Monitor" ),
  Protocol( &PeakTech10Ascii::create, "PT10A", "Peaktech 11 bytes, ASCII" ),
  Protocol( &GDM703::create, "GDM703", "GDM703 26 bytes, binary" ),
  Protocol( &Escort97::create, "ESC97", "Escort 97 21 bytes, binary" ),
  Protocol( &VC940::create, "VC940", "VC940 11 bytes, ASCII/binary" ),
  Protocol( &QM1537::create, "QM1537", "QM1537 14 bytes, ASCII/binary" ),
  Protocol( &IDM73::create, "IDM73", "IDM73 11 bytes, ASCII" ),
  Protocol( &PT506::create, "PT506", "Protek 506 6-15 Bytes, ASCII" ),
  Protocol( &VC870::create, "VC870", "VC870 14 bytes binary" ),
  Protocol( &Fluke45::create, "Fluke45", "Fluke 45, Local, RS232, no echo" ),
  Protocol( &Logger::create, "Logger", "Generic for logging" ),
  Protocol( &Vichy12BytePolling::create, "V12P", "Vichy 12 bytes, ASCII, polling" ),
  // end of list
  Protocol( 0, "", "" )
};

// The precompiled device list
// just add new lines in alphabetical order
DMMClass::Device DMMClass::s_device[] =
{
  Device( "Digitek", "DT-9062", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Digitech", "QM-1350", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Digitech", "QM-1538", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Digitech", "QM-1537", "QM1537", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "ELV", "M9803R", "M9803R", "9600:7:e:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Escort", "95", "Escort97", "2400:6:n:1:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Escort", "97", "Escort97", "2400:6:n:1:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Fluke", "45", "Fluke45", "9600:8:n:1:RTS:DSR:DTR:CTS", 2, 100000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "IOCB", "UV Monitor", "IOCB", "9600:8:n:1:DSR:DTR:CTS", 1, 1000000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Iso-Tech", "IDM73", "IDM73", "19200:8:n:1:DSR:DTR", 1, 6000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "MASTECH", "M9803R", "M9803R", "9600:7:e:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "MASTECH", "MAS-345", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "MASTECH", "MAS-343", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "McVoice", "M-345pro", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "McVoice", "M-980T", "M9803R", "9600:7:e:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Metex", "M-3360D", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M-3830D", "M14P", "1200:7:n:2:DSR:DTR:CTS", 4, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M-3850D", "M14P", "1200:7:n:2:DSR:DTR:CTS", 4, 8000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M-3850M", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 8000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M-4650CR", "M14P", "1200:7:n:2:DSR:DTR:CTS", 4, 20000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M-4660M", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 20000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "M9803R", "M9803R", "9600:7:e:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "ME-11", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "ME-22", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 2000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "ME-32", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "ME-42", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "Universal System 9140", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Metex", "Universal System 9160", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Metrel", "MD9015", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "PeakTech", "3330", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "PeakTech", "4010", "M14P", "9600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "PeakTech", "4360", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "PeakTech", "4390", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "PeakTech", "451", "PT10A", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Protek", "504", "PT506", "1200:7:n:2:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Protek", "505", "PT506", "1200:7:n:2:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Protek", "506", "PT506", "1200:7:n:2:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Radioshack", "22-805 DMM", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Radioshack", "RS22-168A", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Sinometer", "MAS-343", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "TekPower", "TP 4000ZC", "VC820", "2400:8:n:1:RTS:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "UNI-T", "UT30A", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT30E", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT60G", "IDM73", "19200:7:o:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT70B", "IDM73", "2400:7:o:1:DSR:DTR", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT71A", "VC940", "2400:7:n:1:DSR:DTR", 1, 20000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT71B", "VC940", "2400:7:n:1:DSR:DTR", 1, 20000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT71C", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT71D", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "UNI-T", "UT71E", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Vichy", "VC99", "QM1537", "2400:8:n:1:DSR:DTR", 1, 6000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Vichy", "VC8145", "V12P", "9600:8:n:1:RTS:DTR", 1, 80000, 1000, 750, 10, 10, -50, 250 ),
  //
  Device( "Voltcraft", "GDM703", "GDM703", "9600:8:n:1:DSR:DTR:CTS", 2, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "GDM704", "GDM703", "9600:8:n:1:DSR:DTR:CTS", 2, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "GDM705", "GDM703", "9600:8:n:1:DSR:DTR:CTS", 2, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-3610D", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-3650D", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-3830", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-3850", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 8000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-3860", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 8000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-4660", "M14P", "1200:7:n:2:DSR:DTR:CTS", 4, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-4660A", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-4660M", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 20000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "M-465CR", "M14C", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "ME-11", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "ME-22T", "M14P", "2400:7:n:2:DSR:DTR:CTS", 1, 2000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "ME-32", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "ME-42", "M14P", "600:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "MS-9140", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "MS-9160", "M14P", "1200:7:n:2:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "MXD-4660A", "M14P", "9600:7:n:2:DSR:DTR:CTS", 4, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 630", "M14C", "4800:7:n:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 650", "M14C", "4800:7:n:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 670", "M14C", "4800:7:n:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 635", "M15C", "2400:7:n:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 655", "M15C", "2400:7:n:1:DSR:DTR:CTS", 1, 4000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 820", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "VC 840", "VC820", "2400:8:n:1:DSR:DTR", 1, 4000, 1000, 750, 20, 20, -50, 250 ),
  Device( "Voltcraft", "VC 870", "VC870", "9600:8:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 920", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 940", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  Device( "Voltcraft", "VC 960", "VC940", "2400:7:n:1:DSR:DTR", 1, 40000, 1000, 750, 10, 10, -50, 250 ),
  // end of list
  Device( "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0 )
};

DMMClass::DMMClass() :
  m_hasValue( false ),
  m_numValues( 1 ),
  m_lowBat( false ),
  m_consoleLogging( false ),
  m_logCount( 0 ),
  m_logSyncCount( 0 ),
  m_logPollSize( 0 ),
  m_maxDCV( 1000 ),
  m_maxACV( 750 ),
  m_maxDCA( 20 ),
  m_maxACA( 20 ),
  m_freqFullScale( true ),
  m_deviceTimeout( 10 )
{
  for (int i=0; i<8; ++i) m_overflow[i] = false;
}

DMMClass::~DMMClass()
{
}

Port::Error DMMClass::open( const std::string & config )
{
  // give it some time on startup
  m_lastValueTime = m_openTime = time(0);

  return open_impl( config );
}

bool DMMClass::timeout(int timeout_val) const
{
  if (!m_hasValue && (time(0)-m_lastValueTime)>timeout_val) return true;

  return false;
}

std::string DMMClass::mode( int i )
{
  MutexLocker lock( &m_mutex );
  return m_mode[i];
}

std::string DMMClass::unit( int i )
{
  MutexLocker lock( &m_mutex );
  return m_unit[i];
}

std::string DMMClass::prefix( int i )
{
  MutexLocker lock( &m_mutex );
  return m_prefix[i];
}

void DMMClass::setValue( int index, std::string value )
{
  m_value[index] = value;
}

std::string DMMClass::value( int index )
{
  MutexLocker lock( &m_mutex );
  return m_value[index];
}

double DMMClass::minValue( int index )
{
  MutexLocker lock( &m_mutex );
  return m_rangeMin[index];
}

double DMMClass::maxValue( int index )
{
  MutexLocker lock( &m_mutex );
  return m_rangeMax[index];
}

double DMMClass::realValue( int index )
{
  MutexLocker lock( &m_mutex );
  return m_realValue[index];
}

int DMMClass::readData( Port *port, unsigned char *buffer,
						int syncByte, int minLen, int maxLen, int timeo )
{
  int cnt=0;
  int byte;

  timeo *= 10;

  do
  {
	if (m_consoleLogging && m_logPollSize)
	{
	  port->writeString( (char *)m_logPollSequence, m_logPollSize );
	}

	if (port->readByte( &byte, timeo ) != Port::Ok) return -1;

	buffer[cnt++] = byte;

	if (m_consoleLogging) writeLog( byte );

	if (byte == syncByte && cnt >= minLen)
	{
	  return cnt;
	}
  }
  while (cnt < maxLen && byte != syncByte);

  return -1;
}

DMMClass *DMMClass::create_protocol( std::string protocolName )
{
  for (int i=0; i<DMMClass::num_protocols(); ++i)
  {
	if (s_protocol[i].name == protocolName)
	{
	  return s_protocol[i].createFunction();
	}
  }

  return 0;
}

int DMMClass::num_protocols()
{
  static int cnt = count_protocols();

  return cnt;
}

int DMMClass::count_protocols()
{
  int cnt=0;

  while (s_protocol[cnt].createFunction) cnt++;

  return cnt;
}

std::string DMMClass::print_protocols()
{
  std::string table;
  std::stringstream ss( table );

  int cnt=0;
  while (s_protocol[cnt].createFunction)
  {
	ss << s_protocol[cnt].name
	   << ": " << s_protocol[cnt].menuText << "\n";
	cnt++;
  }

  return ss.str();
}

std::string DMMClass::print_devices()
{
  std::string table;
  std::stringstream ss( table );

  int cnt=0;
  while (s_device[cnt].numValues)
  {
	ss << s_device[cnt].brand
	   << " " << s_device[cnt].name << "\n";
	cnt++;
  }

  return ss.str();
}

bool DMMClass::has_device( const std::string & deviceName )
{
  int cnt=0;

  while (s_device[cnt].numValues)
  {
	std::string name = s_device[cnt].brand;
	name += " ";
	name += s_device[cnt].name;

	if (name == deviceName) return true;
	cnt++;
  }

  return false;
}

DMMClass::Device
DMMClass::get_device( const std::string & deviceName )
{
  int cnt=0;

  while (s_device[cnt].numValues)
  {
	std::string name = s_device[cnt].brand;
	name += " ";
	name += s_device[cnt].name;

	if (name == deviceName) return s_device[cnt];
	cnt++;
  }

  return s_device[cnt];
}

std::string DMMClass::device_table()
{
  std::string table;
  std::stringstream ss( table );

  int cnt=0;

  while (s_device[cnt].numValues)
  {
	ss << s_device[cnt].brand;
	ss << ", ";
	ss << s_device[cnt].name;
	ss << ", ";
	TokenList tokenList;
	std::string argument = s_device[cnt].port;
	std::string token;

	for (unsigned i=0; i<argument.size(); ++i)
	{
	  if (argument[i] != ':')
	  {
		token += argument[i];
	  }
	  else
	  {
		tokenList.push_back( token );
		token = "";
	  }
	}
	if (token.size()) tokenList.push_back( token );

	ss << tokenList[0];
	ss << ", ";
	ss << tokenList[1] + tokenList[2] + tokenList[3];
	ss << ", ";

	if (tokenList.size() > 4)
	{
	  for (unsigned i=4; i<tokenList.size(); ++i)
	  {
		if (i>4) ss << " ";
		ss << tokenList[i];
	  }
	}
	else ss << ", ";

	ss << ", ";
	ss << s_device[cnt].numCounts;
	ss << ", ";
	ss << s_device[cnt].numValues;
	ss << ", ";
	ss << s_device[cnt].maxVdc;
	ss << ", ";
	ss << s_device[cnt].maxVac;
	ss << ", ";
	ss << s_device[cnt].maxAdc;
	ss << ", ";
	ss << s_device[cnt].maxAac;
	ss << ", ";
	ss << s_device[cnt].minTemp;
	ss << ", ";
	ss << s_device[cnt].maxTemp;
	ss << ", ";

	ss << s_device[cnt].protocol;
	ss << "\n";

	cnt++;
  }

  return ss.str();
}

std::string DMMClass::insertDecimalPoint( const std::string & string, int pos )
{
  if (string[0] == '-')
  {
	pos++;
  }

  return string.substr( 0, pos ) + "." +
		 string.substr( pos, string.size()-pos );
}

void DMMClass::setConsoleLoggingSyncSequence( const std::string & sync_sequence )
{
  m_logSyncSize = 0;
  m_logSyncCount = 0;
  TokenList token = Util::tokenize( sync_sequence, "," );
  for (unsigned i=0; i<token.size(); ++i)
  {
	m_logSyncSequence[m_logSyncSize++] = token2Byte(Util::strip_whitespace(token[i]));
  }
}

void DMMClass::setConsoleLoggingPollSequence( const std::string & poll_sequence )
{
  m_logPollSize = 0;
  TokenList token = Util::tokenize( poll_sequence, "," );
  for (unsigned i=0; i<token.size(); ++i)
  {
	m_logPollSequence[m_logPollSize++] = token2Byte(Util::strip_whitespace(token[i]));
  }
}

unsigned char DMMClass::token2Byte( const std::string & token ) const
{
  int retval = 0;

  if (token[0] == '\\')
  {
	if (token.size()==2)                          // escape
	{
	  if (token[1] == 'n')      retval = '\n';
	  else if (token[1] == 'r') retval = '\r';
	  else if (token[1] == 't') retval = '\t';
	  else if (token[1] == ',') retval = ',';
	}
	else if (token.size()==4)                     // octal
	{
	  std::stringstream sstr(token.substr(1,3));
	  sstr >> std::oct >> retval;
	}
  }
  else if (isascii(token[0]) && token.size()==1)   // character
  {
	retval = token[0];
  }
  else if (token.substr(0,2)=="0x")   // hexnumber
  {
	std::stringstream sstr(token);
	sstr >> std::hex >> retval;
  }

  return retval;
}

void DMMClass::writeLog( unsigned char byte )
{
  m_logBuffer[m_logCount++] = byte;

  if (byte > 0x20 && isascii(byte))// && byte != '\n' && byte != '\r' && byte != '\t')
  {
	printf( "%c ", byte );
  }
  else if (byte == '\n')
  {
	printf( "\\n" );
  }
  else if (byte == '\r')
  {
	printf( "\\r" );
  }
  else if (byte == '\t')
  {
	printf( "\\t" );
  }
  else
  {
	printf( "  " );
  }

  if (m_logSyncCount < m_logSyncSize)
  {
	if (m_logSyncSequence[m_logSyncCount] == byte) m_logSyncCount++;
  }

  if ((m_logSyncCount && m_logSyncCount == m_logSyncSize) || m_logCount == 16)
  {
	printf( "  " );
	for (int i=0; i<m_logCount; ++i) printf( "%02X ", m_logBuffer[i] );
	printf( " : %d bytes\n", m_logCount );
	m_logCount = 0;
	m_logSyncCount = 0;
  }
}

void DMMClass::setRange( int index )
{
  double counts = m_numCounts;
  double scale = 1.0;
  int pos = 0;

  int stop = 0;
  if (m_value[index][0] == '-') stop++;
  for (int i=m_value[index].size(), cnt=0; i>=stop; --i, ++cnt)
  {
	if (m_value[index][i] == '.')
	{
	  pos = cnt-1;
	  break;
	}
  }

  //printf( "counts=%g pos=%d value=(%s)\n", counts, pos, m_value[index].c_str() );

  for (int i=0; i<pos; ++i) scale *= 10.0;
  counts /= scale;

  if (m_mode[index] == "AC")
  {
	if (m_unit[index] == "V")
	{
	  if (m_prefix[index] == "" && counts > m_maxACV) counts = m_maxACV;
	}
	else
	{
	  if (m_prefix[index] == "" && counts > m_maxACA) counts = m_maxACA;
	}
	m_rangeMin[index] = 0;
  }
  else if (m_mode[index] == "DC")
  {
	if (m_unit[index] == "V")
	{
	  if (m_prefix[index] == "" && counts > m_maxDCV) counts = m_maxDCV;
	}
	else
	{
	  if (m_prefix[index] == "" && counts > m_maxDCA) counts = m_maxDCA;
	}
  }
  else if (m_mode[index] == "TE")
  {
	if (m_unit[index] == "C") counts = m_maxTemp;
	else                      counts = 2*m_maxTemp;
  }
  else if (m_mode[index] == "DU")
  {
	counts = 100;
  }

  // Some DMM have full scale frequency display
  //
  if (m_mode[index] == "FR" && m_freqFullScale)
  {
	int pot=1;

	while (counts>pot) pot *= 10;
	counts = pot;
  }

  m_rangeMax[index] = counts;

  if (m_mode[index] == "FR" ||
	  m_mode[index] == "AC" ||
	  m_mode[index] == "CA" ||
	  m_mode[index] == "DI" ||
	  m_mode[index] == "OH" ||
	  m_mode[index] == "" ||   // hFE?
	  m_mode[index] == "DU")
  {
	m_rangeMin[index] = 0;
  }
  else m_rangeMin[index] = -counts;

  if (m_mode[index] == "TE") m_rangeMin[index] = m_minTemp;
}
