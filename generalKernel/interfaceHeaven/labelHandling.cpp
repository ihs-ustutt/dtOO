#include "labelHandling.h"

#include <logMe/logMe.h>

namespace dtOO {  
  labelHandling::labelHandling() {
    std::ostringstream os;
    os << std::hex << static_cast<void*>(this);
    _label = os.str();
  }

  labelHandling::labelHandling(const labelHandling& orig) {
    _label = orig._label;
  }

  labelHandling::labelHandling(std::string const & label) {
	  _label = label;	
	}
	
  labelHandling::~labelHandling() {
    _label.clear();
  }
  
  void labelHandling::setLabel( std::string const label) {
    _label = label;
  }
  
  void labelHandling::setLabel( char const * const label ) {
    _label = std::string(label);
  }
  
  std::string labelHandling::getLabel( void ) const {
    return _label;
  }
}
