#ifndef vec3dThreeDInMap3dTo3d_H
#define	vec3dThreeDInMap3dTo3d_H

#include <dtOOTypeDef.h>

#include "map3dTo3d.h"
#include "analyticGeometryCompound.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec3dThreeD;
  
  class vec3dThreeDInMap3dTo3d : public map3dTo3d {
    public:
      dt__class(vec3dThreeDInMap3dTo3d, analyticGeometry);
      vec3dThreeDInMap3dTo3d();
      vec3dThreeDInMap3dTo3d(const vec3dThreeDInMap3dTo3d& orig);
      vec3dThreeDInMap3dTo3d(    
  	    vec3dThreeD const * const v3d, 
  		  map3dTo3d const * const m3d, 
  		  bool percentF = false
      );
      virtual ~vec3dThreeDInMap3dTo3d();
      virtual vec3dThreeDInMap3dTo3d * create( void ) const;    
      virtual vec3dThreeDInMap3dTo3d * clone( void ) const;
      virtual vec3dThreeDInMap3dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;         
      virtual dtPoint3 getPoint( 
        dtReal const & uu, dtReal const & vv, dtReal const & ww 
      ) const;
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
    private:
      ptrHandling< vec3dThreeD > _v3d;
      ptrHandling< map3dTo3d > _m3d;
      bool _percentF;
  };
  typedef analyticGeometryCompound<vec3dThreeDInMap3dTo3d> vec3dThreeDInMap3dTo3dCompound;
}
#endif	/* vec3dThreeDInMap3dTo3d_H */

