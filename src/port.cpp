//======================================================================
// File:        port.cpp
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 13:49:23 CET 2006
// Project:     QtDMM
// Description: Base class for multimter port
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

#include "port.h"
#ifndef __WIN32
#include <errno.h>
#include <unistd.h>
#endif

Port::Port() :
#ifdef __WIN32
  m_handle( NULL )
#else
  m_handle( -1 )
#endif
{
}

Port::~Port()
{
}

Port::Error Port::writeByte( int byte ) const
{
  ssize_t retval;
  unsigned char cByte = byte;

#ifndef __WIN32
  do
  {
	retval = ::write( m_handle, &cByte, 1 );
  }
  while (retval != -1 && retval != 1);

  if (retval < 0)
  {
	return errnoValue();
  }
#else
  DWORD num_byte;
  if (!WriteFile( m_handle, &cByte, 1, &num_byte, NULL ))
  {
	return errnoValue();
  }

  if (1!=num_byte) return Port::WriteError;
#endif

  return Port::Ok;
}

Port::Error Port::readLine( std::string & line, ssize_t len, int timeo ) const
{
  Port::Error err;
  int byte;
  int cnt = 0;
  line = "";

  do
  {
	err = readByte( &byte, timeo );

	if (err == Port::Ok)
	{
	  line += (char)byte;
	}
	else
	{
	  return err;
	}
  }
  while (line.size()<len && byte != '\n' && byte != '\r');

  return Port::Ok;
}

Port::Error Port::readByte( int *byte, int timeo ) const
{
  ssize_t retval;
  unsigned char cByte;

#ifndef __WIN32
#if 0
  do
  {
	retval = ::read( m_handle, &cByte, 1 );
  }
  while (retval != -1 && retval != 1);
#else
  do
  {
	retval = ::read( m_handle, &cByte, 1 );

	if (timeo && retval<0)
	{
	  timeo--;
	  ::usleep( 100000 );
	}
  }
  while (timeo && retval<0);
#endif

  if (retval < 0)
  {
	return errnoValue();
  }
#else
  DWORD num_byte;
  if (!ReadFile( m_handle, &cByte, 1, &num_byte, NULL ))
  {
	return errnoValue();
  }

  if (1!=num_byte) return Port::ReadError;
#endif

  *byte = cByte;

  return Port::Ok;
}

Port::Error Port::writeString( const char *string, ssize_t len ) const
{
  ssize_t retval;
  ssize_t remain = len;
  const char *data = string;

#ifndef __WIN32
  do
  {
	retval = ::write( m_handle, data, remain );
	if (retval != -1)
	{
	  data += retval;
	  remain -= retval;
	}
  }
  while (retval != -1 && remain);

  if (retval < 0)
  {
	return errnoValue();
  }
#else
  DWORD num_byte;

  do
  {
	if (WriteFile( m_handle, data, remain, &num_byte, NULL ))
	{
	  data += num_byte;
	  remain -= num_byte;
	}
	else return errnoValue();
  }
  while (retval != -1 && remain);
#endif

  return Port::Ok;
}

Port::Error Port::readString( char *string, ssize_t len ) const
{
  ssize_t retval;
  ssize_t remain = len;
  char *data = string;

#ifndef __WIN32
  do
  {
	retval = ::read( m_handle, data, remain );
	if (retval != -1)
	{
	  data += retval;
	  remain -= retval;
	}
  }
  while (retval != -1 && remain);

  if (retval < 0)
  {
	return errnoValue();
  }
#else
  DWORD num_byte;

  do
  {
	if (ReadFile( m_handle, data, remain, &num_byte, NULL ))
	{
	  data += num_byte;
	  remain -= num_byte;
	}
	else return errnoValue();
  }
  while (retval != -1 && remain);
#endif

  return Port::Ok;
}

Port::Error Port::errnoValue() const
{
#ifndef __WIN32
  switch (errno)
  {
	case EAGAIN:
	  return NoData;
	case EBADF:
	  return BadFileDescriptor;
	case EFAULT:
	  return BadAddress;
	case EINTR:
	  return Interrupted;
	case EINVAL:
	  return Invalid;
	case EIO:
	  return IOError;
	case EISDIR:
	  return IsDirectory;
  }
#else
#endif

  return UnknownError;
}

const char *Port::errorString( Error err )
{
  switch (err)
  {
	case Ok:
	  return "Ok";
	case SyntaxError:
	  return "Syntax error";
	case PermissionDenied:
	  return "Permission denied";
	case NoSuchPort:
	  return "No such device";
	case ConfigError:
	  return "Configuration error";
	case OpenError:
	  return "Open error";
	case WriteError:
	  return "Write error";
	case ReadError:
	  return "Read error";
	case UnknownBaudRate:
	  return "Unknown baud rate";
	case UnknownNumberOfBits:
	  return "Unknown number of bits";
	case UnknownParity:
	  return "Unknown parity";
	case UnknownStopBits:
	  return "Unknown stop bits";
	case NoData:
	  return "No data available";
	case BadFileDescriptor:
	  return "Bad file descriptor";
	case BadAddress:
	  return "Bad address";
	case Interrupted:
	  return "Interrupted";
	case Invalid:
	  return "Invalid device";
	case IOError:
	  return "I/O error";
	case IsDirectory:
	  return "File is directory";
	case UnknownError:
	  return "Unknown error code";
  }

  return "Unknown error code";
}
