#include "scaMuParserTwoD.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtMuParser.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaMuParserTwoD::scaMuParserTwoD() : scaTwoD() {
  }

  scaMuParserTwoD::scaMuParserTwoD(
    scaMuParserTwoD const & orig
  ) : scaTwoD(orig), _parser(new dtMuParser()) {
		for (int ii=0; ii<2; ii++) {
      _arg.push_back( new double(0.) );
		  _argStr[ii] = orig._argStr[ii];
		}
    _expressionStr = orig._expressionStr;
    
    
    try {
      //
      // create parser
      //
			for (int ii=0; ii<2; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserTwoD(), << e.GetMsg() );
    }  
  }

  scaMuParserTwoD::scaMuParserTwoD(
	  std::string const expression, 
    std::string const argOne, 
    std::string const argTwo  
	) : scaTwoD(), _parser(new dtMuParser()) {
		for (int ii=0; ii<2; ii++) {
      _arg.push_back( new double(0.) );
		}
		_argStr[0] = argOne;
		_argStr[1] = argTwo;
    _expressionStr = expression;


    try {
      //
      // create parser
      //
			for (int ii=0; ii<2; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserTwoD(), << e.GetMsg() );
    }  
  }


  scaMuParserTwoD::~scaMuParserTwoD() {
  }

  dtReal scaMuParserTwoD::YFloat( dtReal const & x0, dtReal const & x1 ) const {
    dtReal yy;
		
    try {
      const_cast< double& >(_arg[0]) = static_cast<double>(x0);
			const_cast< double& >(_arg[1]) = static_cast<double>(x1);
			
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__throwIf(nDim!=1, Y());
		  yy = static_cast<dtReal>(yyD[0]);
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool scaMuParserTwoD::closed( dtInt const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  scaMuParserTwoD * scaMuParserTwoD::clone( void ) const {
    return new scaMuParserTwoD( *this );
  }
  
	scaMuParserTwoD * scaMuParserTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< scaMuParserTwoD >(*this, dtT);
	}    
  
  scaMuParserTwoD * scaMuParserTwoD::create( void ) const {
    return new scaMuParserTwoD();
  }
}