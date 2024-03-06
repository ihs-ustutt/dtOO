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

#ifndef VEC2DONEDINMAP2DTO3D_H
#define	VEC2DONEDINMAP2DTO3D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "map1dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec2dOneD;
  class map2dTo3d;
  
  class vec2dOneDInMap2dTo3d : public map1dTo3d {
    public:
      dt__class(vec2dOneDInMap2dTo3d, analyticGeometry);
      vec2dOneDInMap2dTo3d();
      vec2dOneDInMap2dTo3d(const vec2dOneDInMap2dTo3d& orig);
      vec2dOneDInMap2dTo3d(vec2dOneD const * const v2d1d, map2dTo3d const * const m2d);
      vec2dOneDInMap2dTo3d(vec2dOneD const * const v2d1d, map2dTo3d const * const m2d, bool const & percentF);
      virtual ~vec2dOneDInMap2dTo3d();
      //
      // overload 
      //
      virtual vec2dOneDInMap2dTo3d * clone( void ) const;
      virtual vec2dOneDInMap2dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;          
      virtual vec2dOneDInMap2dTo3d * create( void ) const;    
      virtual dtPoint3 getPoint( dtReal const & uu ) const;
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      //
      //
      //
      vec2dOneD const * ptrToVec2dOneD( void ) const;
      map2dTo3d const * ptrToMap2dTo3d( void ) const;      
      bool isPercent( void ) const;
  private:
    ptrHandling< vec2dOneD > _v2d;
    ptrHandling< map2dTo3d > _m2d;
    bool _percentF;
  };
}
#endif	/* VEC2DONEDINMAP2DTO3D_H */
