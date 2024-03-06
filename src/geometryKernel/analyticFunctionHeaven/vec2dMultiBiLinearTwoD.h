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

#ifndef vec2dMultiBiLinearTwoD_H
#define	vec2dMultiBiLinearTwoD_H

#include <dtOOTypeDef.h>

#include "vec2dTwoD.h"
#include "vec2dBiLinearTwoD.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  
  class vec2dMultiBiLinearTwoD : public vec2dTwoD {
  public:
    dt__class(vec2dMultiBiLinearTwoD, analyticFunction);     
    vec2dMultiBiLinearTwoD();
    vec2dMultiBiLinearTwoD( vec2dMultiBiLinearTwoD const & orig );
    vec2dMultiBiLinearTwoD( twoDArrayHandling< dtPoint2 > const & pp );
    vec2dMultiBiLinearTwoD * clone( void ) const;
    virtual vec2dMultiBiLinearTwoD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;           
    vec2dMultiBiLinearTwoD * create( void ) const;    
    virtual ~vec2dMultiBiLinearTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( dtInt const & dir ) const;    
    aFX invY(aFY const & yy) const;
  private:
    twoDArrayHandling< vec2dBiLinearTwoD > _piece;
  };
}
#endif	/* vec2dMultiBiLinearTwoD_H */
