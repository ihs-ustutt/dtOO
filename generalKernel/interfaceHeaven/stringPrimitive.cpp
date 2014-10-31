#include "stringPrimitive.h"
#include <sstream>
#include <istream>
#include <ostream>
#include <logMe/logMe.h>
#include <iomanip>

namespace dtOO { 
  stringPrimitive::stringPrimitive() {
  }

  stringPrimitive::stringPrimitive(const stringPrimitive& orig) {
  }

  stringPrimitive::~stringPrimitive() {
  }

  float stringPrimitive::stringToFloat(std::string const value) {
    float argumentFloat;
    std::istringstream(value) >> argumentFloat;
    return argumentFloat;
  }

  std::string stringPrimitive::floatToString(float const value) {
    std::ostringstream buff;
    buff << value;
    return buff.str();
  }

  int stringPrimitive::stringToInt(std::string const value) {
    float argumentFloat;
    std::istringstream(value) >> argumentFloat;
    return argumentFloat;
  }

  std::string stringPrimitive::intToString(int const value) {
    std::ostringstream buff;
    buff << value;
    return buff.str();
  }

  std::string stringPrimitive::intToStringLZ(int const value, int const nZeros) {
    std::ostringstream buff;
    buff << std::setfill('0') << std::setw(nZeros) << value;;
    return buff.str();
  } 

  bool stringPrimitive::stringContains(std::string const pattern, std::string const str) {
    return (str.find(pattern.c_str()) != std::string::npos);
  }
  
  std::string stringPrimitive::stringRemoveSingle(std::string const pattern, std::string const str) {
    if (pattern.size() != 1) {
      dt__THROW(stringRemoveSingle(),
              << "Only possible to remove one single char." << LOGDEL
              << DTLOGEVAL(pattern) << LOGDEL
              << DTLOGEVAL( pattern.size() ) );
    }
    if ( !stringContains(pattern, str) || !stringContains(pattern, str) ) {
      return std::string("");
    }    
    int pos = str.find(pattern.c_str());
    
    std::string retStr = str;
    
    return retStr.erase( pos );
  }

  std::string stringPrimitive::getStringBetween(std::string const signStart, std::string const signEnd, std::string const str) {  
    if ( (signStart.size() > 1) || (signEnd.size() > 1) ) {
      dt__THROW(getStringBetween(),
              << DTLOGEVAL(signStart.size()) << LOGDEL
              << DTLOGEVAL(signEnd.size()) << LOGDEL
              << "Signs should have size equal one.");
    }

    if ( !stringContains(signStart, str) || !stringContains(signEnd, str) ) {
//      DTDEBUGWF(getStringBetween(),
//              << "No string between " << DTLOGEVAL(signStart) << " "
//              << DTLOGEVAL(signEnd) << " in " << DTLOGEVAL(str) );
      return std::string("");
    }

    int from;
    if ( (signStart.size() == 1) ) {
      from = str.find(signStart.c_str());
    }
    else {
      from = -1;
    }
    int to;
    if ( (signEnd.size() == 1) ) {
      to = str.find(signEnd.c_str());
    }
    else {
      to = -1;
    }

//    DTDEBUGWF(getStringBetween(),
//            << DTLOGEVAL(str) << LOGDEL
//            << DTLOGEVAL(signStart) << LOGDEL
//            << DTLOGEVAL(signEnd) << LOGDEL
//            << DTLOGEVAL(from) << LOGDEL
//            << DTLOGEVAL(to) << LOGDEL
//            << DTLOGEVAL(str.substr( from+1, to-from-1)) );

    return str.substr( from+1, to-from-1);
  }

  std::string stringPrimitive::getStringBetweenAndRemove(std::string const signStart, std::string const signEnd, std::string * const str) {
    std::string retStr = getStringBetween(signStart, signEnd, *str);

    //if (retStr != "") {
      unsigned int from = str->find(signStart.c_str());
      unsigned int to = str->find(signEnd.c_str());

//      DTDEBUGWF(getStringBetweenAndRemove(),
//              << DTLOGEVAL(*str) << LOGDEL
//              << DTLOGEVAL(signStart) << LOGDEL
//              << DTLOGEVAL(signEnd) << LOGDEL
//              << DTLOGEVAL(from) << LOGDEL
//              << DTLOGEVAL(to) << LOGDEL
//              << DTLOGEVAL(str->erase(from, to-from+1)) );

      str->erase(from, to-from+1);
    //}

    return retStr;
  }  
}