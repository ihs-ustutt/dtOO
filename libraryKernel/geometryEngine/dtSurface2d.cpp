#include "dtSurface2d.h"
#include <progHelper.h>

#include <logMe/logMe.h>
#include "dtCurve2d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <vector>

namespace dtOO {	
	dtSurface2d::dtSurface2d() {
		
	}

	dtSurface2d::~dtSurface2d() {
		
	}
	
	float dtSurface2d::minU ( void ) const {
	  return minPara(0);	
	}
	
	float dtSurface2d::maxU ( void ) const {
		return maxPara(0);
	}
	
	float dtSurface2d::minV ( void ) const {
		return minPara(1);
	}
	
	float dtSurface2d::maxV ( void ) const {
		return maxPara(1);
	}
	
	bool dtSurface2d::closedU( void ) const {
		return closed(0);
	}
	
	bool dtSurface2d::closedV( void ) const {
		return closed(1);
	}
	
	float dtSurface2d::u_uPercent(float const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	float dtSurface2d::uPercent_u(float const uu) const {
    float uMin = minPara(0);
		float uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}
	
	float dtSurface2d::v_vPercent(float const percent) const {
    return minPara(1) + percent * (maxPara(1)-minPara(1));
	}

	float dtSurface2d::vPercent_v(float const vv) const {
    float vMin = minPara(1);
		float vMax = maxPara(1);
		
		return (vv-vMin)/(vMax-vMin);
	}
	
	dtPoint2 dtSurface2d::point( dtPoint2 const & pUV) const {
		return point( pUV.x(), pUV.y() );
	}
	
	dtPoint2 dtSurface2d::pointPercent(float const uP, float const vP) const {
		return point(u_uPercent(uP), v_vPercent(vP));
	}
	
  dtVector2 dtSurface2d::firstDerU( float const uu, float const vv) const {
		std::vector<dtVector2> der = firstDer(uu, vv);
		return der[0];
	}
	
  dtVector2 dtSurface2d::firstDerV( float const uu, float const vv) const {
		std::vector<dtVector2> der = firstDer(uu, vv);
		return der[1];		
	}
	
  dtVector2 dtSurface2d::secondDerUU( float const uu, float const vv) const {
		std::vector<dtVector2> der = secondDer(uu, vv);
		return der[0];		
	}
	
  dtVector2 dtSurface2d::secondDerVV( float const uu, float const vv) const {
		std::vector<dtVector2> der = secondDer(uu, vv);
		return der[2];				
	}
	
  dtVector2 dtSurface2d::secondDerUV( float const uu, float const vv) const {
		std::vector<dtVector2> der = secondDer(uu, vv);
		return der[1];				
	}
	
	dtPoint2 dtSurface2d::uv_uvPercent( dtPoint2 const uvPercent) const {
	  return dtPoint2( u_uPercent(uvPercent.x()), v_vPercent(uvPercent.y()) );
	}
	
	int dtSurface2d::nControlPointsU( void ) const {
		return nControlPoints(0);
	}
	
	int dtSurface2d::nControlPointsV( void ) const {
		return nControlPoints(1);
	}
	
	dtCurve2d * dtSurface2d::segmentConstU( float const uu) const {
		return segmentConstU( uu, minPara(1), maxPara(1) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstV( float const vv) const {
		return segmentConstV( vv, minPara(0), maxPara(0) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstUPercent( float const uu) const {
		return segmentConstU( u_uPercent(uu) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstVPercent( float const vv) const {
		return segmentConstV( v_vPercent(vv) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstUPercent( float const uu, float const vvMin, float const vvMax) const {
		return segmentConstU( 
		  u_uPercent(uu), 
			v_vPercent(vvMin), 
			v_vPercent(vvMax) 
	  );
	}
	
	dtCurve2d * dtSurface2d::segmentConstVPercent( float const vv, float const uuMin, float const uuMax) const {
		return segmentConstV( 
		  v_vPercent(vv), 
			u_uPercent(uuMin),
			u_uPercent(uuMax)
	  );
	}
	
	dtPoint2 dtSurface2d::reparamPercent(dtPoint2 const point) const {
		dtPoint2 ppUV = reparam(point);
		return dtPoint2( uPercent_u(ppUV.x()), vPercent_v(ppUV.y()) );
	}
	
	//
	//
	//
  dtPoint2 dtSurface2d::controlPoint( int const uI, int const vI ) const {
		dt__THROW(controlPoint(), <<"Not possible on this kind of surface.");
	}
	
  void dtSurface2d::setControlPoint( int const uI, int const vI, dtPoint2 const point ) {
		dt__THROW(setControlPoint(), <<"Not possible on this kind of surface.");
	}
	
  int dtSurface2d::nControlPoints( int const dim ) const {
		switch (dim) {
			case 0:
				return 0;
			case 1:
				return 0;
			default:
				dt__THROW(
					nControlPoints(),
					<< DTLOGEVAL(dim) << LOGDEL
					<< "dim should be 0 or 1."
				);
		}
	}
		
	void dtSurface2d::dump( void ) const {
    DTINFOWF(
			dump(), 
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << LOGDEL
      << "vv = [ " <<  minPara(1) << ", " << maxPara(1) << "]" 
		);
	}		
}
