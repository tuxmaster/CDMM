//======================================================================
// File:        main.cpp
// Author:      Matthias Toussaint
// Created:     Sat Nov 25 15:37:12 CET 2006
// Project:     QtDMM
// Description: Console based multimeter readout program
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

#include "dmmclass.h"
#include "configuration.h"
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "telnetserver.h"
#include "udpserver.h"
#include <stdio.h>

Configuration *create_config();

/** \mainpage cdmm

	\section intro_sec Introduction

	cdmm is a small console application to read the output of digital
	multimeters connected to the computer (mostly serial or usb-serial).
	Currently it supports 9 different protocols and has built in parameters
	for more than 50 devices.

	If your device isn't listed try playing with the parameters. The chance
	is quite good (As most multimeter are rebranded) that you get it working
	with the settings for a different multimeter or by manually adjusting
	the parameters. If you find a new working device, please report to
	<a href="mailto:qtmm@mtoussaint.de">qtmm\@mtoussaint.de</a>. This way
	future users of this device can benefit from your help.

	For a list of the currently supported multimeters go <a href="../index.html">here</a>.

	If you really need to implement a new protocol start reading the
	documentation for #DMMClass.

	Have fun<br>
	Matthias Toussaint
*/

void printDate( bool utc );
void printTime( bool utc );
void printMs( int64_t start_time );
int64_t getMs();

