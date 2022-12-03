//======================================================================
// File:        thread.cpp
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 18:56:16 CET 2006
// Project:     QtDMM
// Description: Simplest possible posix thread class
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

#include "thread.h"

#include <stdio.h>

Thread::Thread()
{
}

Thread::~Thread()
{
  quit();
}

#ifndef __WIN32
void *Thread::start_thread( void *ptr )
#else
DWORD WINAPI Thread::start_thread( void *ptr )
#endif
{
  Thread *thread = (Thread *)ptr;

  thread->run();

#ifndef __WIN32
  return ptr;
#else
  return 0;
#endif
}

int Thread::start()
{
  m_run = true;

#ifndef __WIN32
  pthread_attr_t attrs;
  ::pthread_attr_init( &attrs );
  return ::pthread_create( &m_pthread, &attrs, Thread::start_thread, this );
#else
  m_handle = CreateThread( 0, // Security attributes
						   0, // Stack size
						   start_thread, // pFun,
						   this, // pArg,
						   0,
						   &m_tid );


  return 0;
#endif
}

void Thread::wait()
{
#ifndef __WIN32
  ::pthread_join( m_pthread, 0 );
#else
  WaitForSingleObject( m_handle, 2000 );
#endif
}

void Thread::quit()
{
#ifndef __WIN32
  ::pthread_exit( 0 );
#else
  CloseHandle( m_handle );
#endif
}
