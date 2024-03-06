/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef STRINGPRIMITIVE_H
#define	STRINGPRIMITIVE_H

#include <dtOOTypeDef.h>

#include <string>
#include <vector>
#include <map>
#include <logMe/dtMacros.h>

namespace dtOO {
  class stringPrimitive {
  public:
    dt__classOnlyName(stringPrimitive);          
    virtual ~stringPrimitive();
    static dtReal stringToFloat(std::string const value);
    static std::string floatToString(dtReal const value);
    static dtInt stringToInt(std::string const value);
    static std::string intToString(int const value);
    static std::string intToStringLZ(int const value, dtInt const nZeros);   
    static bool stringToBool(std::string const value);
    static std::string getStringBetween(
      std::string signStart, std::string signEnd, 
      std::string const str
    );
    static std::string getStringBetweenFirstLast(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );    
    static std::string getStringBetweenAndRemove(
      std::string const signStart, std::string const signEnd, 
      std::string * const str
    );
    static std::string getStringBetweenRespectOcc(
      std::string const signStart, 
      std::string const signEnd, 
      std::string const str
    );
    static std::string getStringBetweenAndRemoveRespectOcc(
      std::string const signStart, 
      std::string const signEnd, 
      std::string * const str
    );
    static bool stringContains(
      std::string const pattern, std::string const str
    );
    static std::string stringRemoveSingle(
      std::string const pattern, std::string const str
    );
	  static std::vector< std::string > convertToStringVector(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );
    static std::vector< std::string > convertToCSVStringVector( 
      std::string str 
    );    
    static std::string replaceOnceStringInString(
      std::string const toReplace, std::string const with, std::string const str
    );    
    static std::string replaceStringInString(
      std::string const toReplace, std::string const with, 
      std::string const str
    );
    static std::vector< std::string > crumbleDown(
      std::string const signStart, std::string const signEnd, 
      std::string const str    
    );
    static bool isWildcard( std::string const str );
    static bool isFullWildcard( std::string const str );
    static bool matchWildcard(
      std::string const & pWildStr, std::string const & pStringStr
    );    
  protected:
    stringPrimitive();       
  private:
    static std::pair< int, dtInt > getFromToBetween(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );
    static std::pair< int, dtInt > getFromToBetweenFirstLast(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );
    static std::pair< int, dtInt > getFromToBetweenRespectOcc(
      std::string const signStart, 
      std::string const signEnd, 
      std::string const str
    );
    static std::vector< dtInt > getOccurences(
      std::string const & pattern, std::string const & str, 
      dtInt from = 0, dtInt to = 0 
    );
    static std::map< int, dtInt > getOccurenceMap(
      std::string const & signStart, std::string const & signEnd, 
      std::string const & str
    );
    static std::string regex_escape( std::string text );
  protected:    
    static std::string _WILD;
    static std::string _WILDSIGN;    
  };
}
#endif	/* STRINGPRIMITIVE_H */
