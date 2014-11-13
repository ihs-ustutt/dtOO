#ifndef ROTATINMAP2DTO3DPHIMS_H
#define	ROTATINMAP2DTO3DPHIMS_H

#include <dtLinearAlgebra.h>
#include "rotatingMap2dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  
  class rotatingMap2dTo3dPhiMS : public rotatingMap2dTo3d {
  public:
    dt__CLASSSTD(rotatingMap2dTo3dPhiMS, analyticGeometry);
    rotatingMap2dTo3dPhiMS();
    rotatingMap2dTo3dPhiMS( dtVector3 const & vv, map2dTo3d const * const m2d );
    rotatingMap2dTo3dPhiMS(const rotatingMap2dTo3dPhiMS& orig);
    virtual ~rotatingMap2dTo3dPhiMS();
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    void setAngle( float const & angle );
    virtual dtPoint3 getPoint( float const & phi, float const & mm, float const & ss ) const; 
  private:
    float _angle;
  };
}
#endif	/* ROTATINMAP2DTO3DPHIMS_H */

