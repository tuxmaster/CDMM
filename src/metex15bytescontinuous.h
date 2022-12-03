//======================================================================
// File:        metex15bytescontinuous.h
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

#ifndef METEX15BYTESCONTINUOUS_HH
#define METEX15BYTESCONTINUOUS_HH

#include "metexascii.h"
#include "serialport.h"

/** \brief Implementation of the Metex 15 byte protocoll in the continuous
	flavour.

	The multimeter continuously sends it's 15 byte string
	without the need for sending any command to the device (ASCII).
	<p>
	Format description:<br>
	MMMVVVVVVUUUUSS
	<p>
	M: Mode<br>
	V: Value (including sign)<br>
	U: Unit<br>
	S: Sync bytes (0x0a, 0x0d)
*/
class Metex15BytesContinuous : public MetexAscii
{
public:
  Metex15BytesContinuous();
  virtual ~Metex15BytesContinuous();

  static DMMClass *create() { return new Metex15BytesContinuous; }

};

#endif // METEX14BYTESCONTINUOUS_HH
