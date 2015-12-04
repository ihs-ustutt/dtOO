#include "eGeneral.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>

namespace dtOO {
  eGeneral::eGeneral() _GLIBCXX_USE_NOEXCEPT : std::exception() {
  }

  eGeneral::eGeneral( std::ostream & msg) _GLIBCXX_USE_NOEXCEPT : std::exception() {
		std::ostringstream& s = dynamic_cast<std::ostringstream&>(msg);
		_exceptionMsg = s.str();
	}
		
  eGeneral::~eGeneral() _GLIBCXX_USE_NOEXCEPT {
  }

  void eGeneral::clear(void) {
		_exceptionMsg.clear();
		std::cout << "_exceptionMsg = " << _exceptionMsg << std::endl;
  }
  
  const char* eGeneral::what( void ) const _GLIBCXX_USE_NOEXCEPT {
    return _exceptionMsg.c_str();
  }
  
  const char* eGeneral::where( void ) const _GLIBCXX_USE_NOEXCEPT {
    if ( 
      ( _exceptionMsg.find_first_of("[") != std::string::npos )
      &&
      ( _exceptionMsg.find_first_of("]") != std::string::npos )
    ) {
      return _exceptionMsg.substr(
        _exceptionMsg.find_first_of("[")+1, 
        _exceptionMsg.find_first_of("]") - _exceptionMsg.find_first_of("[") - 1
      ).c_str();
    }
    else {
      return std::string("").c_str();
    }
  }  
}
