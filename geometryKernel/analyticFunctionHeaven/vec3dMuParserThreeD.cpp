#include "vec3dMuParserThreeD.h"

#include <logMe/logMe.h>
#include <muParser.h>

namespace dtOO {
  vec3dMuParserThreeD::vec3dMuParserThreeD() : vec3dThreeD() {
    _parser = NULL;
  }

  vec3dMuParserThreeD::vec3dMuParserThreeD(vec3dMuParserThreeD const & orig) : vec3dThreeD(orig) {
    _parser = new mu::Parser;
		for (int ii=0; ii<3; ii++) {
      _arg[ii] = new double(0.);
		  _argStr[ii] = orig._argStr[ii];
		}
    _expressionStr = orig._expressionStr;
    
    
    try {
      //
      // create parser
      //
			for (int ii=0; ii<3; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), _arg[ii] );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__THROW( vec3dMuParserThreeD(), << e.GetMsg() );
    }  
  }

  vec3dMuParserThreeD::vec3dMuParserThreeD(
	  std::string const expression, 
    std::string const argOne, 
    std::string const argTwo, 
    std::string const argThree  
	) : vec3dThreeD() {
    _parser = new mu::Parser;
		for (int ii=0; ii<3; ii++) {
      _arg[ii] = new double(0.);
		}
		_argStr[0] = argOne;
		_argStr[1] = argTwo;
		_argStr[2] = argThree;
    _expressionStr = expression;


    try {
      //
      // create parser
      //
			for (int ii=0; ii<3; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), _arg[ii] );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__THROW( vec3dMuParserThreeD(), << e.GetMsg() );
    }  
  }


  vec3dMuParserThreeD::~vec3dMuParserThreeD() {
    delete _parser;
  }

  aFY vec3dMuParserThreeD::Y( aFX const & xx ) const {
    dt__THROW_IF(xx.size()!=3, Y());
    aFY yy(3,0.);
		
    try {
			for (int ii=0; ii<3; ii++) {			
        *_arg[ii] = static_cast<double>(xx[ii]);
		  }
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__THROW_IF(nDim!=3, Y());
			for (int ii=0; ii<3; ii++) {
				yy[ii] = static_cast<float>(yyD[ii]);
			}
    }
    catch (mu::Parser::exception_type &e) {
      dt__THROW(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool vec3dMuParserThreeD::closed( int const & dir ) const {
		dt__THROW_IF( (dir!=0) && (dir!=1) && (dir!=2), closed );
		return false;
	}
	
  vec3dMuParserThreeD * vec3dMuParserThreeD::clone( void ) const {
    return new vec3dMuParserThreeD( *this );
  }
  
  vec3dMuParserThreeD * vec3dMuParserThreeD::create( void ) const {
    return new vec3dMuParserThreeD();
  }
}