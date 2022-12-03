//======================================================================
// File:        vc940.h
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

#ifndef VC940_HH
#define VC940_HH

#include "dmmclass.h"
#include "serialport.h"

/// Byte 8 & 0x04 -> Duty cycle
/// FUnction 10 -> Continuity
class VC940 : public DMMClass
{
  public:
  /** \brief Constructor
   */
	VC940();
	virtual ~VC940();

	virtual Port::Error close();

	static DMMClass *create() { return new VC940; }

  protected:
	SerialPort  m_port;
		const int m_packetLen;

	virtual Port::Error open_impl( const std::string & config );
	virtual void run();

	int readData( unsigned char *buffer ) const;
	bool findOverflow( unsigned char *byte ) const;

};

#endif // VC940_HH
