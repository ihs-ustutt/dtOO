#ifndef analyticRotatingMap1dTo3d_H
#define	analyticRotatingMap1dTo3d_H

#include <dtOOTypeDef.h>

#define _USE_MATH_DEFINES 
#include <math.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include "analyticSurface.h"
#include "analyticCurve.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticCurve;

  class analyticRotatingMap1dTo3d : public analyticSurface {
    public:
      dt__class(analyticRotatingMap1dTo3d, analyticGeometry);
      analyticRotatingMap1dTo3d();
      analyticRotatingMap1dTo3d(
        dtSurface const & dtS, 
        dtPoint3 const & pp,
        dtVector3 const & vv,
        dtReal const & angle = 2.*M_PI
      );
      analyticRotatingMap1dTo3d(
        dtSurface const & dtS, 
        dtVector3 const & vv,
        dtReal const & angle = 2.*M_PI
      );      
      analyticRotatingMap1dTo3d( analyticRotatingMap1dTo3d const & orig );
      virtual analyticRotatingMap1dTo3d * clone( void ) const;
      virtual analyticRotatingMap1dTo3d * create( void ) const;        
      virtual ~analyticRotatingMap1dTo3d();
      dtPoint3 getOrigin( void ) const;
      dtVector3 getRotVector( void ) const;
      dtReal getAngle( void ) const;
      // coordinate transformations
      virtual dtReal u_phi(dtReal const & arg) const;
      virtual dtReal phi_u(dtReal const & arg) const;
      virtual dtReal v_m(dtReal const & arg) const;
      virtual dtReal m_v(dtReal const & arg) const;
      virtual dtPoint3 xyz_phiZ(dtReal const & arg0, dtReal const & arg1) const;
      virtual dtPoint2 uv_phiZ(dtReal const & arg0, dtReal const & arg1) const;
      virtual dtPoint2 uv_deltaPhiRadiusDeltaM(
        dtReal const & bU, dtReal const & bV, 
        dtReal const & arg0, dtReal const & arg1
      ) const;      
      virtual dtPoint2 uv_phiRadiusM(
        dtReal const & arg0, dtReal const & arg1
      ) const;
      virtual dtPoint2 uv_phiM(dtReal const & arg0, dtReal const & arg1) const;
      virtual dtReal zMin( void ) const;
      virtual dtReal zMax( void ) const;
      virtual dtReal z_v(dtReal const & arg) const;
      virtual dtReal v_z(dtReal const & arg) const;
      virtual dtReal r_v(dtReal const & arg0) const;
      virtual dtReal r_m(dtReal const & arg0) const;
      void correctOrigin( void );
      virtual map2dTo3d * segmentRectangle(
        dtPoint2 const & p0, dtPoint2 const & p1
      ) const;      
    private:
      dtCurve const * ptrConstRadiusCurve( void ) const;
    private:
      dtPoint3 _pp;
      dtVector3 _vv;
      dtReal _angle;
  };
}
#endif	/* analyticRotatingMap1dTo3d_H */

