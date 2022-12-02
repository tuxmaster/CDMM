//======================================================================
// File:        configuration.cpp
// Author:      Matthias Toussaint
// Created:     Sun Dec  3 10:31:50 CET 2006
// Project:     QtDMM
// Description: Commandline parsing and configuration file reading
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

#include <configuration.h>
#include <util.h>
#include <fstream>
#include <iostream>

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

void Configuration::addOption( const Option & option )
{
  m_option.push_back( option );
}

bool Configuration::setCommandLine( int argc, char **argv )
{
  if (argc<2) return false;
  
  int argIndex=1;
  bool waitForParameter=false;
  std::string key;
  
  do 
  {
    if (waitForParameter)
    {
      insertEntry( key, argv[argIndex] );
      waitForParameter = false;
    }
    else
    {
      TokenList tokenList = Util::tokenize( argv[argIndex], "=" );
      
      if (tokenList.size() == 1)
      {
        int index = findOption( tokenList[0] );
        
        if (-1 != index)
        {
          if (m_option[index].hasParameter) 
          {
            key = m_option[index].longOption;
            waitForParameter = true;
          }
          else
          {
            insertEntry( m_option[index].longOption, "" );
          }
        }
        else
        {
          std::cout << "Unknown parameter: " << tokenList[0] 
                    << std::endl;
          
          return false;
        }
      }
      else if (tokenList.size() == 2)
      {
        if (tokenList[0].substr( 0, 2 ) != "--")
        {
          std::cout << "Error processing commandline: " << tokenList[0] 
                    << std::endl;
          
          return false;
        }
        
        insertEntry( tokenList[0].substr( 2, tokenList[0].size()-2 ), tokenList[1] );
      }
      else 
      {
        std::cout << "Error processing commandline: " << tokenList[0] 
                  << std::endl;
        
        return false;
      }
    }
    
    ++argIndex;
    
  } while (argIndex<argc);
  
  return true;
}

void Configuration::insertEntry( const std::string & key, 
                                 const std::string & value )
{
  int index = findEntry( key );
  if (-1 == index)
  {
    m_entry.push_back( OptionEntry( key, value ));
  }
  else
  {
    m_entry[index] = OptionEntry( key, value );
  }
}

int Configuration::findOption( const std::string & optionText ) const
{
  int index = findShort( optionText );
  if (-1 == index) index = findLong( optionText );
  
  return index;
}

int Configuration::findShort( const std::string & tst ) const
{
  int i=0;
  while (tst[i] == '-') ++i;
  std::string shortOption = tst.substr( i, tst.size()-i );
  
  i=0;
  
  for (OptionList::const_iterator it = m_option.begin(); 
       it != m_option.end(); ++it, ++i)
  {
    if ((*it).shortOption == shortOption) return i;
  }
  
  return -1;
}

int Configuration::findLong( const std::string & tst ) const
{
  int i=0;
  while (tst[i] == '-') ++i;
  std::string longOption = tst.substr( i, tst.size()-i );
  
  i=0;
  
  for (OptionList::const_iterator it = m_option.begin(); 
       it != m_option.end(); ++it, ++i)
  {
    if ((*it).longOption == longOption) return i;
  }
  
  return -1;
}

int Configuration::findEntry( const std::string & key ) const
{
  int index=0;
  for (EntryList::const_iterator it = m_entry.begin(); 
       it != m_entry.end(); ++it, ++index)
  {
    if ((*it).key == key) return index;
  }
  
  return -1;
}

bool Configuration::hasKey( const std::string & key ) const
{
  for (EntryList::const_iterator it = m_entry.begin(); 
       it != m_entry.end(); ++it)
  {
    if ((*it).key == key) return true;
  }
  
  return false;
}

std::string Configuration::stringValue( const std::string & key ) const
{
  for (EntryList::const_iterator it = m_entry.begin(); 
       it != m_entry.end(); ++it)
  {
    if ((*it).key == key)
    {
      return (*it).value;
    }
  }
  
  return std::string();
}

int Configuration::intValue( const std::string & key ) const
{
  int intValue;
  Util::fromString( &intValue, stringValue( key ) );
  
  return intValue;
}

double Configuration::doubleValue( const std::string & key ) const
{
  double doubleValue;
  Util::fromString( &doubleValue, stringValue( key ) );
  
  return doubleValue;
}

bool Configuration::loadFile( const std::string & filename )
{
  // dear c++ gurus. why the heck do I need c_str here?
  std::ifstream is( filename.c_str(), std::ios_base::in );
  
  if (!is)
  {
    std::cerr << "Couldn't open file: " << filename << std::endl;
    return false;
  }
  std::string line;
  
  while (!is.eof())
  {
    std::getline( is, line );
    line = Util::strip_whitespace( line );
    
    if (line.size())
    {
      if (line[0] != '#')
      {
        TokenList tokenList = Util::tokenize( line, "=" );
        
        int index = findOption( Util::strip_whitespace( tokenList[0] ));
        
        if (-1 != index)
        {
          if (m_option[index].hasParameter)
          {
            if (tokenList.size() == 2)
            {
              insertEntry( m_option[index].longOption, 
                           Util::strip_whitespace( tokenList[1] ));
            }
            else if (tokenList.size() > 2)
            {
              std::cerr << "Too many values in line: " << line << std::endl;
              return false;
            }
            else 
            {
              std::cerr << "Missing value in line: " << line << std::endl;
              return false;
            }
          }
          else
          {
            insertEntry( m_option[index].longOption, "" );
          }
        }
      }
    }
  }
    
  return true;
}

void Configuration::printEntries() const
{
  for (EntryList::const_iterator it = m_entry.begin(); 
       it != m_entry.end(); ++it)
  {
    if ((*it).value.size())
    {
      std::cout << (*it).key << " = " 
          << (*it).value << std::endl;
    }
    else
    {
      std::cout << (*it).key << std::endl;
    }
  }
}

std::string Configuration::helpText() const
{
  std::string help;
  help = "Syntax: -s[ value] or --long[=value]\n";
  help += "The following commandline options can be used\n";
  
  for (OptionList::const_iterator it = m_option.begin(); 
       it != m_option.end(); ++it)
  {
    if ((*it).shortOption.size())
    {
      help += "-";
      help += (*it).shortOption;
    }
    if ((*it).longOption.size())
    {
      if ((*it).shortOption.size()) help += "|";
      help += "--";
      help += (*it).longOption;
    }
    help += "\n    ";
    if (!(*it).hasParameter)
    {
      help += "Flag: ";
    }
    help += (*it).description;
    help += "\n";
  }
  
  help += m_helpText;
  
  return help;
}
