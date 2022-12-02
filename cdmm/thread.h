//======================================================================
// File:        thread.h
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

#ifndef THREAD_HH
#define THREAD_HH

#ifndef __WIN32
#include <pthread.h>

/** \brief Simplistic posix thread class.

    To create a threaded object derive from this class
    and implement the run member function. Check as often
    as you can the m_run member variable and immediately
    return from the function if it is set to false. 
    <p><i>Yes, I know that there are numerous
    libraries that do the same and better. But I find it
    essential to keep the dependencies as low as possible
    for thin clients.</i>
*/
class Thread
{
public:
  /// ctor
  Thread();
  /// dtor
  virtual ~Thread();
      
  /** \brief Start the thread.
  
      \returns pthread_create return code
  */
  int start();
  /** \brief Wait for the thread to finish (no timeout!)
  */
  void wait();
  /** \brief Gracefully stop the thread
  */
  void stop() { m_run = false; }
  /** \brief Forcibly stop thread. 
  
      Not recommended as this might leak resources
  */
  void quit();
  /** \brief Is the thread running?
  */
  bool running() const { return m_run; }
  
protected:
  bool      m_run;
  pthread_t m_pthread;
  
  /** \brief The thread mainloop
  
      This pure virtual function has to be implemented by the thread.
      Put your mainloop in here. Frequently check the m_run flag. If false
      immediately return.
  
      \see m_run running
  */
  virtual void run()=0;
      
private:
  /** pthreads need a "C" compatible function pointer. This static
      function wraps the class member run.
  */
  static void *start_thread( void * );
  
};
#else
#include <windows.h>

/** \brief Simplistic Windows thread class.
*/
class Thread
{
public:
  /// ctor
  Thread();
  /// dtor
  virtual ~Thread();

  /** \brief Start the thread.

      \returns pthread_create return code
  */
  int start();
  /** \brief Wait for the thread to finish (no timeout!)
    Windows implementation has 2s timeout!
  */
  void wait();
  /** \brief Gracefully stop the thread
  */
  void stop() { m_run = false; }
  /** \brief Forcibly stop thread.

      Not recommended as this might leak resources
  */
  void quit();
  /** \brief Is the thread running?
  */
  bool running() const { return m_run; }

protected:
  bool      m_run;
  HANDLE    m_handle;
  DWORD     m_tid;     // thread id

  /** \brief The thread mainloop

      This pure virtual function has to be implemented by the thread.
      Put your mainloop in here. Frequently check the m_run flag. If false
      immediately return.

      \see m_run running
  */
  virtual void run()=0;

private:
  /** We need a "C" compatible function pointer. This static
      function wraps the class member run.
  */
  static DWORD WINAPI start_thread( void * );

};
#endif

#endif // THREAD_HH
