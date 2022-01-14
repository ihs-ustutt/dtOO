#include "qtXmlBase.h"

#include <geometryEngine/dtMuParser.h>
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/analyticFunction.h>

namespace dtOO {
  qtXmlBase::qtXmlBase() {
  }

  qtXmlBase::~qtXmlBase() {
  }

  dtReal qtXmlBase::muParseString( std::string const expression ) {
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      return ( (dtReal) parser.Eval() );
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseString(), 
        << dt__eval(expression) << std::endl
        << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }

  std::vector< dtReal > qtXmlBase::muParseCSString( 
    std::string const expression 
  ) {
    if ( !stringPrimitive::stringContains(",", expression) ) {
      return std::vector< dtReal >(1, muParseString(expression));
    }
    
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      
      int nDim;
      double * yyD = parser.Eval(nDim);
      std::vector< dtReal > yy(nDim, 0.);
			dt__forFromToIndex(0, nDim, ii) yy[ii] = static_cast<dtReal>(yyD[ii]);
      
      return yy;
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseCSString(), << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }  

  int qtXmlBase::muParseStringInt( std::string const expression ) {
    int ret = static_cast< int >(muParseString(expression));
    return ret;
  }  

  std::vector< int > qtXmlBase::muParseCSStringInt( 
    std::string const expression 
  ) {
    std::vector< dtReal > vec = muParseCSString(expression);
    std::vector< int > retVec(vec.size());
    dt__forAllIndex(vec, ii) retVec[ii] = static_cast< int >(vec[ii]);

    return retVec;
  }  
}