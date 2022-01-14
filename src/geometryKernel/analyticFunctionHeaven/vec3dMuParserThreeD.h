#ifndef VEC3DMUPARSERTHREED_H
#define	VEC3DMUPARSERTHREED_H

#include <dtOOTypeDef.h>

#include "vec3dThreeD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  class dtMuParser;
  
  class vec3dMuParserThreeD : public vec3dThreeD {
    public:
      dt__class(vec3dMuParserThreeD, analyticFunction);     
      vec3dMuParserThreeD();
      vec3dMuParserThreeD(vec3dMuParserThreeD const & orig);
      vec3dMuParserThreeD(
        std::string const expression, 
        std::string const argOne, 
        std::string const argTwo, 
        std::string const argThree
      );
      vec3dMuParserThreeD * clone( void ) const;
      virtual vec3dMuParserThreeD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;       
      vec3dMuParserThreeD * create( void ) const;    
      virtual ~vec3dMuParserThreeD();
      virtual aFY Y( aFX const & xx ) const;
      virtual bool closed( int const & dir ) const;    
    private:
      dt__pH(dtMuParser) _parser;
      dt__pVH(double) _arg;
      std::string _expressionStr;
      std::string _argStr[3];
    };
}
#endif	/* VEC3DMUPARSERTHREED_H */