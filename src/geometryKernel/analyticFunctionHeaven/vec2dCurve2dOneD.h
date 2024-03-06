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

#ifndef VEC2DCURVE2DONED_H
#define	VEC2DCURVE2DONED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dOneD.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  class renderInterface;
  class dtTransformer;
  
  class vec2dCurve2dOneD : public vec2dOneD {
    public:
      dt__class(vec2dCurve2dOneD, analyticFunction);     
      vec2dCurve2dOneD();
      vec2dCurve2dOneD(const vec2dCurve2dOneD& orig);
      vec2dCurve2dOneD(dtCurve2d const * const dtC2d);
      virtual vec2dCurve2dOneD * clone( void ) const;
      virtual vec2dCurve2dOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;          
      virtual vec2dCurve2dOneD * create( void ) const;    
      virtual ~vec2dCurve2dOneD();
      using vec2dOneD::Y; // name hiding
      virtual aFY Y( dtReal const & xx) const;
      vectorHandling< renderInterface * > getRender( void ) const;
      dtCurve2d const * ptrDtCurve2d( void ) const;
      virtual bool closed( dtInt const & dir ) const;
    private:
      ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* VEC2DCURVE2DONED_H */
