//======================================================================
// File:        escort97.h
// Author:      Matthias Toussaint
// Created:     Sun Nov 26 18:53:43 CET 2006
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

#ifndef ESCORT97_HH
#define ESCORT97_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief Implementation ESCORT 97/95 binary protocol

    This is a 21 byte binary protocol. The Escort 97 multimeter continuously
    sends the collowing 21 byte packet:<p>

    <table cellpadding="10" cellspacing="0" border="1">
    <tr bgcolor="#bbbbbb"><td colspan="2"><b>Data Items ( 21 Nibbles )</b></td></tr>
    <tr><td>Item</td><td>Meaning</td></tr>
    <tr bgcolor="#eeeeee"><td>1</td><td>Start Code (0x0f)</td></tr>
    <tr><td>2</td><td>Function</td></tr>
    <tr bgcolor="#eeeeee"><td>3</td><td>Primary Mode</td></tr>
    <tr><td>4</td><td>Secondary Mode</td></tr>
    <tr bgcolor="#eeeeee"><td>5</td><td>Primary Value Digital 1 (From right to left)</td></tr>
    <tr><td>6</td><td>Primary Value Digital 2</td></tr>
    <tr bgcolor="#eeeeee"><td>7</td><td>Primary Value Digital 3</td></tr>
    <tr><td>8</td><td>Primary Value Digital 4</td></tr>
    <tr bgcolor="#eeeeee"><td>9</td><td>Primary Value Digital 5 if = 11 the Primary Value = OL</td></tr>
    <tr><td>10</td><td>Bit3 = Primary Sign( 1 = - ) ; Bit2-Bit0 = Primary Range</td></tr>
    <tr bgcolor="#eeeeee"><td>11</td><td>Secondary Value Digital 1 (From right to left)</td></tr>
    <tr><td>12</td><td>Secondary Value Digital 2</td></tr>
    <tr bgcolor="#eeeeee"><td>13</td><td>Secondary Value Digital 3</td></tr>
    <tr><td>14</td><td>Secondary Value Digital 4</td></tr>
    <tr bgcolor="#eeeeee"><td>15</td><td>Secondary Value Digital 5 if = 11 the Secondary Value = OL</td></tr>
    <tr><td>16</td><td>Bit3 = Secondary Sign( 1 = - ) ; Bit2-Bit0 = Secondary Range</td></tr>
    <tr bgcolor="#eeeeee"><td>17</td><td>Bit3 : dBm Set        0 = Normal ; 1 = dBm test<br>
               Bit2 : A/mA Select    0 = mA     ; 1 = A<br>
               Bit1 : Input Warnning 0 = Right  ; 1 = Error<br>
               Bit0 : Battary State  0 = Right  ; 1 = Low</td></tr>
    <tr><td>18</td><td>Bit3 : 4000/40000 Set 0 = 4000   ; 1 = 40000<br>
               Bit2 : Peak Hold Set  0 = Normal ; 1 = Peak Hold test<br>
               Bit1 : 1/1 ,1/100 Set 0 = 1/1    ; 1 = 1/100<br>
               Bit0 : +/- Set        0 = +      ; 1 = -</td></tr>
    <tr bgcolor="#eeeeee"><td>19</td><td>Bit3 : Temp C ,Temp F Set  0 = Temp C ; 1 = Temp F<br>
               Bit2 : Square Wave Output Select  0 = Unused ; 1 = Select<br>
               Bit1 : Timer Counter Select  0 = Unused ; 1 = Select</td></tr>
               Bit0 : --
    <tr><td>20</td><td>Display Mode</td></tr>
    <tr bgcolor="#eeeeee"><td>21</td><td>Stop Code (0x00)</td></tr>
    </table>
    
    <p><b>Display values</b><br>
    100000 (0x20) Value Code = 0<br>
    100001 (0x21) Value Code = 1<br>
    100010 (0x22) Value Code = 2<br>
    100011 (0x23) Value Code = 3<br>
    100100 (0x24) Value Code = 4<br>
    100101 (0x25) Value Code = 5<br>
    100110 (0x26) Value Code = 6<br>
    100111 (0x27) Value Code = 7<br>
    101000 (0x28) Value Code = 8<br>
    101001 (0x29) Value Code = 9<br>
    101011 (0x2a) Value Code = OL<br>

    <p>
    <table cellpadding="10" cellspacing="0" border="1">
    <tr><td colspan="8" bgcolor="#bbbbbb"><b>Decimal point</b></td></tr>
    <tr><td>Value:</td><td>Item9=1</td><td>Item8=2</td><td>Item7=4</td><td>Item6=1</td><td>Item5=9</td></tr>
    <tr bgcolor="#eeeeee"><td>Range:</td><td>0</td><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td></tr>
    <tr><td>V</td><td>1.2419V</td><td>12.419V</td><td>124.19V</td><td>1242.9V</td><td colspan="3">&nbsp;</td></tr>
    <tr bgcolor="#eeeeee"><td>mV</td><td>12.419mV</td><td>124.19mV</td><td colspan="5">&nbsp;</td></tr>
    <tr><td>uA</td><td>124.19uA</td><td>1241.9uA</td><td colspan="5">&nbsp;</td></tr>
    <tr bgcolor="#eeeeee"><td>mA</td><td>12.419mA</td><td>124.19mA</td><td colspan="5">&nbsp;</td></tr>
    <tr><td>A</td><td>1.2419A</td><td>12.419A</tr><td colspan="5">&nbsp;</td></td>
    <tr bgcolor="#eeeeee"><td>ohmo</td><td>124.19Ohm</td><td>1.2419KOhm</td><td>12.419KOhm</td><td>124.19KOhm</td><td>1.2419MOhm</td><td>12.419M</td><td>12.419nS</td></tr>
    <tr><td>diode</td><td>1.2419V</td><td colspan="6">&nbsp;</td></tr>
    <tr bgcolor="#eeeeee"><td>temp</td><td>1241.9 Degree C/ Degree F</td><td colspan="6">&nbsp;</td></tr>
    <tr><td>Hz</td><td>12.419Hz</td><td>124.19Hz</td><td>1.2419KHz</td><td>12.419KHz</td><td>124.19KHz</td><td colspan="2">&nbsp;</td></td>
    <tr bgcolor="#eeeeee"><td>mS</td><td>124.19mS</td><td>1241.9mS</td><td colspan="5">&nbsp;</td></tr>
    <tr><td>Hz counter</td><td>12.419Hz</td><td>124.19Hz</td><td>1.2419KHz</td><td>12.419KHz</td><td>124.19KHz</td><td>1.2419MHz</td><td>12.419MHz</td></tr>
    </table>
    
    <p>
    <table cellpadding="10" cellspacing="0" border="1">
    <tr><td colspan="9" bgcolor="#bbbbbb"><b>Primary function</b></td></tr>
    <tr><td>Function</td><td>02</td><td>03</td><td>17b1</td><td>17b0</td><td>19b3</td><td>19b2</td><td>19b1</td><td> Notes</td></tr>
    <tr bgcolor="#eeeeee"><td>AC  V</td><td>--</td><td>00</td><td>--</td><td>0</td><td>--</td><td>--</td><td>--</td><td>Item2 = 0 - 1</td></tr>
    <tr><td>DC  V</td><td>01</td><td>01</td><td>--</td><td>0</td><td>--</td><td>--</td><td>--</td></tr>
