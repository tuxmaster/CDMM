//======================================================================
// File:        gdm703.h
// Author:      Matthias Toussaint
// Created:     Tue Nov 28 08:24:21 CET 2006
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

#ifndef GDM703_HH
#define GDM703_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief Implementation of the GDM703 ASCII protocoll

    The multimeter continuously sends a 26 byte ascii string containing two values
    <p>
    26 byte format description:<br>
    Smmvvvvvvuuuu BVVVVVVUUUUE
    <p>
    S: Start (0x02)<br>
    m: Mode 1<br>
    v: Value 1 (including sign)<br>
    u: Unit 1<br>
    B: Character B<br>
    V: Value 2 (including sign)<br>
    U: Unit 2<br>
    E: End (0x03)<br>
 */ 
class GDM703 : public DMMClass
{
public:
  /** \brief Constructor
  */
  GDM703();
  virtual ~GDM703();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new GDM703; }
  
protected:
  SerialPort  m_port;
  
  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  void setAscii( int start, int modeLen, const std::string & str, int index );
      
};

#endif // GDM703_HH
