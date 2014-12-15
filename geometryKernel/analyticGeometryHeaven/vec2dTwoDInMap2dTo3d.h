#ifndef VEC2DTWODINMAP2DTO3D_H
#define	VEC2DTWODINMAP2DTO3D_H

#include <dtLinearAlgebra.h>
#include "map2dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class vec2dTwoD;
  class dtTransformer;
  
  class vec2dTwoDInMap2dTo3d : public map2dTo3d {
    public:
      dt__CLASSSTD(vec2dTwoDInMap2dTo3d, analyticGeometry);
      vec2dTwoDInMap2dTo3d();
      vec2dTwoDInMap2dTo3d(const vec2dTwoDInMap2dTo3d& orig);
      vec2dTwoDInMap2dTo3d(vec2dTwoD const * const v2d2d, map2dTo3d const * const m2d);
      vec2dTwoDInMap2dTo3d(vec2dTwoD const * const v2d2d, map2dTo3d const * const m2d, bool const & percentF);
      virtual ~vec2dTwoDInMap2dTo3d();
      //
      // overload
      //      
      virtual vec2dTwoDInMap2dTo3d * clone( void ) const;
      virtual vec2dTwoDInMap2dTo3d * create( void ) const;    
      virtual vec2dTwoDInMap2dTo3d * 
      cloneTransformed( dtTransformer const * const dtT ) const;      
      virtual dtPoint3 getPoint( float const & uu, float const & vv ) const;
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      //
      //
      //        
      vec2dTwoD const * ptrToVec2dTwoD( void ) const;
      map2dTo3d const * ptrToMap2dTo3d( void ) const;      
  private:
    dt__pH(vec2dTwoD) _v2d;
    dt__pH(map2dTo3d) _m2d;
    bool _percentF;
  };
}
#endif	/* VEC2DTWODINMAP2DTO3D_H */

