#include "scaMuParserOneD.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtMuParser.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaMuParserOneD::scaMuParserOneD() : scaOneD() {
  }

  scaMuParserOneD::scaMuParserOneD(
    scaMuParserOneD const & orig
  ) : scaOneD(orig), _parser(new dtMuParser()), _argument(new double(0)) {
    _expressionStr = orig._expressionStr;
    _argumentStr = orig._argumentStr;
    
    try {
      //
      // create parser
      //
      _parser->DefineVar( _argumentStr.c_str(), _argument.get() );
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserOneD(), << e.GetMsg() );
    }  
  }

  scaMuParserOneD::scaMuParserOneD(
    std::string const expression, std::string const argument
  ) : scaOneD(), _parser(new dtMuParser()), _argument(new double(0)) {
    _expressionStr = expression;
    _argumentStr = argument;

    try {
      //
      // create parser
      //
      _parser->DefineVar( _argumentStr.c_str(), _argument.get() );
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserOneD(), 
              << e.GetMsg() );
    }  
  }

  scaMuParserOneD::~scaMuParserOneD() {

  }

  scaMuParserOneD * scaMuParserOneD::clone( void ) const {
    return new scaMuParserOneD( *this );
  }
  
	scaMuParserOneD * scaMuParserOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< scaMuParserOneD >(*this, dtT);
	}  
  
  scaMuParserOneD * scaMuParserOneD::create( void ) const {
    return new scaMuParserOneD();
  }

  dtReal scaMuParserOneD::YFloat(dtReal const & xx) const {
    dtReal myFloat;
    try {
      *_argument = (double) xx;
      myFloat = (dtReal) _parser->Eval();
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw(getValue, << e.GetMsg() );
    }
    return myFloat;
  }

  void scaMuParserOneD::setArgumentRange(dtReal xMin, dtReal xMax) {
    scaOneD::setMinMax(xMin, xMax);
  }
}