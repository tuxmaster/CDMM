//======================================================================
// File:        mutex.cpp
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 18:56:16 CET 2006
// Project:     QtDMM
// Description: Simplest possible posix mutex class
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

#include "mutex.h"
#include <errno.h>

Mutex::Mutex()
{
#ifndef __WIN32
  pthread_mutex_init( &m_mutex, 0 );
#else
  m_isLocked = false;
  InitializeCriticalSection( &m_criticalSection );
#endif
}

Mutex::~Mutex()
{
#ifdef __WIN32
  DeleteCriticalSection( &m_criticalSection );
#endif
}

int Mutex::lock()
{
#ifndef __WIN32
  return ::pthread_mutex_lock( &m_mutex );
#else
  m_isLocked = true;
  EnterCriticalSection( &m_criticalSection );

  return 42;
#endif
}

int Mutex::unlock()
{
#ifndef __WIN32
  return ::pthread_mutex_unlock( &m_mutex );
#else
  m_isLocked = false;
  LeaveCriticalSection( &m_criticalSection );

  return 42;
#endif
}

bool Mutex::tryLock()
{
#ifndef __WIN32
  return (::pthread_mutex_trylock( &m_mutex ) != EBUSY);
#else
  if (isLocked()) return false;
  lock();
  return true;
#endif
}

bool Mutex::isLocked()
{
#ifndef __WIN32
  bool locked = (::pthread_mutex_trylock( &m_mutex ) == EBUSY);

  if (locked) ::pthread_mutex_unlock( &m_mutex );

  return locked;
#else
  return m_isLocked;
#endif
}
