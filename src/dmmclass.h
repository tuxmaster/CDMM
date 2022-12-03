//======================================================================
// File:        dmmclass.h
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
// Copyright 2006-2007 Matthias Toussaint
//======================================================================

#ifndef DMMCLASS_HH
#define DMMCLASS_HH

#include "thread.h"
#include "mutex.h"
#include "port.h"
#include <string>
#include <time.h>
#include "util.h"

#include <stdlib.h>

//class DMMClass;

/** \brief Base class for a Multimeter class (Protokoll version)

	DMMClass is the base class for a multimeter type. It provides
	the API to the multimeter. It is a thread that has to countinuously
	read the value(s) from the multimeter. Another thread can
	retrieve the last values from the multimeter by calling the member
	function value including unit and mode.

	<h1>How to create a new protocol</h1>
	Create a class derived from DMMClass. This new class is responsible
	for reading and decoding of the data sent by the multimeter.
	<p>Most probably the device is connected through a serial port
	(or USB serial). Use the SerialPort class to handle the serial port.
	If you have special needs it's a good idea to create a new subclass
	of Port for this purpose.
	<p>Implement: #open_impl, #close and #run.
	<p>Implement a static create function (DMMClass *create( void ))
	\see Metex14BytesPolling::create, Metex14BytesContinuous::create,
		Metex15BytesContinuous::create, VC820::create, M9803R::create ...
	<p>Include the headerfile of your class in the
	file dmmclass.cpp and add the new protocol to the array #s_protocol.

	\see value, mode and unit
*/
class DMMClass : public Thread
{
public:
  DMMClass();
  virtual ~DMMClass();

  /** \brief Open the port

	  \see Description of the parameter config can be found
	  in the Port description
  */
  Port::Error open( const std::string & config );
  /** \brief Close the port
  */
  virtual Port::Error close()=0;

  /** \brief Set number of values this partucular multimeter provides

	  Some multimeter may have more than on value. This class
	  supports up to 8 values.
  \param[in] num Number of parameter of this device
  */
  void setNumValues( int num ) { m_numValues = num; }
  /** \brief Return number of values this partucular multimeter provides

	  Some multimeter may have more than one value. This class
	  supports up to 8 values.
	  \returns The number of avail values
  */
  int numValues() const { return m_numValues; }
  void setNumCounts( int nc ) { m_numCounts=nc; }
  int numCounts() const { return m_numCounts; }
  double minValue( int );
  double maxValue( int );

  /** \brief The current measurement mode.

	  The specific implementation has to
	  set these to one of the following (The GUI QtDMM depends on this).
	  <p>
	  <table cellpadding="10" cellspacing="0" border="1">
	  <tr bgcolor="#eeeeee"><td><b>DC</b></td><td>A DC measurement [V or A]</td></tr>
	  <tr><td><b>AC</b></td><td>A AC measurement [V or A]</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>OH</b></td><td>Resistance [Ohm]</td></tr>
	  <tr><td><b>DI</b></td><td>Diode "Sperrspannung" (Translators please) in V</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>CA</b></td><td>Capacity [Farad]</td></tr>
	  <tr><td><b>IN</b></td><td>Inductance [Henry]</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>TE</b></td><td>Temperature [C or F]</td></tr>
	  <tr><td><b>FR</b></td><td>Frequency [Hz]</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>DU</b></td><td>Duty cycle in %</td></tr>
	  <tr><td><b>SO</b></td><td>Lautst&auml;rke [dB]</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>LU</b></td><td>Luminosity [Lux]</td></tr>
	  <tr><td><b>PO</b></td><td>Power [dBm]</td></tr>
	  <tr bgcolor="#eeeeee"><td><b>PW</b></td><td>Power [W]</td></tr>
	  <tr><td><b>RP</b></td><td>Rotation rate [rpm]</td></tr>
	  </table>
  \param[in] i Index of value [0-7]
  */
  std::string mode( int i=0 );
  /** \brief String representation of the unit.

	  Example: p, n, u, m, k, M, G
  \param[in] i Index of value [0-7]
  */
  std::string prefix( int i=0 );
  /** \brief String representation of the unit.

	  Example: V, A, F, H, Ohm, %.
  \param[in] i Index of value [0-7]
  */
  std::string unit( int i=0 );
  /** \brief The measured value.

  \param[in] i Index of value [0-7]
   */
  std::string value( int i=0 );
  /** \brief The measured value.

  \param[in] i Index of value [0-7]
   */
  double realValue( int i=0 );

