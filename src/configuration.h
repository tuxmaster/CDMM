//======================================================================
// File:        configuration.h
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

#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <vector>
#include <string>
#include <sstream>

/** \brief Parse and hold configuration parameters from command line
           as well as from configuration files

    Parse commandline parameters with #setCommandLine. The parameter
    have to be in the GNU style short: -s or long: --long-parameter. The parameter are read from left to right.
    A key can only exist once. Duplicate keys override the 
    previously defined.
    This class also can read configuration files containing key,
    value pairs separated with '='. The files my also contain 
    comments. Lines beginning with '#' are treated as comments.
    Load files with #loadFile.

    Example:<p>
    <tt>##################################<br>
    # This is a comment<br>
    #<br>
    long-value = 2<br>
    string = "this is a string"<br></tt>
*/
class Configuration
{
public:
  Configuration();
  ~Configuration();
      
  /** \brief Description of a command line option
  */
  class Option
  {
  public:
    Option( const std::string & _shortOption, 
            const std::string & _longOption,
          bool _hasParameter, const std::string & _description ) :
      shortOption( _shortOption ),
      longOption( _longOption ),
      hasParameter( _hasParameter ),
      description( _description )
    {
    }
    /// Short name of option (one letter)
    std::string shortOption;
    /// Long name of option
    std::string longOption;
    /// True if option needs a value
    bool hasParameter;
    /// Descriptive string for help
    std::string description;
  };
        
  /** \brief Description of a given commandline option
  */
  class OptionEntry
  {
  public:
    OptionEntry( const std::string & _key, 
                  const std::string & _value ) :
      key( _key ),
      value( _value )
    {
    }
    /// The key (left hand)
    std::string key;
    /// The value (right hand)
    std::string value;
  };
  /** \brief Add an option to the internal list
  
      Only options added through this function will
      be accepted by readFile and setCommandLine.
  
      \param option Option to be added
  */
  void addOption( const Option & );
  /** \brief Add an option to the internal list
  
      Only options added through this function will
      be accepted by readFile and setCommandLine.
  
      \param option Option to be added
   */
  bool setCommandLine( int argc, char **argv );
  
  typedef std::vector<Option> OptionList;
  typedef std::vector<OptionEntry> EntryList;
  
  /** \brief Check if key is in the parsed options
      \param key Key to be checked
      \returns true if key is in the internal list
  */
  bool hasKey( const std::string & key ) const;
  /** \brief Returns string representation of correponding value
      \param key Key to be checked
      \returns String representation of the assigned value
  */
  std::string stringValue( const std::string & key ) const;
  /** \brief Returns correponding value as int
  \param key Key to be checked
  \returns Assigned value tranformed to int value
   */
  int intValue( const std::string & key ) const;
  /** \brief Returns correponding value as double
  \param key Key to be checked
  \returns Assigned value tranformed to int value
   */
  double doubleValue( const std::string & key ) const;
  /** \brief Load a file
  
      Load a configuration file containing key, value pairs
      (or single keys). The keys must have been added with
      addOption before.
  
      \param filename Filename of file to be loaded
      \returns true if file could be loaded
   */
  bool loadFile( const std::string & filename );
  /** \brief Set an additional help text
  */
  void setHelpText( const std::string & text ) { m_helpText = text; }
  /** \brief Return helptext containing all options
  */
  std::string helpText() const;

  /// Helper: Print commandline entries to stdout
  void printEntries() const;
  
protected:
  /// List of known options
  OptionList m_option;
  /// List of found options
  EntryList  m_entry;
  /// Additional help text
  std::string m_helpText;
  
  /** \brief Return index of given option
      \param optionText option (may be long or short representation)
      \returns index of option or -1 if not found
  */
  int findOption( const std::string & optionText ) const;
  /** \brief Return index of given short option
  \param optionText short option
  \returns index of option or -1 if not found
   */
  int findShort( const std::string & optionText ) const;
  /** \brief Return index of given long option
  \param optionText long option
  \returns index of option or -1 if not found
   */
  int findLong( const std::string & optionText ) const;
  /** \brief Return index in entries of given key
  \param key key
  \returns index of entry or -1 if not found
   */
  int findEntry( const std::string & key ) const;
  /** \brief Insert (or replace existing) key in entries
  \param key key
  \param value value
   */
  void insertEntry( const std::string & key, 
                    const std::string & value );
};

#endif // CONFIGURATION_HH
