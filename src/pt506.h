//======================================================================
// File:        pt506.h
// Author:      Matthias Toussaint
// Created:     Sat Dec 10 20:06:02 CET 2011
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

#ifndef PT506_HH
#define PT506_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief PT506 6-15 byte ASCII protocoll
   
    The device continuously sends NL terminated ASCII sequences. 
 */ 
class PT506 : public DMMClass
{
public:
  /** \brief Constructor
   */
  PT506();
  virtual ~PT506();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new PT506; }
  
protected:
  SerialPort  m_port;
  
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  int readData( unsigned char *buffer );
  bool findOverflow( const unsigned char *byte );
  
};

#endif // PT506_HH
