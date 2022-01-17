#include "stringPrimitive.h"
#include "progHelper.h"
#include <sstream>
#include <istream>
#include <ostream>
#include <logMe/logMe.h>
#include <iomanip>

#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>

namespace dtOO {
  std::string stringPrimitive::_WILD = "*";
  std::string stringPrimitive::_WILDSIGN = "?";
  
  stringPrimitive::stringPrimitive() {
  }

  stringPrimitive::~stringPrimitive() {
  }

  dtReal stringPrimitive::stringToFloat(std::string const value) {
    dtReal argumentFloat;
    std::istringstream(value) >> argumentFloat;
    return argumentFloat;
  }

  std::string stringPrimitive::floatToString(dtReal const value) {
    std::ostringstream buff;
    buff << std::setprecision(8) << std::fixed << value;
    return buff.str();
  }

  int stringPrimitive::stringToInt(std::string const value) {
    dtReal argumentFloat;
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

  bool stringPrimitive::stringToBool(std::string const value) {
    if ( 
      (value == "true") || (value=="1") || (value=="True")
    ) return true;
    if ( 
      (value == "false") || (value=="0") || (value=="False") 
    ) return false;
		dt__warning(
			stringToBool(),
      << "Value of " << dt__eval(value) << std::endl
      << "Should be true, 1, false, 0 or empty."
		);
    return false;
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
	  std::string signStart, 
		std::string signEnd, 
		std::string const str
	) {
    if ( signStart.empty() ) signStart = "^";
    else signStart = regex_escape(signStart);
    
    if ( signEnd.empty() ) signEnd = "$";
    else signEnd = regex_escape(signEnd);
    
    ::boost::smatch what;
    if (
      ::boost::regex_search(
        str,
        what,
        ::boost::regex(signStart + "(.*?)" + signEnd)
      )
    ) {
      if (what.size() == 2) return what[1].str();
    }
    return std::string("");
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
  
  std::string stringPrimitive::getStringBetweenRespectOcc(
	  std::string const signStart, 
		std::string const signEnd, 
		std::string const str
	) {  
    int pos = str.find_first_of( signStart.c_str() );
    dt__throwIf(pos==std::string::npos, getStringBetweenRespectOcc());
    
    std::map< int, int > occMap = getOccurenceMap(signStart, signEnd, str);
		
    dt__throwIf(occMap.find(pos)==occMap.end(), getStringBetweenRespectOcc());
    
    return str.substr( pos+1, occMap[pos]-pos-1);
  }  
  
  std::string stringPrimitive::getStringBetweenAndRemoveRespectOcc(
	  std::string const signStart, 
		std::string const signEnd, 
		std::string * const str
	) {  
    std::string strBetween 
    = 
    getStringBetweenRespectOcc(signStart, signEnd, *str);
    
    *str 
    = 
    replaceOnceStringInString(signStart + strBetween + signEnd, "", *str);
    return strBetween;
  }
  
	std::vector< std::string > stringPrimitive::convertToStringVector(
	  std::string const signStart, std::string const signEnd, 
		std::string const str
	) {
		std::vector< std::string > values;
		std::string valueStr = str;
		for (int ii=0; ii<str.length(); ii++) {
			std::string aVal 
      = 
      getStringBetweenAndRemoveRespectOcc(signStart, signEnd, &valueStr);

      if ( stringContains(":replace:", aVal) ) {
        std::string replaceRule = aVal;
        aVal 
        = 
        getStringBetweenAndRemoveRespectOcc(signStart, signEnd, &replaceRule);
        std::vector< std::string > replaceVec 
        = 
        convertToStringVector(":", ":", replaceRule);
        
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
    if ( !stringContains(",", str) ) return std::vector< std::string >(1, str);
      
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
  
  std::string stringPrimitive::replaceOnceStringInString(
	  std::string const toReplace, std::string const with, std::string const str
	) {
		if ( !stringContains(toReplace, str) ) return str;

    std::string retStr = str;		
		retStr.replace(
		  retStr.find(toReplace), 
			toReplace.length(), 
			with
	  );

    return retStr;
  }
  
  std::string stringPrimitive::replaceStringInString(
	  std::string const toReplace, std::string const with, std::string const str
	) {
		std::string retStr = replaceOnceStringInString(toReplace, with, str);
		
		if ( stringContains(toReplace, retStr) ) {
			retStr = replaceStringInString(toReplace, with, retStr);
		}

    return retStr;
  }	
  
  std::vector< std::string > stringPrimitive::crumbleDown(
    std::string const signStart, std::string const signEnd, 
    std::string const str    
  ) {
    std::vector< std::string > ret;
    
    int counter = 0;
    while ( counter < str.length() ) {
      std::pair< int, int > fromTo 
      = 
      getFromToBetweenRespectOcc(signStart, signEnd, str.substr(counter));
      
      if ( (fromTo.first == 0) && (fromTo.second == 0) ) break;
      
      fromTo.first = fromTo.first + counter;
      fromTo.second = fromTo.second + counter;
      counter = fromTo.second+1;
      
      ret.push_back(
        str.substr(
          fromTo.first+1,  fromTo.second-fromTo.first-1
        )
      );
    }
    
//    dt__debug(
//      crumbleDown(),
//      << "str = " << str << std::endl
//      << ret
//    );
    
    return ret;
  }
      
  bool stringPrimitive::isWildcard( std::string const str ) {
    return (
      ( str.find(stringPrimitive::_WILD) != std::string::npos )
      ||
      ( str.find(stringPrimitive::_WILDSIGN) != std::string::npos )
    );
  }

  bool stringPrimitive::isFullWildcard( std::string const str ) {
    return ( str == stringPrimitive::_WILD );
  }  
  
  bool stringPrimitive::matchWildcard(
    std::string const & pWildStr, std::string const & pStringStr
  ) {
    const char * pWild = pWildStr.c_str();
    const char * pString = pStringStr.c_str();
      
    int ii, star;

    new_segment:

       star = 0;
       while (pWild[0] == '*')
       {
          star = 1;
          pWild++;
       }

    test_match:

       for (ii = 0; pWild[ii] && (pWild[ii] != '*'); ii++)
       {
          if (pWild[ii] != pString[ii])
          {
             if (! pString[ii]) return 0;
             if (pWild[ii] == '?') continue;
             if (! star) return 0;
             pString++;
             goto test_match;
          }
       }

       if (pWild[ii] == '*')
       {
          pString += ii;
          pWild += ii;
          goto new_segment;
       }

       if (! pString[ii]) return true;
       if (ii && pWild[ii-1] == '*') return true;
       if (! star) return false;
       pString++;
       goto test_match;
  }
  
  std::pair< int, int > stringPrimitive::getFromToBetween(
	  std::string const signStart, std::string const signEnd, 
    std::string const str
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
    
    return 
      std::pair< int, int >(from, to);		
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
  
  std::pair< int, int > stringPrimitive::getFromToBetweenRespectOcc(
	  std::string const signStart, 
		std::string const signEnd, 
		std::string const str
	) {  
    if ( !stringContains(signStart, str) || !stringContains(signEnd, str) ) {
      return std::pair< int, int >(0, 0);
    }
    
    int pos = str.find_first_of( signStart.c_str() );
    dt__throwIf(pos==std::string::npos, getStringBetweenRespectOcc());
    
    std::map< int, int > occMap = getOccurenceMap(signStart, signEnd, str);
		
    dt__throwIf(occMap.find(pos)==occMap.end(), getStringBetweenRespectOcc());
    
    return std::pair< int, int >(pos, occMap[pos]);
    //return str.substr( pos+1, occMap[pos]-pos-1);
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
  
  std::map< int, int > stringPrimitive::getOccurenceMap(
    std::string const & signStart, std::string const & signEnd, 
    std::string const & str
  ) {
    std::vector< int > matchStart = getOccurences(signStart, str);
    std::vector< int > matchEnd = getOccurences(signEnd, str);

    dt__throwIfWithMessage(
      matchStart.size() != matchEnd.size(), 
      getOccurenceMap(),
      << dt__eval(signStart) << std::endl
      << dt__eval(signEnd) << std::endl
      << dt__eval(str) 
    );
    
    progHelper::reverse(matchStart);
    
    std::map< int, int > occMap;
    dt__forAllIndex(matchStart, ii) {
      std::vector< int > dist(matchEnd.size(), 0);
      dt__forAllIndex(matchEnd, jj) {
        dist[jj] = matchEnd[jj] - matchStart[ii];
        if (dist[jj] <= 0) dist[jj] = std::numeric_limits<int>::max();
      }
      std::vector< int >::iterator minIt 
      = 
      std::min_element( dist.begin(), dist.end() );
      occMap[ matchStart[ii] ] 
      = 
      matchEnd[ std::distance(dist.begin(), minIt) ];
      matchEnd.erase( matchEnd.begin() + std::distance(dist.begin(), minIt) );
    }
    return occMap;    
  }  
  
  std::string stringPrimitive::regex_escape( std::string text ) {
    const ::boost::xpressive::sregex re_escape_text 
    = 
    ::boost::xpressive::sregex::compile(
      "([\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\])"
    );
    text 
    = 
    ::boost::xpressive::regex_replace( 
      text, re_escape_text, std::string("\\$1") 
    );
    return text;
  }
}
