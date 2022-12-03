//======================================================================
// File:        serialport.h
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 13:49:23 CET 2006
// Project:     QtDMM
// Description: Encapsulation of serial port
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

#ifndef SERIALPORT_HH
#define SERIALPORT_HH

#include "port.h"
#include <fcntl.h>

#ifndef __WIN32
#include <sys/termios.h>
#include <sys/ioctl.h>

/** \brief Encapsulation of a serial port

	\see open for explanation of the implemented capabilities and
	how to access them.
*/
class SerialPort : public Port
{
public:
  SerialPort();
  virtual ~SerialPort();

  /** \brief Open the serial port.
	  \param config Port description in the following format:
  <tt>devicename:baudrate:bits:parity:stopbits[:RTS][:DTR][:DSR][:CTS]</tt>.<p>
  <b>baudrate</b> can be 300, 600, 1200, 1800, 2400, 4800, 9600, 19200 or 38400<br>
  <b>bits</b> might be 5, 6, 7 or 8<br>
  <b>parity</b> might be n (none), e (even) or o (odd)<br>
  <b>stopbits</b> might be 1 or 2<p>
  The optional RTS, DTR, DSR, CTS parameter can be used to force these
  signals to be set. This can be usefull for multimeter that use
  optocouplers. These guys sometimes are powered with one of these
  signals. For most multimeter a good starting point would be: DTR:DSR:CTS<p>
					Example: <tt><b>/dev/ttyS0:600:8:n:1</b></tt> -
					/dev/ttyS0 at 600 baud, 8 bits, no parity and one stop bit.<br>
					Example: <tt><b>/dev/ttyS0:300:7:e:1</b></tt> -
					/dev/ttyS0 at 300 baud, 7 bits, even parity and one stop bit.<br>
					Example: <tt><b>/dev/ttyS0:1200:7:o:2:RTS:CTS</b></tt> -
					/dev/ttyS0 at 1200 baud, 7 bits, odd parity, two stop bits, RTS and CTS set
   */
  virtual Port::Error open( const std::string & config );
  virtual Port::Error close();

protected:
  struct termios m_oldSettings;
};
#else
/** \brief Encapsulation of a serial port

	\see open for explanation of the implemented capabilities and
	how to access them.
*/
class SerialPort : public Port
{
public:
  SerialPort();
  virtual ~SerialPort();

  /** \brief Open the serial port.
	  \param config Port description in the following format:
  <tt>devicename:baudrate:bits:parity:stopbits[:RTS][:DTR][:DSR][:CTS]</tt>.<p>
  <b>baudrate</b> can be 300, 600, 1200, 1800, 2400, 4800, 9600, 19200 or 38400<br>
  <b>bits</b> might be 5, 6, 7 or 8<br>
  <b>parity</b> might be n (none), e (even) or o (odd)<br>
  <b>stopbits</b> might be 1 or 2<p>
  The optional RTS, DTR, DSR, CTS parameter can be used to force these
  signals to be set. This can be usefull for multimeter that use
  optocouplers. These guys sometimes are powered with one of these
  signals. For most multimeter a good starting point would be: DTR:DSR:CTS<p>
					Example: <tt><b>/dev/ttyS0:600:8:n:1</b></tt> -
					/dev/ttyS0 at 600 baud, 8 bits, no parity and one stop bit.<br>
					Example: <tt><b>/dev/ttyS0:300:7:e:1</b></tt> -
					/dev/ttyS0 at 300 baud, 7 bits, even parity and one stop bit.<br>
					Example: <tt><b>/dev/ttyS0:1200:7:o:2:RTS:CTS</b></tt> -
					/dev/ttyS0 at 1200 baud, 7 bits, odd parity, two stop bits, RTS and CTS set
   */
  virtual Port::Error open( const std::string & config );
  virtual Port::Error close();

protected:
  //struct termios m_oldSettings;
};
#endif

#endif // SERIALPORT_HH
