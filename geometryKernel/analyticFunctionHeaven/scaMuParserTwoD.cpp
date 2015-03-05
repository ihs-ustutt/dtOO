#include "scaMuParserTwoD.h"

#include <logMe/logMe.h>
#include <muParser.h>

namespace dtOO {
  scaMuParserTwoD::scaMuParserTwoD() : scaTwoD() {
    _parser = NULL;
  }

  scaMuParserTwoD::scaMuParserTwoD(scaMuParserTwoD const & orig) : scaTwoD(orig) {
    _parser = new mu::Parser;
		for (int ii=0; ii<2; ii++) {
      _arg[ii] = new double(0.);
		  _argStr[ii] = orig._argStr[ii];
		}
    _expressionStr = orig._expressionStr;
    
    
    try {
      //
      // create parser
      //
			for (int ii=0; ii<2; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), _arg[ii] );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__THROW( scaMuParserTwoD(), << e.GetMsg() );
    }  
  }

  scaMuParserTwoD::scaMuParserTwoD(
	  std::string const expression, 
    std::string const argOne, 
    std::string const argTwo  
	) : scaTwoD() {
    _parser = new mu::Parser;
		for (int ii=0; ii<2; ii++) {
      _arg[ii] = new double(0.);
		}
		_argStr[0] = argOne;
		_argStr[1] = argTwo;
    _expressionStr = expression;


    try {
      //
      // create parser
      //
			for (int ii=0; ii<2; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), _arg[ii] );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__THROW( scaMuParserTwoD(), << e.GetMsg() );
    }  
  }


  scaMuParserTwoD::~scaMuParserTwoD() {
    delete _parser;
  }

  float scaMuParserTwoD::YFloat( float const & x0, float const & x1 ) const {
    float yy;
		
    try {
      *_arg[0] = static_cast<double>(x0);
			*_arg[1] = static_cast<double>(x1);
			
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__THROW_IF(nDim!=1, Y());
		  yy = static_cast<float>(yyD[0]);
    }
    catch (mu::Parser::exception_type &e) {
      dt__THROW(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool scaMuParserTwoD::closed( int const & dir ) const {
		dt__THROW_IF( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  scaMuParserTwoD * scaMuParserTwoD::clone( void ) const {
    return new scaMuParserTwoD( *this );
  }
  
  scaMuParserTwoD * scaMuParserTwoD::create( void ) const {
    return new scaMuParserTwoD();
  }
}