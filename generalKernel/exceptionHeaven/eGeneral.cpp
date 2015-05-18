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
}
