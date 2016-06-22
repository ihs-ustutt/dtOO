#ifndef analyticRotatingMap1dTo3d_H
#define	analyticRotatingMap1dTo3d_H

#define _USE_MATH_DEFINES 
#include <math.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include "analyticSurface.h"
#include "splineCurve3d.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class splineCurve3d;

  class analyticRotatingMap1dTo3d : public analyticSurface {
    public:
      dt__class(analyticRotatingMap1dTo3d, analyticGeometry);
      analyticRotatingMap1dTo3d();
      analyticRotatingMap1dTo3d(
        dtSurface const & dtS, 
        dtPoint3 const & pp,
        dtVector3 const & vv,
        float const & angle = 2.*M_PI
      );
      analyticRotatingMap1dTo3d(
        dtSurface const & dtS, 
        dtVector3 const & vv,
        float const & angle = 2.*M_PI
      );      
      analyticRotatingMap1dTo3d( analyticRotatingMap1dTo3d const & orig );
      virtual analyticRotatingMap1dTo3d * clone( void ) const;
      virtual analyticRotatingMap1dTo3d * create( void ) const;        
      virtual ~analyticRotatingMap1dTo3d();
      dtPoint3 getOrigin( void ) const;
      dtVector3 getRotVector( void ) const;
      float getAngle( void ) const;
      // coordinate transformations
      virtual float u_phi(float const & arg) const;
      virtual float phi_u(float const & arg) const;
      virtual float v_m(float const & arg) const;
      virtual float m_v(float const & arg) const;
      virtual dtPoint3 xyz_phiZ(float const & arg0, float const & arg1) const;
      virtual dtPoint2 uv_phiZ(float const & arg0, float const & arg1) const;
      virtual dtPoint2 uv_deltaPhiRadiusDeltaM(
        float const & bU, float const & bV, 
        float const & arg0, float const & arg1
      ) const;      
      virtual dtPoint2 uv_phiRadiusM(
        float const & arg0, float const & arg1
      ) const;
      virtual dtPoint2 uv_phiM(float const & arg0, float const & arg1) const;
      virtual float zMin( void ) const;
      virtual float zMax( void ) const;
      virtual float z_v(float const & arg) const;
      virtual float v_z(float const & arg) const;
      virtual float r_v(float const & arg0) const;
      virtual float r_m(float const & arg0) const;
      void correctOrigin( void );
      virtual map2dTo3d * segmentRectangle(
        dtPoint2 const & p0, dtPoint2 const & p1
      ) const;      
    private:
      dtCurve const * ptrConstRadiusCurve( void ) const;
    private:
      dtPoint3 _pp;
      dtVector3 _vv;
      float _angle;
  };
}
#endif	/* analyticRotatingMap1dTo3d_H */

