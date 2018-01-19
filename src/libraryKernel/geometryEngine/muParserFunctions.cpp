#include "muParserFunctions.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

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
  
  void muParserFunctions::registrateAnalyticFunctions( 
    aFPtrVec const * const aFPtrVec 
  ) {
    _aFPtrVec = aFPtrVec;
  }
        
  ::mu::value_type muParserFunctions::oneInRange(
    ::mu::value_type low, ::mu::value_type high, ::mu::value_type value
  ) {
    if ( (value >= low) || (value < high) ) return 1.;
    
    return 0.;
  }
  
  ::mu::value_type muParserFunctions::YFix( ::mu::char_type const * expr) {
    std::string str(expr);
    
    return
      dtXmlParser::muParseString(
        dtXmlParser::replaceDependencies(
          str, NULL, muParserFunctions::instance()->_aFPtrVec, NULL
        )
      );
  }  
  
  ::mu::value_type muParserFunctions::Y( ::mu::char_type const * expr, ::mu::value_type val) {
    std::string str(expr);
    str
    =
    dtXmlParser::replaceStringInString( 
      "()", 
      "("+dtXmlParser::floatToString(val)+")",
      str
    );
    
    return
      dtXmlParser::muParseString(
        dtXmlParser::replaceDependencies(
          str, NULL, muParserFunctions::instance()->_aFPtrVec, NULL
        )
      );
  }    
}
