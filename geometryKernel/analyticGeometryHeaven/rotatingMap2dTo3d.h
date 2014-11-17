#ifndef ROTATINMAP2DTO3D_H
#define	ROTATINMAP2DTO3D_H

#include <dtLinearAlgebra.h>
#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  
  class rotatingMap2dTo3d : public map3dTo3d {
  public:
    dt__CLASSSTD(rotatingMap2dTo3d, analyticGeometry);
    rotatingMap2dTo3d();
    rotatingMap2dTo3d( dtVector3 const & vv, map2dTo3d const * const m2d );
    rotatingMap2dTo3d(const rotatingMap2dTo3d& orig);
    virtual ~rotatingMap2dTo3d();
    virtual rotatingMap2dTo3d * clone( void ) const;
    virtual rotatingMap2dTo3d * create( void ) const;    
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv, float const & ww ) const; 
    float u_phi(float const & arg) const;
    float u_phirvw(float const & phir, float const & vv, float const & ww) const;
//    float phi_u(float const & arg) const;
    float v_m(float const & arg) const;
    float v_mw(float const & mm, float const & ss) const;
//    float m_v(float const & arg) const;    
    float w_s(float const & arg) const;
//    float s_w(float const & arg) const;    
  private:
    ptrHandling< map2dTo3d > _m2d;
    dtVector3 _vv;
  };
}
#endif	/* ROTATINMAP2DTO3D_H */

