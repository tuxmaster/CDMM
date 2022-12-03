//======================================================================
// File:        m9803r.h
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 12:18:18 CET 2006
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

#ifndef M9803R_HH
#define M9803R_HH

#include "dmmclass.h"
#include "serialport.h"

/** \brief M9803R 11 byte binary protocoll

	Reverse engeneered by Matthias Toussaint
	<ul><li>Port settings seem to be 9600 7E1 or 9600 7O1
	(looks like a bug)</li>
	<li>Protocoll is 11 bytes binary fixed length</li>
	<li>END OF PACKET is 0x0d 0x0a</li>
	<li>Multimeter continuously sends data</li>
	</ul>
	<b>Byte 0:</b> Sign ored together<br>
	0x00 -> positive<br>
	0x08 -> negative<br>
	0x01 -> overflow<br>

	<b>Byte 1:</b> Digit 4 binary<br>
	<b>Byte 2:</b> Digit 3 binary<br>
	<b>Byte 3:</b> Digit 2 binary<br>
	<b>Byte 4:</b> Digit 1 binary<br>

	<b>Byte 5:</b> Mode<br>
	0x00 DCV<br>
	0x01 ACV<br>
	0x02 DCA<br>
	0x03 ACA<br>
	0x04 Ohms<br>
	0x05 Ohms + Beep<br>
	0x06 Diode V<br>
	0x07 ADP<br>
	0x08 DCA (10A)<br>
	0x09 ACA (10A)<br>
	0x0A Freq<br>
	0x0C Capacity<br>

	<b>Byte 6:</b>Decimal point position<br>
	<table cellpadding="10" cellspacing="0" border="1">
	<tr bgcolor="#aaaaaa"><td><b>Display</b></td><td><b>Unit</b></td><td><b>Value</b></td></tr>
	<tr><td colspan="3" bgcolor="#bbbbbb"><center>Frequency</center></td></tr>
	<tr><td>0.000</td><td>kHz</td><td>0x00</td></tr>
	<tr bgcolor="#eeeeee"><td>00.00</td><td>kHz</td><td>0x01</td></tr>
	<tr><td>00.00</td><td>Hz</td><td>0x05</td></tr>
	<tr bgcolor="#eeeeee"><td>000.0</td><td>Hz</td><td>0x06</td></tr>
	<tr><td colspan="3" bgcolor="#bbbbbb"><center>Voltage</center></td></tr>
	<tr><td>000.0</td><td>mV</td><td>0x00</td></tr>
	<tr bgcolor="#eeeeee"><td>0.000</td><td>V</td><td>0x01</td></tr>
	<tr><td>00.00</td><td>V</td><td>0x02</td></tr>
	<tr bgcolor="#eeeeee"><td>000.0</td><td>V</td><td>0x03</td></tr>
	<tr><td>0000.</td><td>V</td><td>0x04</td></tr>
	<tr><td colspan="3" bgcolor="#bbbbbb"><center>Current</center></td></tr>
	<tr><td>0.000</td><td>mA</td><td>0x00</td></tr>
	<tr bgcolor="#eeeeee"><td>00.00</td><td>mA</td><td>0x01</td></tr>
	<tr><td>000.0</td><td>mA</td><td>0x02</td></tr>
	<tr><td colspan="3" bgcolor="#bbbbbb"><center>Capacity</center></td></tr>
	<tr><td>0.000</td><td>nF</td><td>0x00</td></tr>
	<tr bgcolor="#eeeeee"><td>00.00</td><td>nF</td><td>0x01</td></tr>
	<tr><td>000.0</td><td>nF</td><td>0x02</td></tr>
	<tr bgcolor="#eeeeee"><td>0.000</td><td>uF</td><td>0x03</td></tr>
	<tr><td>00.00</td><td>uF</td><td>0x04</td></tr>
	<tr><td colspan="3" bgcolor="#bbbbbb"><center>Resistance</center></td></tr>
	<tr><td>000.0</td><td>Ohm</td><td>0x00</td></tr>
	<tr bgcolor="#eeeeee"><td>0.000</td><td>kOhm</td><td>0x01</td></tr>
	<tr><td>00.00</td><td>kOhm</td><td>0x02</td></tr>
	<tr bgcolor="#eeeeee"><td>000.0</td><td>kOhm</td><td>0x03</td></tr>
	<tr><td>0000.</td><td>kOhm</td><td>0x04</td></tr>
	<tr bgcolor="#eeeeee"><td>00.00</td><td>MOhm</td><td>0x05</td></tr>
	</table>

	<b>Byte 7:</b> Hold/min/Max/Rel ored together<br>
	0x01 Hold<br>
	0x02 Rel<br>
	0x04 Min<br>
	0x08 Max<br>

	<b>Byte 8:</b> Auto/Manu<br>
	0x01 APOF (AutoPowerOff)<br>
	0x02 Manu<br>
	0x04 Auto<br>
	0x08 MEM<br>
 */
class M9803R : public DMMClass
{
public:
  /** \brief Constructor
   */
  M9803R();
  virtual ~M9803R();

  virtual Port::Error close();

  static DMMClass *create() { return new M9803R; }

protected:
  SerialPort  m_port;

  virtual Port::Error open_impl( const std::string & config );
  virtual void run();

};

#endif // M9803R_HH
