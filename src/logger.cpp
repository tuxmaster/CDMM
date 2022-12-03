//======================================================================
// File:        logger.cpp
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

#include "logger.h"

#include <iostream>

Logger::Logger() :
  DMMClass()
{
}

Logger::~Logger()
{
  m_port.close();
}

Port::Error Logger::open_impl( const std::string & config )
{
  return m_port.open( config );
}

Port::Error Logger::close()
{
  return m_port.close();
}

void Logger::run()
{
  unsigned char buffer[64];

  while (m_run)
  {
	int cnt = readData( &m_port, buffer, 0, 1, 63, 1 );

	m_mutex.lock();
	m_hasValue = true;
	m_mutex.unlock();
  }
}
