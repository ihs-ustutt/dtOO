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
  
  mu::value_type muParserFunctions::evalAF(mu::char_type const * cc) { 
    //return muParserFunctions::instance()->one(v);
    std::cout << "Wixxen: " << cc << std::endl;
    return 1.;
  }  
}
