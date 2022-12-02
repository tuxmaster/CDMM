//======================================================================
// File:        metex14bytespolling.h
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

#ifndef METEX14BYTESPOLLING_HH
#define METEX14BYTESPOLLING_HH

#include <metexascii.h>

/** \brief Implementation of the Metex 14 byte protocoll in the polling
    flavour. 

    The multimeter responds with  it's 14 byte string (ASCII) after 
    receiving the sequence D, 0x0d. We poll every 250ms (The ME-32 survived this. Don't know about other models)
    <p>
    Format description:<br>
    MMMVVVVVVUUUUS
    <p>
    M: Mode<br>
    V: Value (including sign)<br>
    U: Unit<br>
    S: Sync byte (0x0d)
 */ 
class Metex14BytesPolling : public MetexAscii
{
public:
  Metex14BytesPolling();
  virtual ~Metex14BytesPolling();
  
  static DMMClass *create() { return new Metex14BytesPolling; }
  
protected:
  
};

#endif // METEX14BYTESPOLLING_HH
