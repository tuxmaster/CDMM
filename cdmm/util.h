//======================================================================
// File:        util.h
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

#ifndef UTIL_HH
#define UTIL_HH

#include <string>
#include <vector>

typedef std::vector<std::string> TokenList;

/** \brief Genaral untility functions
*/
class Util
{
public:
  
  static std::string strip_whitespace( const std::string & str );
  static TokenList tokenize( const std::string & str, 
                             const std::string & separators );
  static void split_MEAS( const std::string & meas, std::string & value, std::string & prefix );
  
  /** Helper function
  \param value Any value that can be streamed by stringstream
  \param str The string to be converted
  \returns true on success
   */
  template <typename T>
  static bool fromString( T * value, const std::string & str )
  {
    std::stringstream ss( str );
    return ss >> *value;
  }

};

#if 0
std::vector<std::string> &
split( const std::string &s, char delim, std::vector<std::string> &elems ) 
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) 
  {
      elems.push_back(item);
  }
  return elems;
}
std::vector<std::string> 
split( const std::string &s, char delim ) 
{
  std::vector<std::string> elems;
  
  return split(s, delim, elems);
}
#endif

#endif // UTIL_HH
