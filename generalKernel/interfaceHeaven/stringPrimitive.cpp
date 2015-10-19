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

  bool stringPrimitive::stringContains(
	  std::string const pattern, std::string const str
	) {
    return (str.find(pattern.c_str()) != std::string::npos);
  }
  
  std::string stringPrimitive::stringRemoveSingle(
	  std::string const pattern, std::string const str
	) {
    if (pattern.size() != 1) {
      dt__throw(
				stringRemoveSingle(),
        << "Only possible to remove one single char." << std::endl
        << dt__eval(pattern) << std::endl
        << dt__eval(pattern.size()) 
			);
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
    std::pair< int, int > fromTo = getFromToBetween(signStart, signEnd, str);
		
    if ( (fromTo.first == 0) && (fromTo.second == 0) ) return std::string("");
		
    return str.substr( fromTo.first+1, fromTo.second-fromTo.first-1);
  }

  std::string stringPrimitive::getStringBetweenFirstLast(
	  std::string const signStart, 
		std::string const signEnd, 
		std::string const str
	) {  
    std::pair< int, int > fromTo 
    = 
    getFromToBetweenFirstLast(signStart, signEnd, str);
		
    if ( (fromTo.first == 0) && (fromTo.second == 0) ) return std::string("");
		
    return str.substr( fromTo.first+1, fromTo.second-fromTo.first-1);
  }  

  std::string stringPrimitive::getStringBetweenAndRemove(
	  std::string const signStart, std::string const signEnd, 
		std::string * const str
	) {
	  //
		// get return string
		//
    std::string retStr = getStringBetween(signStart, signEnd, *str);
		
		//
		// get indices and remove return string with start and end sign
		//
    std::pair< int, int > fromTo = getFromToBetween(signStart, signEnd, *str);
		if ( (fromTo.first == 0) && (fromTo.second == 0) ) return retStr;
		
    //
    // adjust from to 0 if first signStart is empty
    //
    int from = std::max(0, fromTo.first);
    int to = fromTo.second;    
    int nChars = to - from + 1;
    
    dt__throwIfWithMessage(
      to==(str->length()+1), 
      getStringBetweenAndRemove(),
      << "Case not yet implemented."
    );
    
    //
    // remove substring with signStart and signEnd
    //
		str->erase(from, nChars);
    
    return retStr;
  }
  
	std::vector< std::string > stringPrimitive::convertToStringVector(
	  std::string const signStart, std::string const signEnd, 
		std::string const str
	) {
		std::vector< std::string > values;
		std::string valueStr = str;
		for (int ii=0; ii<str.length(); ii++) {
			std::string aVal = getStringBetweenAndRemove(signStart, signEnd, &valueStr);
      if ( stringContains(":replace:", aVal) ) {
        std::string replaceRule = aVal;
        aVal = getStringBetweenAndRemove(signStart, signEnd, &replaceRule);
        std::vector< std::string > replaceVec 
        = 
        convertToStringVector(":", ":", replaceRule);
        
        dt__info(
          convertToStringVector(), 
          << dt__eval(aVal) << std::endl
          << dt__eval(replaceRule) << std::endl
          << dt__eval(replaceVec)
        );
        
        for (int ii=2; ii<replaceVec.size(); ii++) {
          values.push_back( 
            replaceStringInString(replaceVec[1], replaceVec[ii], aVal) 
          );
        }
      }
      else values.push_back(aVal);
      
      if (
        valueStr.length() == 0 
        || 
        !stringContains(signStart, valueStr) 
        ||
        !stringContains(signEnd, valueStr) 
      ) break;      
		}

		return values;
	}

  std::vector< std::string > stringPrimitive::convertToCSVStringVector(
    std::string str
  ) {
    std::vector< std::string > retStr;    
    
    //
    // get all inner elements
    //
    do {
      retStr.push_back( getStringBetweenAndRemove("", ",", &str) );
    }
    while ( stringContains(",", str) );
    
    //
    // last element
    //
    retStr.push_back(str);
    
    return retStr;
  }
  
  std::string stringPrimitive::replaceStringInString(
	  std::string const toReplace, std::string const with, std::string const str
	) {
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
	
  std::pair< int, int > stringPrimitive::getFromToBetween(
	  std::string const signStart, std::string const signEnd, std::string const str
	) {

    //
    // return empty string if signStart and signEnd are not part of the string
    //
    if ( 
      !stringContains(signStart, str) 
      || 
      !stringContains(signEnd, str) 
    ) return std::pair< int, int >(0, 0);

    int from;
    if (!signStart.empty()) {
      from = str.find_first_of(signStart.c_str()) + signStart.size()-1;
    }
    else from = -1;
    int to;
    if (!signEnd.empty()) {
      to = str.find_first_of(signEnd.c_str(), from+1) + signEnd.size()-1;
    }
    else to = str.length()+1;

    //
    // check if there is one more occurrence of signStart between from and to
    //
    if (
      (signStart != signEnd)
      &&
      (str.find_first_of(signStart.c_str(), from+1) < to)
    ) {
      //
      // get occurrences of signStart and signEnd
      //
      std::vector< int > ocSignStart 
      = 
      getOccurences(signStart, str, from+1, to-1);
      std::vector< int > ocSignEnd 
      = 
      getOccurences(signEnd, str, to+1);      
      
      //
      // adjust to
      //
      to = ocSignEnd[ocSignStart.size()-1];
    }
    
    return std::pair< int, int >(from, to);		
	}
  
  std::pair< int, int > stringPrimitive::getFromToBetweenFirstLast(
	  std::string const signStart, std::string const signEnd, std::string const str
	) {
    if ( (signStart.size() > 1) || (signEnd.size() > 1) ) {
      dt__throw(
				getStringFromTo(),
        << dt__eval(signStart.size()) << std::endl
        << dt__eval(signEnd.size()) << std::endl
        << "Signs should have size equal one."
			);
    }

    if ( !stringContains(signStart, str) || !stringContains(signEnd, str) ) {
      return std::pair< int, int >(0, 0);
    }

    int from;
    if ( (signStart.size() == 1) ) {
      from = str.find_first_of(signStart.c_str());
    }
    else from = -1;
    int to;
    if ( (signEnd.size() == 1) ) {
      to = str.find_last_of(signEnd.c_str());
    }
    else to = str.length()+1;

    return std::pair< int, int >(from, to);		
	}
  
  std::vector< int > stringPrimitive::getOccurences(
    std::string const & pattern, std::string const & str, int from, int to
  ) {
    int aMatch = from;
    if (to==0) to = str.size();
    
    std::vector< int > matches(0);
    while (true) {
      aMatch = str.find_first_of(pattern, aMatch);
      
      //
      // if no more match return vector
      //
      if ( (aMatch == std::string::npos) || (aMatch>to) ) return matches;
      
      //
      // store position and increment
      //
      matches.push_back(aMatch);
      aMatch++;
    }
  }		
}