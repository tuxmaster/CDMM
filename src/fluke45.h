//======================================================================
// File:        fluke45.h
// Author:      Matthias Toussaint
// Created:     Tue Dec 20 00:09:48 CET 2011
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
// Copyright 2011 Matthias Toussaint
//======================================================================

#ifndef FLUKE45_HH
#define VC870_HH

#include <dmmclass.h>
#include <serialport.h>

/** \brief VC870 14 byte binary protocoll
  
 */ 
class Fluke45 : public DMMClass
{
public:
  /** \brief Constructor
   */
  Fluke45();
  virtual ~Fluke45();
  
  virtual Port::Error close();
  
  static DMMClass *create() { return new Fluke45; }
  
protected:
  SerialPort  m_port;
  std::string m_rate;
  std::string m_func1;
  std::string m_range1;
  std::string m_func2;
  std::string m_range2;

  virtual Port::Error open_impl( const std::string & config );
  virtual void run();
  
  Port::Error readData( std::string & );
  bool findOverflow( const char *byte );
  void checkRange( const std::string & range, int index );
  void checkFunc( const std::string & func, int index );
  
};

#endif // FLUKE45_HH
