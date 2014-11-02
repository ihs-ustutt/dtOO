#ifndef ROTATINGSPLINE_H
#define	ROTATINGSPLINE_H

#define _USE_MATH_DEFINES 
#include <math.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include "analyticSurface.h"
#include "splineCurve3d.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class splineCurve3d;

  class rotatingSpline : public analyticSurface {
    public:
      dt__CLASSSTD(rotatingSpline, analyticGeometry);
      rotatingSpline();
      rotatingSpline(
        dtSurface const & dtS, 
        dtPoint3 const & pp,
        dtVector3 const & vv,
        float const & angle = 2.*M_PI
      );
      rotatingSpline( rotatingSpline const & orig );
      virtual rotatingSpline * clone( void ) const;
      virtual rotatingSpline * create( void ) const;        
      virtual ~rotatingSpline();
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
      virtual dtPoint2 uv_deltaPhiRadiusDeltaM(float const & bU, float const & bV, float const & arg0, float const & arg1) const;      
      virtual dtPoint2 uv_phiRadiusM(float const & arg0, float const & arg1) const;
      virtual dtPoint2 uv_phiM(float const & arg0, float const & arg1) const;
      virtual float zMin( void ) const;
      virtual float zMax( void ) const;
      virtual float z_v(float const & arg) const;
      virtual float v_z(float const & arg) const;
      virtual float r_v(float const & arg0) const;
      virtual float r_m(float const & arg0) const;
      void correctOrigin( void );
      virtual map2dTo3d * pickPercent(
        float const & uu0, float const & uu1, 
        float const & vv0, float const & vv1
      ) const;        
    private:
      dtCurve const * ptrConstRadiusCurve( void ) const;
//      dtCurve * getCircleCurve( float const & vv ) const;      
    private:
      dtPoint3 _pp;
      dtVector3 _vv;
      float _angle;
  };
}
#endif	/* ROTATINGSPLINE_H */

