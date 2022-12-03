//======================================================================
// File:        vichy12bytepolling.h
// Author:      John Sheahan
// Created:     Mon 23 Jan 2012
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
// Copyright 2012 John Sheahan
//======================================================================

#ifndef VICHY12BYTEPOLLING_HH
#define VICHY12BYTEPOLLING_HH

#include "vichyascii.h"

/** \brief Implementation of the Vichy protocol in the polling
	flavour.

	The multimeter responds with  it's 12 byte string (ASCII) after
	receiving the sequence  0x8d. We poll every 250ms

	The poll is really 3 lines
	0x89  returns 12 bytes
	<p>
	Format description:<br>
	MMMMsVVVVVVS
	<p>
	M1M2M3M4
	M: Mode<br>    \0x89 \0xf0 \0xc0 \0x81
	s == '@' or 0x40 for positive, 'P' or 0x50 for negative
	V: Value (no sign)<br>
	S: Sync byte (0x0a)

	note there are ony 5 digits displayed, the last digit repeats.
	  (might be a checksum?)
	M1 is the poll byte echoed

	m3 says something about range.
	auto => 0xc0
	fixed, 5.8v =>   0x80
	fixed, 05.  =>   0x88
	fixed, 005. =>   0x90
	fixed  0005.9 => 0x98

	mV M2 = 0xe8
	mA M2 = 0xb0
	ohm     0xe0
	acv     0xf8
	20A     0xa8
	temp    0xc0
	cap     0xc8
	diode   0xd8
	hz      0xd0

	polls with 0x8b and 0x8a may return the second display and the bar graph
 */
class Vichy12BytePolling : public VichyAscii
{
public:
  Vichy12BytePolling();
  virtual ~Vichy12BytePolling();

  static DMMClass *create() { return new Vichy12BytePolling; }

protected:

};

#endif