int main( int argc, char **argv )
{
  Configuration *config = create_config();

  if (!config->setCommandLine( argc, argv ))
  {
	std::cout << "Usage: cdmm [options] | --help" << std::endl;
	exit(-1);
  }

  // version
  //
  if (config->hasKey( "version" ))
  {
	std::cout << "cdmm 0.4" << std::endl;
	std::cout << "Copyright 2006-2012 Matthias Toussaint" << std::endl;
	exit(0);
  }

  // help
  //
  if (config->hasKey( "help" ))
  {
	std::cout << config->helpText();
	exit(0);
  }

  // check for print flags
  //
  if (config->hasKey( "device-table" ))
  {
	std::cout << DMMClass::device_table() << std::endl;
	exit(0);
  }

  // check for print flags
  //
  bool showList=false;

  if (config->hasKey( "list-protocols" ))
  {
	showList = true;
	std::cout << DMMClass::print_protocols();
  }

  if (config->hasKey( "list-devices" ))
  {
	showList = true;
	std::cout << DMMClass::print_devices();
  }

  if (showList) exit(0);

  // configuration file ?
  //
  if (config->hasKey( "file" ))
  {
	if (!config->loadFile( config->stringValue( "file" )))
	{
	  exit(-1);
	}
	// commandline always overrides
	config->setCommandLine( argc, argv );
  }

#ifndef __WIN32
  int sleeptime = 1000;
#else
  int sleeptime = 1;
#endif
  if (config->hasKey( "interval" ))
  {
#ifndef __WIN32
	sleeptime = (int)( config->doubleValue( "interval" ) * 1000000.0 );
	if (sleeptime < 100000) sleeptime = 100000;
#else
	sleeptime = (int)( config->doubleValue( "interval" ) * 1000.0 );
	if (sleeptime < 100) sleeptime = 100;
#endif
  }

  //config->printEntries();

  std::string protocol;
  std::string port;

  if (!config->hasKey( "port" ))
  {
	std::cerr << "Port missing" << std::endl;
	exit(-1);
  }
  else port = config->stringValue( "port" );

  if (config->hasKey( "port-settings" ))
  {
	port += ":";
	port += config->stringValue( "port-settings" );
  }

  if (!config->hasKey( "console-logging" ))
  {
	if (!config->hasKey( "device" ))
	{
	  if (!config->hasKey( "protocol" ))
	  {
		std::cerr << "Protocol missing" << std::endl;
		exit(-1);
	  }
	  else protocol = config->stringValue( "protocol" );
	}
	else
	{
	  if (DMMClass::has_device( config->stringValue( "device" )))
	  {
		if (config->hasKey( "protocol" ))
		{
		  std::cerr << "Ignoring --protocol. Device already specified"
					<< std::endl;
		}

		protocol = DMMClass::get_device( config->stringValue( "device" )).protocol;
		port += ":";
		port += DMMClass::get_device( config->stringValue( "device" )).port;
	  }
	  else
	  {
		std::cerr << "Unknown device: " << config->stringValue( "device" )
				  << std::endl;
		exit( -1 );
	  }
	}
  }
  else
  {
	protocol = "Logger";
  }

  DMMClass *dmm = DMMClass::create_protocol( protocol );

  if (0 == dmm)
  {
	std::cerr << "Unknow protocol: " << protocol
			  << std::endl;
	exit(-1);
  }

  // Display limits
  //
  int    counts   =  4000;
  double max_vdc  =  1000;
  double max_vac  =  750;
  double max_adc  =  10;
  double max_aac  =  10;
  double min_temp = -50;
  double max_temp =  250;

  if (config->hasKey( "device" ))
  {
	counts = DMMClass::get_device( config->stringValue( "device" )).numCounts;
	max_vdc = DMMClass::get_device( config->stringValue( "device" )).maxVdc;
	max_vac = DMMClass::get_device( config->stringValue( "device" )).maxVac;
	max_adc = DMMClass::get_device( config->stringValue( "device" )).maxAdc;
	max_aac = DMMClass::get_device( config->stringValue( "device" )).maxAac;
	min_temp = DMMClass::get_device( config->stringValue( "device" )).minTemp;
	max_temp = DMMClass::get_device( config->stringValue( "device" )).maxTemp;
  }

  if (config->hasKey( "counts" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --counts. Device already specified"
				<< std::endl;
	}
	else counts = config->intValue( "counts" );
  }

  if (config->hasKey( "max-vdc" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --max-vdc. Device already specified"
				<< std::endl;
	}
	else max_vdc = config->intValue( "max-vdc" );
  }

  if (config->hasKey( "max-vac" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --max-vac. Device already specified"
				<< std::endl;
	}
	else max_vac = config->intValue( "max-vac" );
  }

  if (config->hasKey( "max-adc" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --max-adc. Device already specified"
				<< std::endl;
	}
	else max_adc = config->intValue( "max-adc" );
  }

  if (config->hasKey( "max-aac" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --max-aac. Device already specified"
				<< std::endl;
	}
	else max_aac = config->intValue( "max-aac" );
  }

  if (config->hasKey( "min-temp" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --min-temp. Device already specified"
				<< std::endl;
	}
	else min_temp = config->intValue( "min-temp" );
  }

  if (config->hasKey( "max-temp" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --max-temp. Device already specified"
				<< std::endl;
	}
	else max_temp = config->intValue( "max-temp" );
  }

  dmm->setNumCounts( counts );
  dmm->setLimits( max_vdc, max_vac, max_adc, max_aac, min_temp, max_temp, true );

  // Logging
  //
  bool console_logging = false;
  if (config->hasKey( "console-logging" ))
  {
	dmm->setConsoleLogging( true );
	console_logging = true;
  }

  if (config->hasKey( "logging-sync-sequence" ))
  {
	dmm->setConsoleLoggingSyncSequence( config->stringValue( "logging-sync-sequence" ) );
  }

  if (config->hasKey( "logging-poll-sequence" ))
  {
	dmm->setConsoleLoggingPollSequence( config->stringValue( "logging-poll-sequence" ) );
  }

  if (config->hasKey( "num-values" ))
  {
	if (config->hasKey( "device" ))
	{
	  std::cerr << "Ignoring --num-values. Device already specified"
				<< std::endl;
	  dmm->setNumValues( DMMClass::get_device(
		  config->stringValue( "device" )).numValues );
	}
	else
	{
	  dmm->setNumValues( config->intValue( "num-values" ) );
	}
  }

  if (config->hasKey( "device" ))
  {
	dmm->setNumValues( DMMClass::get_device(
		config->stringValue( "device" )).numValues );
  }

  // Output options
  //
  bool showDate = false;
  if (config->hasKey( "show-date" )) showDate = true;

  bool showTime = false;
  if (config->hasKey( "show-time" )) showTime = true;

  bool showMs = false;
  if (config->hasKey( "show-ms" )) showMs = true;

  bool utc = false;
  if (config->hasKey( "utc" )) utc = true;

  bool showRange = false;
  if (config->hasKey( "show-range" )) showRange = true;

  bool quiet = false;
  if (config->hasKey( "quiet" )) quiet = true;

  bool table = false;
  if (config->hasKey( "table" )) table = true;

  // Telnet options
  //
  int telnetPort = -1;
  if (config->hasKey( "telnet-port" ))
  {
	telnetPort = config->intValue( "telnet-port" );
  }

  int telnetMaxConnect = 3;
  if (config->hasKey( "telnet-max-connections" ))
  {
	telnetMaxConnect = config->intValue( "telnet-max-connections" );
  }

  // UDP options
  //
  int udpPort = -1;
  if (config->hasKey( "udp-port" ))
  {
	udpPort = config->intValue( "udp-port" );
  }

  int timeout_val = 5;
  if (config->hasKey( "timeout" ))
  {
	timeout_val = config->intValue( "timeout" );
  }

  TelnetServer *telnet = 0;
  UdpServer *udp = 0;

  if (Port::Ok == dmm->open( port ))
  {
	dmm->start();

	if (telnetPort != -1)
	{
	  telnet = new TelnetServer( dmm, telnetPort, telnetMaxConnect );
	  telnet->start();
	}

	if (udpPort != -1)
	{
	  udp = new UdpServer( dmm, udpPort );
	  udp->start();
	}

	if (!quiet && !console_logging && table)
	{
	  if (showDate) std::cout << "Date\t";
	  if (showTime) std::cout << "Time\t";
	  if (showMs) std::cout << "ms\t";
	  for (int i=0; i<dmm->numValues(); ++i)
	  {
		std::cout << "Value";
		std::cout << "\t" << "Unit" << "\t" << "Mode" << "\t";
		if (showRange)
		{
		  std::cout << "Min" << "\t" << "Max" << "\t";
		}
	  }
	  std::cout << std::endl;
	}

	int64_t start_time = getMs();

	while (1)
	{
	  if (dmm->timeout(timeout_val))
	  {
		std::cerr << "Timeout!" << std::endl;
		dmm->close();
		exit(-1); // windows doesn't exit here?
	  }

	  if (!quiet && !console_logging)
	  {
		if (dmm->hasValue())
		{
		  dmm->resetHasValue();
		  if (table)
		  {
			if (showDate)
			{
			  printDate( utc );
			  std::cout << "\t";
			}
			if (showTime)
			{
			  printTime( utc );
			  std::cout << "\t";
			}
			if (showMs)
			{
			  printMs( start_time );
			  std::cout << "\t";
			}

			for (int i=0; i<dmm->numValues(); ++i)
			{
			  if (!dmm->overflow(i))
			  {
				std::cout << dmm->value(i);
			  }
			  else
			  {
				std::cout << "OL";
			  }
			  std::cout << "\t" << dmm->prefix(i) << dmm->unit(i) << "\t" << dmm->mode(i);

			  if (showRange)
			  {
				std::cout << "\t" << dmm->minValue(i) << "\t" << dmm->maxValue(i) << "\t";
			  }
			}
			std::cout << std::endl;
		  }
		  else
		  {
			for (int i=0; i<dmm->numValues(); ++i)
			{
			  std::cout << "V" << i << ":\t";

			  if (showDate)
			  {
				printDate( utc );
				std::cout << "\t";
			  }
			  if (showTime)
			  {
				printTime( utc );
				std::cout << "\t";
			  }
			  if (showMs)
			  {
				printMs( start_time );
				std::cout << "\t";
			  }

			  if (showRange)
			  {
				std::cout << dmm->minValue(i) << "\t" << dmm->maxValue(i) << "\t";
			  }

			  if (!dmm->overflow(i))
			  {
				std::cout << dmm->value(i);
			  }
			  else
			  {
				std::cout << "OL";
			  }

			  std::cout << "\t" << dmm->prefix(i) << dmm->unit(i) << "\t" << dmm->mode(i) << std::endl;
			  //printf( "%.6e\n", dmm->realValue(i) );
			}
		  }
		}
	  }
#ifndef __WIN32
	  usleep( sleeptime );
#else
	  Sleep( sleeptime );
	  //Sleep( sleeptime );
#endif
	}

	dmm->stop();
  }
  else std::cerr << "Open failed!" << std::endl;

  delete telnet;
  delete udp;

  return 0;
}

