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

#ifndef VEC3DCURVEONED_H
#define	VEC3DCURVEONED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dOneD.h"
#include "analyticFunctionCompound.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class renderInterface;
  class dtTransformer;  
  
  class vec3dCurveOneD : public vec3dOneD {
  public:
    dt__class(vec3dCurveOneD, analyticFunction);     
    vec3dCurveOneD();
    vec3dCurveOneD(const vec3dCurveOneD& orig);
    vec3dCurveOneD(dtCurve const * const dtC);
    virtual vec3dCurveOneD * clone( void ) const;
    virtual vec3dCurveOneD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;      
    virtual vec3dCurveOneD * create( void ) const;    
    virtual ~vec3dCurveOneD();
    virtual aFY Y( aFX const & xx) const;    
    dtCurve * ptrDtCurve(void) const;
    dtCurve const * const ptrConstDtCurve(void) const;
    virtual bool closed( dtInt const & dir ) const;
  private:
    ptrHandling<dtCurve> _dtC;
  };
  typedef analyticFunctionCompound<vec3dCurveOneD> vec3dCurveOneDCompound;
}
#endif	/* VEC3DCURVEONED_H */
