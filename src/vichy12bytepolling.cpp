//======================================================================
// File:        vichy12bytespolling.cpp
// Author:      John Sheahan
// Created:     Mon 23 Jan 2012
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
// Copyright 2012 John Sheahan
//======================================================================

#include <vichy12bytepolling.h>

#include <iostream>


Vichy12BytePolling::Vichy12BytePolling() :
  //  VichyAscii( "MMUUVVVVVVX\n", "L", true, "\x89\x8b\x8a", 250 )
    VichyAscii( "MMUUVVVVVVX\n", "L", true, "\x89", 250 )
{
}

Vichy12BytePolling::~Vichy12BytePolling()
{
}
