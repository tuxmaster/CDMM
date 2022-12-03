//======================================================================
// File:        vc870.h
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

#ifndef VC870_HH
#define VC870_HH

#include "dmmclass.h"
#include "serialport.h"

/** \brief VC870 14 byte binary protocoll

 */
class VC870 : public DMMClass
{
public:
  /** \brief Constructor
   */
  VC870();
  virtual ~VC870();

  virtual Port::Error close();

  static DMMClass *create() { return new VC870; }

protected:
  SerialPort  m_port;

  virtual Port::Error open_impl( const std::string & config );
  virtual void run();

  int readData( unsigned char *buffer );
  bool findOverflow( const unsigned char *byte );

};

#endif // VC870_HH
