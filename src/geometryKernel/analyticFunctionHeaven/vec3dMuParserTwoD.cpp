#include "vec3dMuParserTwoD.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtMuParser.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  vec3dMuParserTwoD::vec3dMuParserTwoD() : vec3dTwoD() {
    
  }

  vec3dMuParserTwoD::vec3dMuParserTwoD(
    vec3dMuParserTwoD const & orig
  ) : vec3dTwoD(orig), _parser( new dtMuParser() ) {
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
      dt__throw( vec3dMuParserTwoD(), << e.GetMsg() );
    }  
  }

  vec3dMuParserTwoD::vec3dMuParserTwoD(
	  std::string const expression, 
    std::string const argOne, std::string const argTwo
	) : vec3dTwoD(), _parser(new dtMuParser()) {
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
      dt__throw( vec3dMuParserTwoD(), << e.GetMsg() );
    }  
  }


  vec3dMuParserTwoD::~vec3dMuParserTwoD() {
  }

  aFY vec3dMuParserTwoD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=2, Y());
    aFY yy(3,0.);
		
    try {
			for (int ii=0; ii<2; ii++) {			
        const_cast< double& >(_arg[ii]) = static_cast<double>(xx[ii]);
		  }
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__throwIf(nDim!=3, Y());
			for (int ii=0; ii<3; ii++) {
				yy[ii] = static_cast<float>(yyD[ii]);
			}
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool vec3dMuParserTwoD::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec3dMuParserTwoD * vec3dMuParserTwoD::clone( void ) const {
    return new vec3dMuParserTwoD( *this );
  }
  
	vec3dMuParserTwoD * vec3dMuParserTwoD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dMuParserTwoD >(*this, dtT);
	}    
  
  vec3dMuParserTwoD * vec3dMuParserTwoD::create( void ) const {
    return new vec3dMuParserTwoD();
  }
}