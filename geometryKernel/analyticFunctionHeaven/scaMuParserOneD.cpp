#include "scaMuParserOneD.h"

#include <logMe/logMe.h>
#include <muParser.h>
#include <progHelper.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>

namespace dtOO {
  scaMuParserOneD::scaMuParserOneD() : scaOneD() {
    _parser = NULL;
  }

  scaMuParserOneD::scaMuParserOneD(scaMuParserOneD const & orig) : scaOneD(orig) {
    _parser = new mu::Parser;
    _argument = new double(0);
    _expressionStr = orig._expressionStr;
    _argumentStr = orig._argumentStr;
    
    try {
      //
      // create parser
      //
      _parser->DefineVar( _argumentStr.c_str(), _argument );
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

  scaMuParserOneD::scaMuParserOneD(std::string const expression, std::string const argument) : scaOneD() {
    _parser = new mu::Parser;
    _argument = new double(0);
    _expressionStr = expression;
    _argumentStr = argument;

    try {
      //
      // create parser
      //
      _parser->DefineVar( _argumentStr.c_str(), _argument );
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


  scaMuParserOneD::scaMuParserOneD(mu::Parser * parserP, double * argumentP, float const * const xxMin, float const * const xxMax) : scaOneD() {
    _parser = parserP;
    _argument = argumentP;
    
    scaOneD::setMinMax(*xxMin, *xxMax);
  }

  scaMuParserOneD::~scaMuParserOneD() {
    delete _argument;
    delete _parser;
  }

  scaMuParserOneD * scaMuParserOneD::clone( void ) const {
    return new scaMuParserOneD( *this );
  }
  
  scaMuParserOneD * scaMuParserOneD::create( void ) const {
    return new scaMuParserOneD();
  }

  float scaMuParserOneD::YFloat(float const & xx) const {
    float myFloat;
    try {
      *_argument = (double) xx;
      myFloat = (float) _parser->Eval();
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw(getValue, << e.GetMsg() );
    }
    return myFloat;
  }

  void scaMuParserOneD::setArgumentRange(float xMin, float xMax) {
    scaOneD::setMinMax(xMin, xMax);
  }
  
  vectorHandling< renderInterface * > scaMuParserOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		
		vectorHandling< dtPoint2 > p2(nU);
    float interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			float iiF = static_cast<float>(ii);
      float xx = xMin(0) + iiF * interval;
      float yy = YFloat(xx);
			p2[ii] = dtPoint2(xx, yy);
    }
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid2dLine(p2);
		
		return rV;
  }
}