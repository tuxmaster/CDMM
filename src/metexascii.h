//======================================================================
// File:        metexascii.h
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 18:12:23 CET 2006
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

#ifndef METEXASCII_HH
#define METEXASCII_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief Base implementation of the Metex ASCII protocoll

    The multimeter responds with a ascii string
    <p>
    14 byte format description:<br>
    MMMVVVVVVUUUUS
    <p>
    M: Mode<br>
    V: Value (including sign)<br>
    U: Unit<br>
    S: Sync byte (0x0d)
 */ 
class MetexAscii : public DMMClass
{
public:
  /** \brief Constructor
      \param format String describing the protokoll. <br>
                    Example: "MMMVVVVVVUUUU\r"<br>
                    M: Mode<br>
                    V: Value<br>
                    U: Unit<br>
                    This would describe a string with 3 mode bytes
                    followed by 6 value bytes followed again by 4
                    unit bytes. The sync byte would be '\\r' (0x0d).
                    This is the standard 14 byte format.
      \param overflowString If this (sub)string is found int the
                            value the measurement is considered
                            to be invalid (overflow). All known (to me)
                            Metex and Voltcraft multimeter send 'L' in
                            some combination ("0L" or "0.L" or ".L")
      \param poll True if the multimeter has to be polled
      \param pollString ASCII string to be sent to the multimeter
                        for polling
      \param pollTime Repetition time for polling in milliseconds
  */
  MetexAscii( const std::string & format,
              const std::string & overflowString,
              bool poll, const std::string & pollString, int pollTime  );
  virtual ~MetexAscii();
  
  virtual Port::Error close();
  
protected:
  SerialPort  m_port;
  int         m_length;
  int         m_syncByte;
  int         m_modeStart, m_modeLen;
  int         m_valueStart, m_valueLen;
  int         m_unitStart, m_unitLen;
  std::string m_overflowStr;
  bool        m_poll;
  std::string m_pollString;
  int         m_pollTime;
  
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  void setMetexAscii( const std::string & str, int index );
  
};

#endif // METEX14BYTESASCII_HH