<tr bgcolor="#eeeeee"><td>AC+DC  V</td><td>01</td><td>02</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>AC mV</td><td>02</td><td>00</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr bgcolor="#eeeeee"><td>DC mV</td><td>02</td><td>01</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>AC+DC mV</td><td>02</td><td>02</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr bgcolor="#eeeeee"><td>AC uA</td><td>03</td><td>00</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>DC uA</td><td>03</td><td>01</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td> </td></tr>
<tr bgcolor="#eeeeee"><td>AC+DC uA</td><td>03</td><td>02</td><td>--</td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>AC mA</td><td>04</td><td>00</td><td>0 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td> </td></tr>
<tr bgcolor="#eeeeee"><td>DC mA</td><td>04</td><td>01</td><td>0 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td> </td></tr>
<tr><td>AC+DC mA</td><td>04</td><td>02</td><td>0 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td> </td></tr>
<tr bgcolor="#eeeeee"><td>AC  A</td><td>04</td><td>00</td><td>  1 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>DC  A</td><td>04</td><td>01</td><td>  1 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr bgcolor="#eeeeee"><td>AC+DC  A</td><td>04</td><td>02</td><td>  1 </td><td>0 </td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>AC  dBm</td><td>--</td><td>00</td><td>--</td><td>  1 </td><td>--</td><td>--</td><td>--</td><td> Item2 = 0 - 4</td></tr>
<tr bgcolor="#eeeeee"><td>DC  dBm</td><td>--</td><td>01</td><td>--</td><td>  1 </td><td>--</td><td>--</td><td>--</td><td> Item2 = 0 - 4</td></tr>
<tr><td>AC+DC  dBm</td><td>--</td><td>02</td><td>--</td><td>  1 </td><td>--</td><td>--</td><td>--</td><td> Item2 = 0 - 4</td></tr>
<tr bgcolor="#eeeeee"><td>TEMP   C</td><td>05</td><td>03</td><td>--</td><td>--</td><td>0 </td><td>0 </td><td>0 </td><td>&nbsp;</td></tr>
<tr><td>TEMP   F</td><td>05</td><td>03</td><td>--</td><td>--</td><td>  1 </td><td>0 </td><td>0 </td><td>&nbsp;</td></tr>
<tr bgcolor="#eeeeee"><td>OHMO</td><td>06</td><td>04</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>DIODE</td><td>07</td><td>05</td><td>--</td><td>--</td><td>--</td><td>-- </td><td>-- </td><td>&nbsp;</td></tr>
<tr bgcolor="#eeeeee"><td>Cx</td><td>08</td><td>06</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
<tr><td>Hz</td><td>--</td><td>08</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>Item2 = 0 - 4</td></tr>
<tr bgcolor="#eeeeee"><td>Pulse Width</td><td>--</td><td>09</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>Item2 = 0 - 4</td></tr>
<tr><td>Duty Cycle</td><td>--</td><td>0A</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>Item2 = 0 - 4</td></tr>
<tr bgcolor="#eeeeee"><td>Hz Counter</td><td>07</td><td>08</td><td>--</td><td>--</td><td>--</td><td>--</td><td>--</td><td>&nbsp;</td></tr>
    </table>
 */ 
class Escort97 : public DMMClass
{
public:
  Escort97();
  virtual ~Escort97();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new Escort97; }
  
protected:
  SerialPort  m_port;
    
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  std::string byteToDigit( unsigned char byte );
  
};

#endif // ESCORT97_HH
