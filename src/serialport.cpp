//======================================================================
// File:        serialport.cpp
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

#include "serialport.h"
#include <errno.h>
#include <ctype.h>
#include <iostream>
#include "util.h"
#include <string.h>

#include <stdio.h>

SerialPort::SerialPort() :
  Port()
{
}

SerialPort::~SerialPort()
{
  close();
}

Port::Error SerialPort::open( const std::string & config )
{
  bool rts=false, dtr=false, dsr=false, cts=false;

  TokenList token = Util::tokenize( config, ":" );
  int tokenCnt = token.size();

  for (int i=5; i<tokenCnt; ++i)
  {
	for (unsigned j=0; j<token[i].size(); ++j)
	{
	  token[i][j] = toupper( token[i][j] );
	}
	if (token[i] == "RTS")      rts=true;
	else if (token[i] == "DTR") dtr=true;
	else if (token[i] == "DSR") dsr=true;
	else if (token[i] == "CTS") cts=true;
  }

  int baud;
  if (!Util::fromString( &baud, token[1] )) return Port::SyntaxError;
  int bits;
  if (!Util::fromString( &bits, token[2] )) return Port::SyntaxError;
  int stopBits;
  if (!Util::fromString( &stopBits, token[4] )) return Port::SyntaxError;

  int parity = -1;

  if (token[3] == "N" || token[3] == "n") parity = 0;
  if (token[3] == "E" || token[3] == "e") parity = 1;
  if (token[3] == "O" || token[3] == "o") parity = 2;

  if (parity == -1) return Port::UnknownParity;
  if (stopBits != 1 && stopBits != 2) return Port::UnknownStopBits;

#ifndef __WIN32
  tcflag_t cflag = CREAD | CLOCAL;

  if (stopBits == 2)
  {
	cflag |= CSTOPB;
  }

  switch (bits)
  {
	case 5:
	  cflag |= CS5;
	  break;
	case 6:
	  cflag |= CS6;
	  break;
	case 7:
	  cflag |= CS7;
	  break;
	case 8:
	  cflag |= CS8;
	  break;
	default:
	  return Port::UnknownNumberOfBits;
  }

  switch (parity)
  {
	case 0: // None
	  break;
	case 1: // Even
	  cflag |= PARENB;
	  break;
	case 2: // Odd
	  cflag |= PARENB;
	  cflag |= PARODD;
  }

  struct termios attr;
  int            mdlns;

  memset( &attr, 0, sizeof( struct termios ) );

  m_handle = ::open( token[0].c_str(), O_RDWR | O_NOCTTY | O_NDELAY );

  if (-1 == m_handle)
  {
	switch (errno)
	{
	  case EACCES:
		return Port::PermissionDenied;
	  case ENXIO:
	  case ENODEV:
	  case ENOENT:
		return Port::NoSuchPort;
	  default:
		return Port::OpenError;
	}
  }

#if 0 // blocking
  fcntl( m_handle, F_SETFL, 0 );
#else // non-blocking
  int flags;
  if (-1 == (flags = fcntl(m_handle, F_GETFL, 0))) flags = 0;
  fcntl(m_handle, F_SETFL, flags | O_NONBLOCK);
#endif

  tcgetattr( m_handle, &m_oldSettings );

  attr.c_oflag = 0;
  attr.c_lflag = 0;
  attr.c_cflag = cflag;

  // According to Thomas Hoffman flags should be like this
  //
  if (0 == parity)          // Ignore parity errors
  {
	attr.c_iflag = IGNBRK | IGNPAR ;
  }
  else
  {
	attr.c_iflag = IGNBRK | INPCK | ISTRIP;
  }

  attr.c_cc[VTIME]= 0;
  attr.c_cc[VMIN]= 1;

  if (300 == baud)
  {
	cfsetospeed( &attr, B300 );
	cfsetispeed( &attr, B300 );
  }
  else if (600 == baud)
  {
	cfsetospeed( &attr, B600 );
	cfsetispeed( &attr, B600 );
  }
  else if (1200 == baud)
  {
	cfsetospeed( &attr, B1200 );
	cfsetispeed( &attr, B1200 );
  }
  else if (1800 == baud)
  {
	cfsetospeed( &attr, B1800 );
	cfsetispeed( &attr, B1800 );
  }
  else if (2400 == baud)
  {
	cfsetospeed( &attr, B2400 );
	cfsetispeed( &attr, B2400 );
  }
  else if (4800 == baud)
  {
	cfsetospeed( &attr, B4800 );
	cfsetispeed( &attr, B4800 );
  }
  else if (9600 == baud)
  {
	cfsetospeed( &attr, B9600 );
	cfsetispeed( &attr, B9600 );
  }
  else if (19200 == baud)
  {
	cfsetospeed( &attr, B19200 );
	cfsetispeed( &attr, B19200 );
  }
  else
  {
	cfsetospeed( &attr, B38400 );
	cfsetispeed( &attr, B38400 );
  }

  // really needed?
  tcflush( m_handle, TCIFLUSH );

  if (-1 == tcsetattr( m_handle, TCSANOW, &attr ))
  {
	goto config_error;
  }

  mdlns = 0;
  if (-1 == ioctl( m_handle, TIOCMGET, &mdlns ))
  {
	goto config_error;
  }

  if (rts) mdlns |= TIOCM_RTS;
  else     mdlns &= ~TIOCM_RTS;

  if (dtr) mdlns |= TIOCM_DTR;
  else     mdlns &= ~TIOCM_DTR;

  if (dsr) mdlns |= TIOCM_DSR;
  else     mdlns &= ~TIOCM_DSR;

  if (cts) mdlns |= TIOCM_CTS;
  else     mdlns &= ~TIOCM_CTS;

  if (-1 == ioctl( m_handle, TIOCMSET, &mdlns ))
  {
	goto config_error;
  }

  tcsetattr( m_handle, TCSAFLUSH, &attr );

  return Port::Ok;

#else
  // This is just f***ing braindead!!!!!
  WCHAR wideString[256];
	// yet another oddity (need to prepend this crap to make COM10 and above work)
  char port_name[256];
  strcpy( port_name, "\\\\.\\" );
  strcat( port_name, token[0].c_str() );
  MultiByteToWideChar( CP_ACP, 0, port_name, strlen(port_name)+1, wideString, strlen(port_name)+1 );

  m_handle = CreateFile( (const WCHAR *)wideString,
						 GENERIC_READ | GENERIC_WRITE,
						 0,
						 NULL,
						 OPEN_EXISTING,
						 0,
						 NULL );

  if (m_handle == INVALID_HANDLE_VALUE) return Port::OpenError;

  DCB dcb;      /* device control block */

  if (!GetCommState( m_handle, &dcb )) goto config_error;

  dcb.BaudRate = baud;
  dcb.ByteSize = bits;

  if (0 == parity)      dcb.Parity = NOPARITY;
  else if (1 == parity) dcb.Parity = EVENPARITY;
  else                  dcb.Parity = ODDPARITY;

  if (stopBits == 1) dcb.StopBits = ONESTOPBIT;
  else               dcb.StopBits = TWOSTOPBITS;

  if (rts) dcb.fRtsControl = RTS_CONTROL_ENABLE;
  else     dcb.fRtsControl = RTS_CONTROL_DISABLE;
  if (dtr) dcb.fDtrControl = DTR_CONTROL_ENABLE;
  else     dcb.fDtrControl = DTR_CONTROL_ENABLE;

  dcb.fInX = FALSE;

  if (!SetCommState( m_handle, &dcb )) goto config_error;

  return Port::Ok;
#endif

config_error:
#ifndef __WIN32
  ::close(m_handle);
  m_handle = -1;
#else
  CloseHandle( m_handle );
  m_handle = NULL;
#endif

  return Port::ConfigError;
}

Port::Error SerialPort::close()
{
#ifndef __WIN32
  if (-1 != m_handle)
  {
	// restore
	::tcsetattr( m_handle, TCSANOW, &m_oldSettings );
	::close( m_handle );
	m_handle = -1;
  }
#else
  CloseHandle( m_handle );
#endif

  return Port::Ok;
}
