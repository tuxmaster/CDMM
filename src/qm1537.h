//======================================================================
// File:        qm1537.h
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
// Copyright 2007 Matthias Toussaint
//======================================================================

#ifndef QM1537_HH
#define QM1537_HH

#include "dmmclass.h"
#include "serialport.h"

/**
  Digitech QM-1537 serial protocol

  Port setting is 2400 bps, 8 data bits, 1 stop bit, no parity, digits "4000".

  Send-only, no communication to multimeter. Protocol is 14 bytes long, half-ASCII, half-binary.

  <i>The protocol was described by Rob Brown</i>

  <table>
  <tr><td>Byte 0</td><td>0x0A</td></tr>
  <tr><td>Byte 1</td><td>'+' or '-'</td></tr>
  <tr><td>Byte 2</td><td>digit 1</td></tr>
  <tr><td>Byte 3</td><td>digit 2</td></tr>
  <tr><td>Byte 4</td><td>digit 3</td></tr>
  <tr><td>Byte 5</td><td>digit 4</td></tr>
  <tr><td>Byte 6 </td><td>0x20</td></tr>
  <tr><td>Byte 7</td><td>"A" (see below)</td></tr>
  <tr><td>Byte 8</td><td>"B" (see below)</td></tr>
  <tr><td>Byte 9</td><td>"C" (see below)</td></tr>
  <tr><td>Byte 10</td><td>"D" (see below)</td></tr>
  <tr><td>Byte 11</td><td>"E" (see below)</td></tr>
  <tr><td>Byte 12</td><td>0x00</td></tr>
  <tr><td>Byte 13</td><td>0x0D</td></tr>
  </table>

  Remark: <i>;0:; as value (digits 1-4) indicates OL</i><p>

  <table>
  <tr><td> </td><td>A</td><td>B</td><td>C</td><td>D</td><td>E</td></tr>
  <tr><td colspan="6"><b>DC</b></td></tr>
  <tr><td>XXX.X mV</td><td>34</td><td>10</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>X.XXX V</td><td>31</td><td>10</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XX.XX V</td><td>32</td><td>10</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XXX.X V</td><td>34</td><td>10</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XXXX V</td><td>30</td><td>10</td><td>00</td><td>00</td><td>80</td></tr>

  <tr><td colspan="6"><b>AC</b></td></tr>
  <tr><td>XXX.X mV</td><td>34</td><td>08</td><td>00</td><td>40</td><td>80</td></tr>
  <tr><td>X.XXX V</td><td>31</td><td>08</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XX.XX V</td><td>32</td><td>08</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XXX.X V</td><td>34</td><td>08</td><td>00</td><td>00</td><td>80</td></tr>
  <tr><td>XXXX V</td><td>30</td><td>08</td><td>00</td><td>00</td><td>80</td></tr>

  <tr><td colspan="6"><b>Resistance</b></td></tr>

  <tr><td>XXX.X Ohms</td><td>34</td><td>00</td><td>00</td><td>00</td><td>20</td></tr>
  <tr><td>X.XXX kOhms</td><td>31</td><td>00</td><td>00</td><td>20</td><td>20</td></tr>
  <tr><td>XX.XX kOhms</td><td>32</td><td>00</td><td>00</td><td>20</td><td>20</td></tr>
  <tr><td>XXX.X kOhms</td><td>34</td><td>00</td><td>00</td><td>20</td><td>20</td></tr>
  <tr><td>X.XXX MOhms</td><td>31</td><td>00</td><td>00</td><td>10</td><td>20</td></tr>
  <tr><td>XX.XX MOhms</td><td>32</td><td>00</td><td>00</td><td>10</td><td>20</td></tr>

  <tr><td colspan="6"><b>Continuity</b></td></tr>

  <tr><td>XXX.X Ohms</td><td>34</td><td>00</td><td>00</td><td>08</td><td>20</td></tr>

  <tr><td colspan="6"><b>Diode</b></td></tr>

  <tr><td>X.XXX V</td><td>31</td><td>00</td><td>00</td><td>04</td><td>00</td></tr>

  <tr><td colspan="6"><b>Capacitance</b></td></tr>

  <tr><td>XX.XX nF</td><td>32</td><td>00</td><td>02</td><td>00</td><td>04</td></tr>

  <tr><td colspan="6"><b>Frequency</b></td></tr>

  <tr><td>X.XXX Hz</td><td>31</td><td>20</td><td>00</td><td>00</td><td>08</td></tr>

  <tr><td colspan="6"><b>Duty Cycle</b></td></tr>

  <tr><td>XXX.X %</td><td>34</td><td>00</td><td>00</td><td>02</td><td>00</td></tr>

  <tr><td colspan="6"><b>uA</b></td></tr>

  <tr><td>XXX.X uA-DC</td><td>34</td><td>10</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>XXXX  uA-DC</td><td>30</td><td>10</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>XXX.X uA-AC</td><td>34</td><td>08</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>XXXX  uA-AC</td><td>30</td><td>08</td><td>00</td><td>00</td><td>40</td></tr>

  <tr><td colspan="6"><b>mA</b></td></tr>

  <tr><td>XX.XX mA-DC</td><td>32</td><td>10</td><td>00</td><td>40</td><td>40</td></tr>
  <tr><td>XXX.X mA-DC</td><td>34</td><td>10</td><td>00</td><td>40</td><td>40</td></tr>
  <tr><td>XX.XX mA-AC</td><td>32</td><td>08</td><td>00</td><td>40</td><td>40</td></tr>
  <tr><td>XXX.X mA-DC</td><td>34</td><td>08</td><td>00</td><td>40</td><td>40</td></tr>

  <tr><td colspan="6"><b>A</b></td></tr>

  <tr><td>X.XXX A-DC</td><td>31</td><td>10</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>XX.XX A-DC</td><td>32</td><td>10</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>X.XXX A-AC</td><td>31</td><td>08</td><td>00</td><td>00</td><td>40</td></tr>
  <tr><td>XX.XX A-AC</td><td>32</td><td>08</td><td>00</td><td>00</td><td>40</td></tr>

  </table>

  <b>A is byte 7</b><br>
  30 when there is no decimal point.<br>
  31 when there is one digit before the decimal point<br>
  32 when there are two digits before the decimal point<br>
  34 when there are three digits before the decimal point<p>

  <b>B is byte 8</b><br>
  bit 1: Set when hold is active<br>
  bit 3: Set for AC<br>
  bit 4: Set for DC<br>
  bit 5: Set when auto-range is active<p>

  <b>C is byte 9</b><br>
  Bit 1 set for something to do with capacitance (nano prefix?)<br>
  Bit 3 set when auto-power-off is enabled<br>
  Bit 4 set when Min Hold is active<br>
  Bit 5 set when Max Hold is active<p>

  <b>D is byte 10</b><br>
  Bit 1: Set for Duty Cycle (percentage)<br>
  Bit 2: Set for diode test<br>
  Bit 3: Set for continuity test<br>
  Bit 4: Set for Mega prefix<br>
  Bit 5: Set for kilo prefix<br>
  Bit 6: set for milli prefix<br>
  Bit 7: set for micro prefix<p>

  <b>E is byte 11</b><br>
  Bit 2: Set for capacitance<br>
  Bit 3: Set for frequency<br>
  Bit 5: Set for resistance (including continuity)<br>
  Bit 6: Set for current<br>
  Bit 7: Set for voltage (including diode test)<br>
*/
class QM1537 : public DMMClass
{
  public:
  /** \brief Constructor
   */
	QM1537();
	virtual ~QM1537();

	virtual Port::Error close();

	static DMMClass *create() { return new QM1537; }

  protected:
	SerialPort  m_port;

	virtual Port::Error open_impl( const std::string & config );
	virtual void run();

	int readData( unsigned char *buffer );

};

#endif // QM1537_HH
