#include "vec2dMuParserTwoD.h"

#include <logMe/logMe.h>
#include <muParser.h>

namespace dtOO {
  vec2dMuParserTwoD::vec2dMuParserTwoD() : vec2dTwoD() {
    _parser = NULL;
  }

  vec2dMuParserTwoD::vec2dMuParserTwoD(vec2dMuParserTwoD const & orig) : vec2dTwoD(orig) {
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
      dt__THROW( vec2dMuParserTwoD(), << e.GetMsg() );
    }  
  }

  vec2dMuParserTwoD::vec2dMuParserTwoD(
	  std::string const expression, 
    std::string const argOne, std::string const argTwo
	) : vec2dTwoD() {
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
      dt__THROW( vec2dMuParserTwoD(), << e.GetMsg() );
    }  
  }


  vec2dMuParserTwoD::~vec2dMuParserTwoD() {
    delete _parser;
  }

  aFY vec2dMuParserTwoD::Y( aFX const & xx ) const {
    dt__THROW_IF(xx.size()!=2, Y());
    aFY yy(2,0.);
		
    try {
			for (int ii=0; ii<2; ii++) {			
        *_arg[ii] = static_cast<double>(xx[ii]);
		  }
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__THROW_IF(nDim!=2, Y());
			for (int ii=0; ii<2; ii++) {
				yy[ii] = static_cast<float>(yyD[ii]);
			}
    }
    catch (mu::Parser::exception_type &e) {
      dt__THROW(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool vec2dMuParserTwoD::closed( int const & dir ) const {
		dt__THROW_IF( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec2dMuParserTwoD * vec2dMuParserTwoD::clone( void ) const {
    return new vec2dMuParserTwoD( *this );
  }
  
  vec2dMuParserTwoD * vec2dMuParserTwoD::create( void ) const {
    return new vec2dMuParserTwoD();
  }
}