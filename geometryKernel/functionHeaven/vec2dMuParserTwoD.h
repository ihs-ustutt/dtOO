#ifndef VEC2DMUPARSERTWOD_H
#define	VEC2DMUPARSERTWOD_H

#include "vec2dTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace mu {
  class Parser;
}

namespace dtOO {
  class vec2dMuParserTwoD : public vec2dTwoD {
  public:
    dt__CLASSSTD(vec2dMuParserTwoD, analyticFunction);     
    vec2dMuParserTwoD();
    vec2dMuParserTwoD(vec2dMuParserTwoD const & orig);
    vec2dMuParserTwoD(
      std::string const expression, 
      std::string const argOne, 
      std::string const argTwo
    );
    vec2dMuParserTwoD * clone( void ) const;
    vec2dMuParserTwoD * create( void ) const;    
    virtual ~vec2dMuParserTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    mu::Parser * _parser;
    double * _arg[2];
    std::string _expressionStr;
    std::string _argStr[2];
  };
}
#endif	/* VEC2DMUPARSERTWOD_H */