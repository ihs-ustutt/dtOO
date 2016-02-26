#ifndef vec3dMuParserOneD_H
#define	vec3dMuParserOneD_H

#include "vec3dOneD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace mu {
  class Parser;
}

namespace dtOO {
  class dtTransformer;
  
  class vec3dMuParserOneD : public vec3dOneD {
  public:
    dt__class(vec3dMuParserOneD, analyticFunction);     
    vec3dMuParserOneD();
    vec3dMuParserOneD(vec3dMuParserOneD const & orig);
    vec3dMuParserOneD(
      std::string const expression, 
      std::string const argOne
    );
    vec3dMuParserOneD * clone( void ) const;
    virtual vec3dMuParserOneD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;       
    vec3dMuParserOneD * create( void ) const;    
    virtual ~vec3dMuParserOneD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    dt__pH(mu::Parser) _parser;
    dt__pVH(double) _arg;
    std::string _expressionStr;
    std::string _argStr[1];
  };
}
#endif	/* vec3dMuParserOneD_H */