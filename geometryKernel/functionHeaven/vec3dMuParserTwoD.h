#ifndef VEC3DMUPARSERTWOD_H
#define	VEC3DMUPARSERTWOD_H

#include "vec3dTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace mu {
  class Parser;
}

namespace dtOO {
  class vec3dMuParserTwoD : public vec3dTwoD {
  public:
    dt__CLASSSTD(vec3dMuParserTwoD, analyticFunction);     
    vec3dMuParserTwoD();
    vec3dMuParserTwoD(vec3dMuParserTwoD const & orig);
    vec3dMuParserTwoD(
      std::string const expression, 
      std::string const argOne, 
      std::string const argTwo
    );
    vec3dMuParserTwoD * clone( void ) const;
    vec3dMuParserTwoD * create( void ) const;    
    virtual ~vec3dMuParserTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    mu::Parser * _parser;
    double * _arg[2];
    std::string _expressionStr;
    std::string _argStr[2];
  };
}
#endif	/* VEC3DMUPARSERTWOD_H */