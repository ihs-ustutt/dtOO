#include "muParserFunctions.h"

#include <logMe/logMe.h>

namespace dtOO {
  dt__pH(muParserFunctions) muParserFunctions::_pH(NULL);
  
  muParserFunctions::muParserFunctions() {
  }

  muParserFunctions * muParserFunctions::instance( void ) {
    if (_pH.isNull()) _pH.reset( new muParserFunctions() );
    return _pH.get();
  }
  
  muParserFunctions::~muParserFunctions() {
  }
  
  mu::value_type muParserFunctions::oneInRange(
    mu::value_type low, mu::value_type high, mu::value_type value
  ) {
    if ( (value >= low) || (value < high) ) return 1.;
    
    return 0.;
  }  
}
