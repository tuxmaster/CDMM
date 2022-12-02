//======================================================================
// File:        vc820.h
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 11:38:20 CET 2006
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

#ifndef VC820_HH
#define VC820_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief VC820 14 byte binary protocoll
   
    The device continuously sends 14 byte sequences. Written in
    hex one could see the following:<p>
    15, 25, 3B, 40, 55, 67, 7D, 8F, 9E, A0, B0, C0, D1, E1
    <p>
    The upper nibble is the position in the data set. The lower 
    nibble reflects the state of the <b>segments</b> of the display.
    This is rather ridiculous, but probably very cheap in the
    (hardware) implementation.
    <p>\image html "vc820_display.jpg"
    
    <table>
    <tr><td>Byte</td><td>0x01</td><td>0x02</td><td>0x04</td><td>0x08</td></tr>
    <tr><td>0</td><td>RS232</td><td>AUTO</td><td>DC</td><td>AC</td></tr>
    <tr><td>1</td><td>A1</td><td>A6</td><td>A5</td><td>Minus</td></tr>
    <tr><td>2</td><td>A2</td><td>A7</td><td>A3</td><td>A4</td></tr>
    <tr><td>3</td><td>B1</td><td>B6</td><td>B5</td><td>P1</td></tr>
    <tr><td>4</td><td>B2</td><td>B7</td><td>B3</td><td>B4</td></tr>
    <tr><td>5</td><td>C1</td><td>C6</td><td>C5</td><td>P2</td></tr>
    <tr><td>6</td><td>C2</td><td>C7</td><td>C3</td><td>C4</td></tr>
    <tr><td>7</td><td>D1</td><td>D6</td><td>D5</td><td>P3</td></tr>
    <tr><td>8</td><td>D2</td><td>D7</td><td>D3</td><td>D4</td></tr>
    <tr><td>9</td><td>Diode</td><td>k</td><td>n</td><td>u</td></tr>
    <tr><td>10</td><td>Beep</td><td>M</td><td>%</td><td>m</td></tr>
    <tr><td>11</td><td>Hold</td><td>High</td><td>Ohm</td><td>F</td></tr>
    <tr><td>12</td><td>Bat</td><td>Hz</td><td>V</td><td>A</td></tr>
    <tr><td>13</td><td>C</td></tr>
    </table>
    
    Byte 13 isn't specified by the QM1538 the first nibble is
    <b>always</b> 1110b. So temperature isn't going to work.
    
    The digits are named from left to right with A-D. There is no
    segment for DC but the bit will be set.
 */ 
class VC820 : public DMMClass
{
public:
  /** \brief Constructor
   */
  VC820();
  virtual ~VC820();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new VC820; }
  
protected:
  SerialPort  m_port;
  
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  int readData( unsigned char *buffer );
  std::string digitFromSegments( unsigned char a, unsigned char b ) const;
  bool findOverflow( const unsigned char *byte );
  
};

#endif // VC820_HH
