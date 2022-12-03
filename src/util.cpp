//======================================================================
// File:        util.cpp
// Author:      Matthias Toussaint
// Created:     Sun Dec  3 16:27:25 CET 2006
// Project:     QtDMM
// Description: Utility class
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

#include "util.h"

TokenList Util::tokenize( const std::string & line,
						  const std::string & separators )

{
  TokenList tokenList;
  std::string token;

  for (unsigned i=0; i<line.size(); ++i)
  {
	unsigned s;
	for (s=0; s<separators.size(); ++s)
	{
	  if (line[i] == separators[s])
	  {
		tokenList.push_back( token );
		token = "";
		break;
	  }
	}

	if (s>=separators.size()) token += line[i];
  }
  if (token.size()) tokenList.push_back( token );

  return tokenList;
}

std::string
Util::strip_whitespace( const std::string & str )
{
  std::string retVal;

  int start;
  for (start=0; start<(int)str.size(); ++start)
  {
	if (str[start] != ' '  && str[start] != '\t' &&
		str[start] != '\r' && str[start] != '\n' &&
		str[start] != '\"')
	{
	  break;
	}
  }

  int stop;
  for (stop=(int)str.size()-1; stop>0; --stop)
  {
	if (str[stop] != ' '  && str[stop] != '\t' &&
		str[stop] != '\r' && str[stop] != '\n' &&
		str[stop] != '\"')
	{
	  break;
	}
  }

  return str.substr( start, stop-start+1 );
}

void
Util::split_MEAS( const std::string & meas, std::string & value, std::string & prefix )
{
  // copy number
  unsigned i=0;
  value = "";
  while (meas[i] != 'E' && i<meas.size())
  {
	if (meas[i] != '+') value += meas[i];
	++i;
  }
  // find prefix
  std::string unit = meas.substr(i);
  prefix = "";
  if (unit == "E-3")      prefix = "m";
  else if (unit == "E-6") prefix = "u";
  else if (unit == "E-9") prefix = "n";
  else if (unit == "E+3") prefix = "k";
  else if (unit == "E+6") prefix = "M";
}
