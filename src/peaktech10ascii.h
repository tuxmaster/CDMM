//======================================================================
// File:        peaktech10ascii.h
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 14:46:41 CET 2006
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

#ifndef PEAKTECH10ASCII_HH
#define PEAKTECH10ASCII_HH

#include "dmmclass.h"
#include "serialport.h"

/** \brief Base implementation of the PeakTech 11 byte ASCII protocoll

	The multimeter continuously writes an ascii string
	<p>
	11 byte format description:<br>
	\#VVVVVVUUUUS
	<p>
	\#: Start character<br>
	V: Value (including sign)<br>
	U: Unit<br>
	S: Sync Byte (0x0d)
 */
class PeakTech10Ascii : public DMMClass
{
public:
  /** \brief Constructor
  */
  PeakTech10Ascii();
  virtual ~PeakTech10Ascii();

  virtual Port::Error close();

  static DMMClass *create() { return new PeakTech10Ascii; }

protected:
  SerialPort  m_port;

  virtual Port::Error open_impl( const std::string & config );
  virtual void run();

  int readData( char *data ) const;

};

#endif // PEAKTECH10ASCII_HH
