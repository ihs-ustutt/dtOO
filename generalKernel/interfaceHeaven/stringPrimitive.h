#ifndef STRINGPRIMITIVE_H
#define	STRINGPRIMITIVE_H

#include <string>
#include <vector>

#include <logMe/dtMacros.h>

namespace dtOO {
  class stringPrimitive {
  public:
    dt__CLASSNAME(stringPrimitive);     
    stringPrimitive();
    stringPrimitive(const stringPrimitive& orig);
    virtual ~stringPrimitive();
    static float stringToFloat(std::string const value);
    static std::string floatToString(float const value);
    static int stringToInt(std::string const value);
    static std::string intToString(int const value);
    static std::string intToStringLZ(int const value, int const nZeros);   
    static std::string getStringBetween(std::string const signStart, std::string const signEnd, std::string const str);
    static std::string getStringBetweenAndRemove(std::string const signStart, std::string const signEnd, std::string * const str);
    static bool stringContains(std::string const pattern, std::string const str);
    static std::string stringRemoveSingle(std::string const pattern, std::string const str);
  private:

  };
}
#endif	/* STRINGPRIMITIVE_H */

