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

#ifndef vec3dOneDInMap3dTo3d_H
#define	vec3dOneDInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include "map1dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec3dOneD;
  class map3dTo3d;
  class renderInterface;
  
  class vec3dOneDInMap3dTo3d : public map1dTo3d {
    public:
      dt__class(vec3dOneDInMap3dTo3d, analyticGeometry);
      vec3dOneDInMap3dTo3d();
      vec3dOneDInMap3dTo3d(const vec3dOneDInMap3dTo3d& orig);
      vec3dOneDInMap3dTo3d(    
        vec3dOneD const * const v1d, 
        map3dTo3d const * const m3d, 
        bool percentF = false
      );        
      virtual ~vec3dOneDInMap3dTo3d();
      virtual vec3dOneDInMap3dTo3d * clone( void ) const;
      virtual vec3dOneDInMap3dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;     
      virtual vec3dOneDInMap3dTo3d * create( void ) const;
      virtual dtPoint3 getPoint( dtReal const & uu ) const;
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      vec3dOneD const * refToVec3dOneD( void ) const;
      map3dTo3d const * refToMap3dTo3d( void ) const;
      vectorHandling< renderInterface * > getExtRender( void ) const;
    private:
      ptrHandling< vec3dOneD > _v1d;
      ptrHandling< map3dTo3d > _m3d;
      bool _percentF;    
  };
}
#endif	/* vec3dOneDInMap3dTo3d_H */
