//======================================================================
// File:        logger.h
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

#ifndef LOGGER_HH
#define LOGGER_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief Empty implementation for logging
 */ 
class Logger : public DMMClass
{
public:
  /** \brief Constructor
   */
  Logger();
  virtual ~Logger();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new Logger; }

protected:
  SerialPort  m_port;
  
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
};

#endif // LOGGER_HH
