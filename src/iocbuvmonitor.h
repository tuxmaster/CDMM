//======================================================================
// File:        iocbuvmonitor.h
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

#ifndef IOCBUVMONITOR_HH
#define IOCBUVMONITOR_HH

#include "metexascii.h"

/** \brief Implementation of the Institute of Organic Chemistry and Biochemistry (Praha, Czech Republic) protocoll.

	The device responds with  it's 14 byte string (ASCII) after
	receiving the sequence D, 0x0d. This is the MAX132 based
	device used by the Institute of Organic Chemistry and Biochemistry
	in Czech Republic (Radek Liboska)
	<p>
	Format description:<br>
	MMVVVVVVVUUUUS
	<p>
	M: Mode<br>
	V: Value (including sign)<br>
	U: Unit<br>
	S: Sync byte (0x0d)<p>
	Poll once a second with "D\r"
 */
class IOCBUVMonitor : public MetexAscii
{
public:
  IOCBUVMonitor();
  virtual ~IOCBUVMonitor();

  static DMMClass *create() { return new IOCBUVMonitor; }

};

#endif // IOCBUVMONITOR_HH