  /** \brief Do we have a measurement already?

	  \returns true if something was measured
  */
  bool hasValue() { MutexLocker lock( &m_mutex ); return m_hasValue; }
  void resetHasValue() { MutexLocker lock( &m_mutex ); m_hasValue=false; }
  /** \brief An overflow happened.

	  The Implementation of the multimeter class
	  has to take care of this.
   */
  bool overflow( int i ) { MutexLocker lock( &m_mutex ); return m_overflow[i]; }
  /** \brief Battery is low.

	  The Implementation of the multimeter class
	  has to take care of this.
   */
  bool lowBat() { MutexLocker lock( &m_mutex ); return m_lowBat; }

  void setConsoleLogging( bool on ) { m_consoleLogging = on; }
  void setConsoleLoggingSyncSequence( const std::string & sync_sequence );
  void setConsoleLoggingPollSequence( const std::string & poll_sequence );

  void writeLog( unsigned char byte );
  void resetLog() { m_logCount=0; }

  /** \brief Check if there was a timeout

  If the multimeter didn't send data for 5 seconds this goes true
   */
  bool timeout(int timeout_val) const;

  void setLimits( double dcv, double acv, double dca, double aca, double mint, double maxt, bool ffs )
  {
	m_maxDCV = dcv;
	m_maxACV = acv;
	m_maxDCA = dca;
	m_maxACA = aca;
	m_minTemp = mint;
	m_maxTemp = maxt;
	m_freqFullScale = ffs;
  }

  /** \brief Information structure for a multimeter protocol
	*/
  class Protocol
  {
  public:
	Protocol( DMMClass *(*_fkt)(void),
			  const std::string & _name,
			  const std::string & _menuText ) :
	  createFunction( _fkt ),
	  name( _name ),
	  menuText( _menuText )
	{
	}
	/// The createFunction returns a new DMMClass object
	DMMClass *(*createFunction)(void);
	/// The short protocol name
	std::string name;
	/// A more verbose text for menus
	std::string menuText;
  };

  /** \brief Information structure for a multimeter device
	*/
  class Device
  {
  public:
	Device();
	Device( const std::string & _brand,
			const std::string & _name,
			const std::string & _protocol,
			const std::string & _port,
			int _numValues,
			int _numCounts,
			double _maxVdc,
			double _maxVac,
			double _maxAdc,
			double _maxAac,
			double _minTemp,
			double _maxTemp
		  ) :
	  brand( _brand ),
	  name( _name ),
	  protocol( _protocol ),
	  port( _port ),
	  numValues( _numValues ),
	  numCounts( _numCounts ),
	  maxVdc( _maxVdc ),
	  maxVac( _maxVac ),
	  maxAdc( _maxAdc ),
	  maxAac( _maxAac ),
	  minTemp( _minTemp ),
	  maxTemp( _maxTemp )
	{
	}
	/// Brand name of device
	std::string brand;
	/// Model name of device
	std::string name;
	/// Protocol name for this device
	std::string protocol;
	/// Port description for this device (without devicenode)
	std::string port;
	/// Number of values this device provides
	int numValues;
	/// Number of counts for the display
	int numCounts;
	double maxVdc;
	double maxVac;
	double maxAdc;
	double maxAac;
	double minTemp;
	double maxTemp;
  };

