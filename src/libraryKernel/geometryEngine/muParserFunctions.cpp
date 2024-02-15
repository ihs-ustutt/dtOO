/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "muParserFunctions.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
  dt__pH(muParserFunctions) muParserFunctions::_pH(NULL);
  
  muParserFunctions::muParserFunctions() {
  }

  muParserFunctions * muParserFunctions::instance( void ) {
    if (!_pH) _pH.reset( new muParserFunctions() );
    return _pH.get();
  }
  
  muParserFunctions::~muParserFunctions() {
  }
  
  void muParserFunctions::registrateAnalyticFunctions( 
    lvH_analyticFunction const * const aFPtrVec 
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
