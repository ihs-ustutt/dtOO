#include "scaMuParserThreeD.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtMuParser.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaMuParserThreeD::scaMuParserThreeD() : scaThreeD() {
  }

  scaMuParserThreeD::scaMuParserThreeD(
    scaMuParserThreeD const & orig
  ) : scaThreeD(orig), _parser(new dtMuParser()) {
		dt__forFromToIndex(0, 3, ii) {
      _arg.push_back( new double(0.) );
		  _argStr[ii] = orig._argStr[ii];
		}
    _expressionStr = orig._expressionStr;
    
    
    try {
      //
      // create parser
      //
			dt__forFromToIndex(0, 3, ii) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserThreeD(), << e.GetMsg() );
    }  
  }

  scaMuParserThreeD::scaMuParserThreeD(
	  std::string const expression, 
    std::string const argOne, 
    std::string const argTwo,
    std::string const argThree    
	) : scaThreeD(), _parser(new dtMuParser()) {
		dt__forFromToIndex(0, 3, ii) _arg.push_back( new double(0.) );
		_argStr[0] = argOne;
		_argStr[1] = argTwo;
    _argStr[2] = argThree;
    _expressionStr = expression;


    try {
      //
      // create parser
      //
			dt__forFromToIndex(0, 3, ii) {
        _parser->DefineVar( _argStr[ii].c_str(), &(_arg[ii]) );
			}
      _parser->SetExpr( _expressionStr.c_str() );
    }
    //
    //error handling
    //
    catch (mu::Parser::exception_type &e) {
      dt__throw( scaMuParserThreeD(), << e.GetMsg() );
    }  
  }


  scaMuParserThreeD::~scaMuParserThreeD() {
  }

  float scaMuParserThreeD::YFloat( 
    float const & x0, float const & x1, float const & x2 
  ) const {
    float yy;
		
    try {
      const_cast< double& >(_arg[0]) = static_cast<double>(x0);
			const_cast< double& >(_arg[1]) = static_cast<double>(x1);
      const_cast< double& >(_arg[2]) = static_cast<double>(x2);
			
			int nDim;
      double * yyD = _parser->Eval(nDim);
		  dt__throwIf(nDim!=1, Y());
		  yy = static_cast<float>(yyD[0]);
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw(Y(), << e.GetMsg() );
    }
    return yy;		
	}
	
	bool scaMuParserThreeD::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1) && (dir!=2), closed() );
		return false;
	}
	
  scaMuParserThreeD * scaMuParserThreeD::clone( void ) const {
    return new scaMuParserThreeD( *this );
  }
  
	scaMuParserThreeD * scaMuParserThreeD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< scaMuParserThreeD >(*this, dtT);
	}    
  
  scaMuParserThreeD * scaMuParserThreeD::create( void ) const {
    return new scaMuParserThreeD();
  }
}