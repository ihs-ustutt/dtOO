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
  class dtTransformer;
  
  class vec3dMuParserTwoD : public vec3dTwoD {
  public:
    dt__class(vec3dMuParserTwoD, analyticFunction);     
    vec3dMuParserTwoD();
    vec3dMuParserTwoD(vec3dMuParserTwoD const & orig);
    vec3dMuParserTwoD(
      std::string const expression, 
      std::string const argOne, 
      std::string const argTwo
    );
    vec3dMuParserTwoD * clone( void ) const;
    virtual vec3dMuParserTwoD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;       
    vec3dMuParserTwoD * create( void ) const;    
    virtual ~vec3dMuParserTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    dt__pH(mu::Parser) _parser;
    dt__pVH(double) _arg;
    std::string _expressionStr;
    std::string _argStr[2];
  };
}
#endif	/* VEC3DMUPARSERTWOD_H */