//======================================================================
// File:        mutex.h
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 18:56:16 CET 2006
// Project:     cdmm
// Description: Simplest possible posix/Windows mutex class
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
// Copyright 2006-2011 Matthias Toussaint
//======================================================================

#ifndef MUTEX_HH
#define MUTEX_HH

#ifndef __WIN32
#include <pthread.h>

/** \brief Simplistic posix mutex class

    Not much to say. lock and unlock to synchronize your threads.
*/
class Mutex
{
public:
  Mutex();
  ~Mutex();
      
  /** \brief Lock the mutex
  
  \returns pthread_mutex_lock return value
  */
  int lock();
  /** \brief Unock the mutex
  
  \returns pthread_mutex_unlock return value
   */
  int unlock();
  /** \brief Try to lock mutex. 
  
  If it is already locked function returns false.
  \returns true is successfull. false if already locked
  */
  bool tryLock();
  /** \brief Check if mutex is already locked
  
  \returns true if mutex is already locked
  */
  bool isLocked();
  
protected:
  pthread_mutex_t m_mutex;
  
};
#else
#include <windows.h>

/** \brief Simplistic Windows mutex class

    Not much to say. lock and unlock to synchronize your threads.
*/
class Mutex
{
public:
  Mutex();
  ~Mutex();

  /** \brief Lock the mutex

  \returns pthread_mutex_lock return value
  */
  int lock();
  /** \brief Unock the mutex

  \returns pthread_mutex_unlock return value
   */
  int unlock();
  /** \brief Try to lock mutex.

  If it is already locked function returns false.
  \returns true is successfull. false if already locked
  */
  bool tryLock();
  /** \brief Check if mutex is already locked

  \returns true if mutex is already locked
  */
  bool isLocked();

protected:
  CRITICAL_SECTION m_criticalSection;
  bool             m_isLocked;

};
#endif

/** \brief Helper class to lock a Mutex inside a scope

    Create a temporary MutexLocker object to lock a mutex
    inside a scope. <p><i>Yes, I know that there are numerous
    libraries that do the same and better. But I find it
    essential to keep the dependencies as low as possible
    for thin clients.</i>
*/
class MutexLocker
{
public:
  MutexLocker( Mutex *mutex ) { m_mutex = mutex; m_mutex->lock(); }
  ~MutexLocker() { m_mutex->unlock(); }
      
private:
  Mutex *m_mutex;

};
    
#endif // MUTEX_HH
