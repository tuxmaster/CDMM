//======================================================================
// File:        metex14bytescontinuous.cpp
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

#include <metex14bytescontinuous.h>

#include <iostream>

Metex14BytesContinuous::Metex14BytesContinuous() :
  MetexAscii( "MMMVVVVVVUUUU\r", "L", false, "", 500 )
{
}

Metex14BytesContinuous::~Metex14BytesContinuous()
{
  m_port.close();
}
