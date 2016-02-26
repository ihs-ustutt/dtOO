#include "vec3dMuParserOneD.h"

#include <logMe/logMe.h>
#include <muParser.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  vec3dMuParserOneD::vec3dMuParserOneD() : vec3dOneD() {
    
  }

  vec3dMuParserOneD::vec3dMuParserOneD(
    vec3dMuParserOneD const & orig
  ) : vec3dOneD(orig), _parser(new mu::Parser) {
		for (int ii=0; ii<1; ii++) {
      _arg.push_back( new double(0.) );
		  _argStr[ii] = orig._argStr[ii];
		}
    _expressionStr = orig._expressionStr;
    
    
    try {
      //
      // create parser
      //
			for (int ii=0; ii<1; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( vec3dMuParserOneD(), << e.GetMsg() );
    }  
  }

  vec3dMuParserOneD::vec3dMuParserOneD(
	  std::string const expression, 
    std::string const argOne
	) : vec3dOneD(), _parser(new mu::Parser()) {
		for (int ii=0; ii<2; ii++) {
      _arg.push_back( new double(0.) );
		}
		_argStr[0] = argOne;
    _expressionStr = expression;


    try {
      //
      // create parser
      //
			for (int ii=0; ii<1; ii++) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( vec3dMuParserOneD(), << e.GetMsg() );
    }  
  }


  vec3dMuParserOneD::~vec3dMuParserOneD() {
  }

  aFY vec3dMuParserOneD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=1, Y());
    aFY yy(3,0.);
		
    try {
			for (int ii=0; ii<1; ii++) {
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
	
	bool vec3dMuParserOneD::closed( int const & dir ) const {
		dt__throwIf(dir!=0, closed() );
		return false;
	}
	
  vec3dMuParserOneD * vec3dMuParserOneD::clone( void ) const {
    return new vec3dMuParserOneD( *this );
  }
  
	vec3dMuParserOneD * vec3dMuParserOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dMuParserOneD >(*this, dtT);
	}    
  
  vec3dMuParserOneD * vec3dMuParserOneD::create( void ) const {
    return new vec3dMuParserOneD();
  }
}