#include "dtOMField.h"

namespace dtOO {
  dtOMField::dtOMField( std::string const & label, dtOMMesh const & om ) 
    : labelHandling(label), _om(om) {
  }
  
  dtOMField::~dtOMField() {
  }
  
  dtOMMesh const & dtOMField::refMesh( void ) const {
    return _om;
  }
}