Configuration *create_config()
{
  Configuration *config = new Configuration;
  config->addOption( Configuration::Option( "v", "version", false,
					 "Show version and exit\n" ) );
  config->addOption( Configuration::Option( "q", "quiet", false,
					 "Don't output to stdout\n" ) );
  config->addOption( Configuration::Option( "f", "file", true,
					 "Parameter file\n" ) );
  config->addOption( Configuration::Option( "p", "port", true,
					 "Port description\n" ) );
  config->addOption( Configuration::Option( "", "port-settings", true,
					 "Port settings\n" ) );
  config->addOption( Configuration::Option( "P", "protocol", true,
					 "Protocol name\n" ) );
  config->addOption( Configuration::Option( "C", "counts", true,
					 "Set number of counts (Example: 4000 [not 3999]). Default 4000\n" ) );
  config->addOption( Configuration::Option( "", "max-vdc", true,
					 "Maximum DC Volts. Default 1000V\n" ) );
  config->addOption( Configuration::Option( "", "max-vac", true,
					 "Maximum AC Volts. Default 750V\n" ) );
  config->addOption( Configuration::Option( "", "max-adc", true,
					 "Maximum DC Amperes. Default 10A\n" ) );
  config->addOption( Configuration::Option( "", "max-aac", true,
					 "Maximum AC Amperes. Default 10A\n" ) );
  config->addOption( Configuration::Option( "", "min-temp", true,
					 "Minimum temperature. Default -50°C\n" ) );
  config->addOption( Configuration::Option( "", "max-temp", true,
					 "Maximum temperature. Default +250°C\n" ) );
  config->addOption( Configuration::Option( "", "interval", false,
					 "Show value every interval seconds (float value > 0.1)\n" ));
  config->addOption( Configuration::Option( "", "show-time", false,
					 "Show value with time hh:mm:ss.s\n" ));
  config->addOption( Configuration::Option( "", "show-date", false,
					 "Show value with date yyyy:dd:mm\n" ));
  config->addOption( Configuration::Option( "", "show-ms", false,
					 "Show time in ms since start\n" ));
  config->addOption( Configuration::Option( "", "utc", false,
					 "Show date/time in UTC\n" ));
  config->addOption( Configuration::Option( "r", "show-range", false,
					 "Show current range\n" ));
  config->addOption( Configuration::Option( "h", "help", false,
					 "Show helptext\n" ) );
  config->addOption( Configuration::Option( "v", "version", false,
					 "Show version\n" ) );
  config->addOption( Configuration::Option( "t", "table", false,
					 "Output data as table\n" ) );
  config->addOption( Configuration::Option( "l", "list-protocols", false,
					 "Show list of implemented protocols\n" ) );
  config->addOption( Configuration::Option( "L", "list-devices", false,
					 "Show list of known devices\n" ) );
  config->addOption( Configuration::Option( "d", "device", true,
					 "Name of the device (See --list-devices)\n" ) );
  config->addOption( Configuration::Option( "n", "num-values", true,
					 "Number of values this device provides\n" ) );
  config->addOption( Configuration::Option( "", "console-logging", false,
					 "Enable raw output to console\n" ) );
  config->addOption( Configuration::Option( "", "logging-sync-sequence", true,
					 "Byte sequence for syncing of console logging (comma separated)\n" ) );
  config->addOption( Configuration::Option( "", "logging-poll-sequence", true,
					 "Byte sequence for polling data when in console loggin (comma separated)\n" ) );
  //config->addOption( Configuration::Option( "", "telnet-port", true,
  //                   "Switch on telnet mode and set port.\n    Commands are: start [interval], stop, quit|bye and help\n" ) );
  //config->addOption( Configuration::Option( "", "telnet-max-connections", true,
  //                   "Set maximum simultaneous telnet connections (default 3)\n" ) );
  config->addOption( Configuration::Option( "", "device-table", false,
					 "Print a table containing the devices\n" ));
  config->addOption( Configuration::Option( "", "udp-port", true,
					 "Switch on udp mode and set port.\n     Sending a udp packet gets exactly one response, the latest valid value\n" ) );
  config->addOption( Configuration::Option( "", "timeout", true,
											"Set timout for multimeter valid response. Default is 5 seconds\n" ) );

  config->setHelpText( "Example: /dev/ttyS0:600:8:n:1:RTS"
	  "\nExample: Metex ME-32 at /dev/ttyUSB0\n"
	  "cdmm --port=/dev/ttyUSB0:600:7:n:2:DSR:DTR:CTS --protocol=M14P or\n"
	  "cdmm --device=\"Voltcraft ME-32\" --port=/dev/ttyUSB0\n"
	  "\nThe corresponding parameter file for the last call\nwould be:\n"
	  "device = Voltcraft ME-32\n"
	  "port   = /dev/ttyUSB0\n"
	  "\ncdmm 0.4 Copyright 2006-2012 Matthias Toussaint\n" );
  return config;
}

void printDate( bool utc )
{
  struct timeval tv;
  gettimeofday( &tv, NULL );

  time_t epoch = tv.tv_sec;
  struct tm *tmp;

  if (utc) tmp = gmtime( &epoch );
  else     tmp = localtime( &epoch );

  char str[256];
  sprintf( str, "%04d/%02d/%02d",
		   1900+tmp->tm_year, tmp->tm_mday, tmp->tm_mon );

  std::cout << str;
}

void printTime( bool utc )
{
  struct timeval tv;
  gettimeofday( &tv, NULL );

  time_t epoch = tv.tv_sec;
  struct tm *tmp;

  if (utc) tmp = gmtime( &epoch );
  else     tmp = localtime( &epoch );

  char str[256];
  sprintf( str, "%02d:%02d:%02d.%1d",
		   tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (int)(tv.tv_usec/100000) );

  std::cout << str;
}

int64_t getMs()
{
  struct timeval tv;
  gettimeofday( &tv, NULL );

  int64_t ms = tv.tv_sec*1000;
  ms += tv.tv_usec/1000;

  return ms;
}

void printMs( int64_t start_time )
{
  int64_t now = getMs();

  printf( "%u", (unsigned)(now-start_time) );
}

