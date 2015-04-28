#include "stringPrimitive.h"
#include <sstream>
#include <istream>
#include <ostream>
#include <logMe/logMe.h>
#include <iomanip>

namespace dtOO { 
  stringPrimitive::stringPrimitive() {
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
      dt__throw(stringRemoveSingle(),
              << "Only possible to remove one single char." << std::endl
              << dt__eval(pattern) << std::endl
              << dt__eval( pattern.size() ) );
    }
    if ( !stringContains(pattern, str) ) { //|| !stringContains(pattern, str) ) {
      return std::string("");
    }    
    int pos = str.find(pattern.c_str());
    
    std::string retStr = str;
    
    return retStr.erase( pos );
  }

  std::string stringPrimitive::getStringBetween(
	  std::string const signStart, 
		std::string const signEnd, 
		std::string const str
	) {  
    if ( (signStart.size() > 1) || (signEnd.size() > 1) ) {
      dt__throw(
				getStringBetween(),
        << dt__eval(signStart.size()) << std::endl
        << dt__eval(signEnd.size()) << std::endl
        << "Signs should have size equal one."
			);
    }

    if ( !stringContains(signStart, str) || !stringContains(signEnd, str) ) {
//      dt__debug(getStringBetween(),
//              << "No string between " << dt__eval(signStart) << " "
//              << dt__eval(signEnd) << " in " << dt__eval(str) );
      return std::string("");
    }

    int from;
    if ( (signStart.size() == 1) ) {
      from = str.find_first_of(signStart.c_str());
    }
    else from = -1;
    int to;
    if ( (signEnd.size() == 1) ) {
      to = str.find_first_of(signEnd.c_str(), from+1);
    }
    else to = -1;

//    dt__debug(getStringBetween(),
//            << dt__eval(str) << std::endl
//            << dt__eval(signStart) << std::endl
//            << dt__eval(signEnd) << std::endl
//            << dt__eval(from) << std::endl
//            << dt__eval(to) << std::endl
//            << dt__eval(str.substr( from+1, to-from-1)) );

    return str.substr( from+1, to-from-1);
  }

  std::string stringPrimitive::getStringBetweenAndRemove(std::string const signStart, std::string const signEnd, std::string * const str) {
    std::string retStr = getStringBetween(signStart, signEnd, *str);

    //if (retStr != "") {
      unsigned int from = str->find_first_of(signStart.c_str());
      unsigned int to = str->find_first_of(signEnd.c_str(), from+1);

//      dt__debug(getStringBetweenAndRemove(),
//              << dt__eval(*str) << std::endl
//              << dt__eval(signStart) << std::endl
//              << dt__eval(signEnd) << std::endl
//              << dt__eval(from) << std::endl
//              << dt__eval(to) << std::endl
//              << dt__eval(str->erase(from, to-from+1)) );

    if ( stringContains(signStart, *str) && stringContains(signEnd, *str) ) {
      str->erase(from, to-from+1);
    }

    return retStr;
  }
	
	std::vector< std::string > stringPrimitive::convertToStringVector(std::string const signStart, std::string const signEnd, std::string const str) {
		std::vector< std::string > values;
		std::string valueStr = str;
		while (valueStr.length() != 0) {
			std::string aVal = getStringBetweenAndRemove(signStart, signEnd, &valueStr);
//			if (aVal.length() != 0 ) {
				values.push_back(aVal);
//				dt__info( 
//					openFileAndParse(), 
//					<< dt__eval(values.back()) << std::endl
//				);
//			}
//			else dt__THROW(convertToStringVector(), << "Error in creating the array.");
		}

		return values;
	}

  std::string stringPrimitive::replaceStringInString(std::string const toReplace, std::string const with, std::string const str) {
		if ( !stringContains(toReplace, str) ) return str;

    std::string retStr = str;		
		retStr.replace(
		  retStr.find(toReplace), 
			toReplace.length(), 
			with
	  );
		
		if ( stringContains(toReplace, retStr) ) {
			retStr = replaceStringInString(toReplace, with, retStr);
		}

    return retStr;
  }	
	
}