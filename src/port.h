//======================================================================
// File:        port.h
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

#ifndef PORT_HH
#define PORT_HH

#include <string>
#include <sstream>

#ifdef __WIN32
#include <winsock2.h>
#include <windows.h>
#endif

/** \brief Port is a thing the multimeter is connected to, that can be
    opened, closed and one can read and write.  

    Port is the base class for a multimeter port (which in most of the
    cases would be a SerialPort). UNIX error values are translated to
    typesafe internal values which also can be translated into plain
    english strings.
*/
class Port
{
public:
  enum Error
  {
    Ok = 0,
    SyntaxError,
    PermissionDenied,
    NoSuchPort,
    ConfigError,
    OpenError,
    WriteError,
    ReadError,
    UnknownBaudRate,
    UnknownNumberOfBits,
    UnknownParity,
    UnknownStopBits,
    NoData,
    BadFileDescriptor,
    BadAddress,
    Interrupted,
    Invalid,
    IOError,
    IsDirectory,
    UnknownError
  };
  
  /// ctor
  Port();
  /// dtor
  virtual ~Port();
  
  /** Open the port given a general purpose configuration string
      containing the description of the port. 
      
      \param config The description of the port containing whatever 
                    is needed for the specific port implementation.
  Could be for example <tt><b>/dev/ttyS0:600:8:N:1</b></tt> or 
  <tt><b>localhost:1234</b></tt>
  */
  virtual Error open( const std::string & config )=0;
  /** close the previously opened port
    */
  virtual Error close()=0;
  /** Write a byte to the port.
      \param byte Value to be written
      \returns Error code
  */
  virtual Error writeByte( int byte ) const;
  /** Read a byte from the port. 
      \param byte Value to be read. Is invalid if an error occured.
      \returns Error code
   */
  virtual Error readByte( int *byte, int timeo=10 ) const;
  /** Write a string to the port.
      \param string String to be written
      \param len Number of bytes to be written
      \returns Error code
  */
  virtual Error writeString( const char *string, ssize_t len ) const;
  /** Read a string from the port.
      \param string String to be read.
      \param len Number of bytes to be read
      \returns Error code
   */
  virtual Error readString( char *string, ssize_t len ) const;
  /** Read line until newline
      \param string String to be read.
      \param len Maximum number of bytes to be read
      \returns Error code
  */
  virtual Error readLine( std::string & line, ssize_t len, int timeo=10 ) const;
  /** Translate Error code into a human readable string.
      \param err Error code (as returned by the read/write/open/close)
                 members of this class
      \returns Pointer to human readable string
  */
  static const char *errorString( Error err );
   
protected:
#ifndef __WIN32
  int m_handle;
#else
  HANDLE m_handle;
#endif

  /** Translate errno into internal value
  \returns Error code corresponding to the current value of errno
  */
  Error errnoValue() const;
  
};

#endif // PORT_HH
