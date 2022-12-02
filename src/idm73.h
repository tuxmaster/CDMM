//======================================================================
// File:        idm73.h
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

#ifndef IDM73_HH
#define IDM73_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief Iso-Tech IDM73 serial protocol

  Port setting is 19200 bps, 8 data bits, 1 stop bit, no parity, digits "6000".

  Send-only, no communication to multimeter. Protocol is 11 bytes long, ASCII.

  
  11 byte format description:<br>
  R D3 D2 D1 D0 F S O1 O2 CR LF<p>

  R:     Range<br>
  D0-D3: Digits 0-3<br>
  F:     Format<br>
  S:     Status<br>
  O1:    Option1<br>
  O2:    Option2<br>
  CR:    Carriage return (0x0d)<br>
  LF:    Line feed (0x04)<br>
  
  <table>
  <tr><td><b>Byte 0</b></td><td><b><i>Range</i></b></td><td></td></tr>
  <tr><td>Bin</td><td>Hex</td><td>Hex</td></tr>
  </table>  

  <table>
  <tr><td><b>Byte 1-4</b></td><td><b><i>Digit 3-0 (ASCII)</i></b></td><td></td></tr>
  </table>  
    
  <table>
  <tr><td><b>Byte 5</b></td><td><b><i>Function</i></b></td><td></td></tr>
  <tr><td>0111011</td><td>0x3b</td><td>Voltage</td></tr>
  <tr><td>0111101</td><td>0x3d</td><td>&my;A</td></tr>
  <tr><td>0111111</td><td>0x3f</td><td>A</td></tr>
  <tr><td>0110011</td><td>0x33</td><td>Ohm</td></tr>
  <tr><td>0110101</td><td>0x35</td><td>Continuity</td></tr>
  <tr><td>0110001</td><td>0x31</td><td>Diode</td></tr>
  <tr><td>0110010</td><td>0x32</td><td>Frequency</td></tr>
  <tr><td>0110110</td><td>0x36</td><td>Capacitance</td></tr>
  </table>  

  <table>
  <tr><td><b>Byte 6</b></td><td><b><i>Status</i></b></td><td></td><td></td><td></td><td></td><td></td></tr>
  <tr><td>Bit 6</td><td>Bit 5</td><td>Bit 4</td><td>Bit 3</td><td>Bit 2</td><td>Bit 1</td><td>Bit 0</td></tr>
  <tr><td>0</td><td>1</td><td>1</td><td>Judge</td><td>Sign</td><td>Lo Batt</td><td>Overflow</td></tr>
  </table>
  
   <table>
  <tr><td><b>Byte 7</b></td><td><b><i>Option 1</i></b></td><td></td><td></td><td></td><td></td><td></td></tr>
  <tr><td>Bit 6</td><td>Bit 5</td><td>Bit 4</td><td>Bit 3</td><td>Bit 2</td><td>Bit 1</td><td>Bit 0</td></tr>
  <tr><td>0</td><td>1</td><td>1</td><td>Pmax</td><td>Pmin</td><td>0</td><td>VAHZ</td></tr>
  </table>
   
   <table>
  <tr><td><b>Byte 8</b></td><td><b><i>Option 2</i></b></td><td></td><td></td><td></td><td></td><td></td></tr>
  <tr><td>Bit 6</td><td>Bit 5</td><td>Bit 4</td><td>Bit 3</td><td>Bit 2</td><td>Bit 1</td><td>Bit 0</td></tr>
  <tr><td>0</td><td>1</td><td>1</td><td>DC</td><td>AC</td><td>AUTO</td><td>APO</td></tr>
  </table>
  
  <table>
  <tr><td><b>Byte 9</b></td><td><b><i>Carriage return</i></b></td></tr>
  <tr><td>0001101</td><td><b><i>0x0d</i></b></td></tr>
  </table>  
  
  <table>
  <tr><td><b>Byte 10</b></td><td><b><i>Line feed</i></b></td></tr>
  <tr><td>0001010</td><td><b><i>0x0a</i></b></td></tr>
  </table>  

 */
class IDM73 : public DMMClass
{
  public:
  /** \brief Constructor
   */
    IDM73();
    virtual ~IDM73();
  
    virtual Port::Error close();
  
    static DMMClass *create() { return new IDM73; }
  
  protected:
    SerialPort  m_port;
  
    virtual Port::Error open_impl( const std::string & config );
    virtual void run();
  
    int readData( unsigned char *buffer ) const;
  
};

#endif // IDM73_HH
