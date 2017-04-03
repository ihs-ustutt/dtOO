#ifndef STRINGPRIMITIVE_H
#define	STRINGPRIMITIVE_H

#include <string>
#include <vector>
#include <map>
#include <logMe/dtMacros.h>

namespace dtOO {
  class stringPrimitive {
  public:
    dt__classOnlyName(stringPrimitive);          
    virtual ~stringPrimitive();
    static float stringToFloat(std::string const value);
    static std::string floatToString(float const value);
    static int stringToInt(std::string const value);
    static std::string intToString(int const value);
    static std::string intToStringLZ(int const value, int const nZeros);   
    static std::string getStringBetween(
      std::string const signStart, std::string const signEnd, 
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
    static std::string replaceStringInString(
      std::string const toReplace, std::string const with, 
      std::string const str
    );
    static std::vector< std::string > crumbleDown(
      std::string const signStart, std::string const signEnd, 
      std::string const str    
    );
    static bool isWildcard( std::string const str );
    static bool matchWildcard(
      std::string const & pWildStr, std::string const & pStringStr
    );    
  protected:
    stringPrimitive();       
  private:
    static std::pair< int, int > getFromToBetween(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );
    static std::pair< int, int > getFromToBetweenFirstLast(
      std::string const signStart, std::string const signEnd, 
      std::string const str
    );
    static std::pair< int, int > getFromToBetweenRespectOcc(
      std::string const signStart, 
      std::string const signEnd, 
      std::string const str
    );
    static std::vector< int > getOccurences(
      std::string const & pattern, std::string const & str, 
      int from = 0, int to = 0 
    );
    static std::map< int, int > getOccurenceMap(
      std::string const & signStart, std::string const & signEnd, 
      std::string const & str
    );
  protected:    
    static std::string _WILD;
    static std::string _WILDSIGN;    
  };
}
#endif	/* STRINGPRIMITIVE_H */

