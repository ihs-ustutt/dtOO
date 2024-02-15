/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

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
      virtual bool closed( dtInt const & dir ) const;    
    private:
      dt__pH(dtMuParser) _parser;
      dt__pVH(double) _arg;
      std::string _expressionStr;
      std::string _argStr[3];
    };
}
#endif	/* VEC3DMUPARSERTHREED_H */