  /** \brief Create an instance of the given protocol.

	  \param[in] protocolName Name of the protocol. Currently implemented
	  are:<p>
  <table cellpadding="10" cellspacing="0" border="1">
  <tr bgcolor="#bbbbbb"><td><b>Name</b></td><td><b>Protocol class</b></td></tr>
  <tr><td>M14P</td><td>Metex14BytesPolling</td></tr>
  <tr bgcolor="#eeeeee"><td>M14C</td><td>Metex14BytesContinuous</td></tr>
  <tr><td>M15C</td><td>Metex15BytesContinuous</td></tr>
  <tr bgcolor="#eeeeee"><td>VC820</td><td>VC820</td></tr>
  <tr><td>M9803R</td><td>M9803R</td></tr>
  <tr bgcolor="#eeeeee"><td>IOCB</td><td>IOCBUVMonitor</td></tr>
  <tr><td>PT10A</td><td>PeakTech10Ascii</td></tr>
  <tr bgcolor="#eeeeee"><td>GDM703</td><td>GDM703</td></tr>
  <tr><td>ESC97</td><td>Escort97</td></tr>
  <tr bgcolor="#eeeeee"><td>VC940</td><td>VC940</td></tr>
  <tr><td>QM1537</td><td>QM1537</td></tr>
  <tr bgcolor="#eeeeee"><td>PT506</td><td>PT506</td></tr>
  <tr><td>VC870</td><td>VC870</td></tr>
  </table>
	  \returns Pointer to new instance of the requested protocol or 0
			   if the protocol is unknown
  */
  static DMMClass *create_protocol( std::string protocolName );
  /** \brief Return number of known protocols
  */
  static int num_protocols();
  /** \brief Return protocol description

	  \param i Index of the protocol
			must be smaller than num_protocols())
	  \returns Protocol description
  */
  static Protocol protocol( int i ) { return s_protocol[i]; }
  /** \brief Return list of known protocol names separated by '\n'
   */
  static std::string print_protocols();
  /** \brief Return list of known devices separated by '\n'
   */
  static std::string print_devices();
  /** \brief Test if the devicename is known

	  \param deviceName Name of the device ("Voltcraft ME-32")
	  \returns true if device is known
  */
  static bool has_device( const std::string & deviceName );
  /** \brief Get device description
	  \param deviceName Name of the device ("Voltcraft ME-32")
	  \return Device description
  */
  static Device get_device( const std::string & deviceName );
  /** Return table with information about all known devices
   */
  static std::string device_table();

protected:
  /// Seconds since epoch when port was opened. This is needed for the timeout
  time_t      m_openTime;
  time_t      m_lastValueTime;
  /// Set to true if a valid value has been read from the multimeter
  bool        m_hasValue;
  /// Stores the number of values this multimeter provides
  int         m_numValues;
  /// Stores the number of counts this multimeter provides
  int         m_numCounts;
  /// Set to true if an overflow happened (And clear if the overflow condition is gone)
  bool        m_overflow[8];
  /// Set true if multimeter indicates low battery state
  bool        m_lowBat;
  /// Hold this mutex when reading or writing values
  Mutex       m_mutex;
  /// The mode \see mode
  std::string m_mode[8];
  /// The unit prefix \see unit
  std::string m_prefix[8];
  /// The unit \see unit
  std::string m_unit[8];
  /// The value as string \see value
  std::string m_value[8];
  /// The value as string \see value
  double m_rangeMin[8];
  /// The value as string \see value
  double m_rangeMax[8];
  /// The value as double \see realValue
  double m_realValue[8];

  bool          m_consoleLogging;
  int           m_logCount;
  int           m_logSyncCount;
  int           m_logSyncSize;
  unsigned char m_logSyncSequence[256];
  unsigned char m_logPollSequence[256];
  int           m_logPollSize;
  unsigned char m_logBuffer[256];
  double        m_maxDCV;
  double        m_maxACV;
  double        m_maxDCA;
  double        m_maxACA;
  double        m_minTemp;
  double        m_maxTemp;
  bool          m_freqFullScale;
  int           m_deviceTimeout;

  static Protocol s_protocol[];
  static Device s_device[];

  /** \brief The main loop

	  Continuously read the multimeter here (And do whatever is
	  needed to make the multimeter send values [polling]). New
	  values have to be filled into m_value, m_mode and m_unit.
	  Hold the mutex while doing so.

	  Example:
	  <pre>
  ... we assume a 14 byte protocol ...

  unsigned char buffer[64];

  while (m_run)
  {
	int cnt = readData( buffer );

	if (-1 != cnt)
	{
	  unsigned char *byte = buffer+cnt-14;

	  ... decode the raw multimeter data ...

	  m_mutex.lock();
	  m_value[0] = ...
	  m_unit[0] = ...
	  m_mode[0] = ...
	  m_hasValue = true;
	  m_mutex.unlock();
	}
  }
  </pre>
	  \see m_value, m_unit and m_mode
  */
  virtual void run()=0;

  /** Helper function.
	  Read data from the given port.
	  \param[in] port The Port to read from
	  \param[out] buffer The data is copied into this buffer. Make shure
				 that it is big enough to hold at least maxLen bytes.
	  \param[in] syncByte Stop reading when the sync byte has been read (and
				 at least minLen bytes have been read.
	  \param[in] minLen Read at least minLen bytes
	  \param[in] maxLen Abort reading if maxLen bytes have been read without
				 the occurence of the syncByte

	  \returns The number of bytes read or -1 if reading failed
  */
  int readData( Port *port, unsigned char *buffer, int syncByte,
				int minLen, int maxLen, int timeo=0 );

  void setValue( int index, std::string value );

  /** \brief Open the port (the real thing)

  \see Description of the parameter config can be found
  in the Port description
   */
  virtual Port::Error open_impl( const std::string & config ) = 0;

  std::string insertDecimalPoint( const std::string &, int pos );

  unsigned char token2Byte( const std::string & token ) const;
  void setHasValue() { m_hasValue=true; m_lastValueTime=time(0); }

  /** Set range by guessing it from counts and decimal point.
	  If the driver knows it better it should do that individually.
	  It obeys the maximum settings for AC/DC Volt and Amperes
  */
  void setRange( int index );

private:
  static int count_protocols();

};

#endif // DMMCLASS_HH
