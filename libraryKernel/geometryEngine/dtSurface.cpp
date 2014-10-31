#include "dtSurface.h"
#include <progHelper.h>

#include <logMe/logMe.h>
#include "dtCurve.h"
#include "dtCurve2d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <vector>

namespace dtOO {	
	dtSurface::dtSurface() {
		
	}

	dtSurface::~dtSurface() {
		
	}
	
	float dtSurface::minU ( void ) const {
	  return minPara(0);	
	}
	
	float dtSurface::maxU ( void ) const {
		return maxPara(0);
	}
	
	float dtSurface::minV ( void ) const {
		return minPara(1);
	}
	
	float dtSurface::maxV ( void ) const {
		return maxPara(1);
	}
	
	bool dtSurface::closedU( void ) const {
		return closed(0);
	}
	
	bool dtSurface::closedV( void ) const {
		return closed(1);
	}
	
	float dtSurface::u_uPercent(float const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	float dtSurface::uPercent_u(float const uu) const {
    float uMin = minPara(0);
		float uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}
	
	float dtSurface::v_vPercent(float const percent) const {
    return minPara(1) + percent * (maxPara(1)-minPara(1));
	}

	float dtSurface::vPercent_v(float const vv) const {
    float vMin = minPara(1);
		float vMax = maxPara(1);
		
		return (vv-vMin)/(vMax-vMin);
	}
	
	dtPoint3 dtSurface::getPoint3d( dtPoint2 const & pUV) const {
		return point( pUV.x(), pUV.y() );
	}
	
	dtPoint3 dtSurface::getPointPercent3d(float const uP, float const vP) const {
		return point(u_uPercent(uP), v_vPercent(vP));
	}
	
	std::vector< dtPoint3 > 
	dtSurface::getPoint3d( 
		std::vector< float > const & uu, 
		std::vector< float > const & vv
	) const {
		std::vector< dtPoint3 > pp( uu.size() );
		dt__FORALL(uu, ii,
		  pp[ii] = point( uu[ii], vv[ii] );				
		);
		
		return pp;
	}
	
	std::vector< dtPoint3 > 
	dtSurface::getPointPercent3d( 
		std::vector< float > const & uu, 
		std::vector< float > const & vv
	) const {
		std::vector< dtPoint3 > pp( uu.size() );
		dt__FORALL(uu, ii,
		  pp[ii] = point( u_uPercent(uu[ii]), v_vPercent(vv[ii]) );				
		);
		
		return pp;		
	} 
	
	std::vector< dtPoint3 > 
	dtSurface::getPoint3d( 
		std::vector< float > const * const uuV, 
		std::vector< float > const * const vvV
	) const {
		return getPoint3d(*uuV, *vvV);
	}
	
  dtVector3 dtSurface::getVectorDdU3d( float const uu, float const vv) const {
		std::vector<dtVector3> der = firstDer(uu, vv);
		return der[0];
	}
	
  dtVector3 dtSurface::getVectorDdV3d( float const uu, float const vv) const {
		std::vector<dtVector3> der = firstDer(uu, vv);
		return der[1];		
	}
	
  dtVector3 dtSurface::getVectorDdUDdU3d( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[0];		
	}
	
  dtVector3 dtSurface::getVectorDdVDdV3d( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[2];				
	}
	
  dtVector3 dtSurface::getVectorDdUDdV3d( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[1];				
	}
	
	dtVector3 dtSurface::normalPercent( float const uP, float const vP ) const {
		return normal( u_uPercent(uP), v_vPercent(vP) );
	}
	
	dtPoint2 dtSurface::uv_uvPercent( dtPoint2 const uvPercent) const {
	  return dtPoint2( u_uPercent(uvPercent.x()), v_vPercent(uvPercent.y()) );
	}
		
	dtPoint3 dtSurface::controlPoint( int const nPoint ) const {
		int nn = nPoint;
	  int nV = nn/nControlPoints(0);
		int nU = nn - nV * nControlPoints(0);
		
		return controlPoint(nU, nV);
		
	}
	
	int dtSurface::nControlPoints( void ) const {
		return nControlPoints(0)*nControlPoints(1);
	}
	
	void dtSurface::setControlPoint( int const nPoint, dtPoint3 const point ) {
		int nn = nPoint;
	  int nV = nn/nControlPoints(0);
		int nU = nn - nV * nControlPoints(0);
		
    setControlPoint(nU, nV, point);		
	}
	
	int dtSurface::getNControlPointsU( void ) const {
		return nControlPoints(0);
	}
	
	int dtSurface::getNControlPointsV( void ) const {
		return nControlPoints(1);
	}
	
	dtCurve * dtSurface::getCurveConstU( float const uu) const {
		return getCurveConstU( uu, minPara(1), maxPara(1) );
	}
	
	dtCurve * dtSurface::getCurveConstV( float const vv) const {
		return getCurveConstV( vv, minPara(0), maxPara(0) );
	}
	
	dtCurve * dtSurface::getCurveConstUPercent( float const uu) const {
		return getCurveConstU( u_uPercent(uu) );
	}
	
	dtCurve * dtSurface::getCurveConstVPercent( float const vv) const {
		return getCurveConstV( v_vPercent(vv) );
	}
	
	dtCurve * dtSurface::getCurveConstUPercent( float const uu, float const vvMin, float const vvMax) const {
		return getCurveConstU( 
		  u_uPercent(uu), 
			v_vPercent(vvMin), 
			v_vPercent(vvMax) 
	  );
	}
	
	dtCurve * dtSurface::getCurveConstVPercent( float const vv, float const uuMin, float const uuMax) const {
		return getCurveConstV( 
		  v_vPercent(vv), 
			u_uPercent(uuMin),
			u_uPercent(uuMax)
	  );
	}
	
	dtPoint2 dtSurface::reparamPercent(dtPoint3 const point) const {
		dtPoint2 ppUV = reparam(point);
		return dtPoint2( uPercent_u(ppUV.x()), vPercent_v(ppUV.y()) );
	}
	//
	//
	//
	void dtSurface::dump( void ) const {
    DTINFOWF(
			dump(), 
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << LOGDEL
      << "vv = [ " <<  minPara(1) << ", " << maxPara(1) << "]" 
		);
	}	
	
	void dtSurface::revert(void) {
		DTFUNCTIONNOTI(revert());
	}
	
	void dtSurface::offsetNormal(float const nn) {
		DTFUNCTIONNOTI(offsetNormal());
	}
	
}
