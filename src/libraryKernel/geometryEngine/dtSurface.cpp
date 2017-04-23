#include "dtSurface.h"
#include <logMe/dtMacros.h>

#include <logMe/logMe.h>
#include "dtCurve.h"
#include "dtCurve2d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <vector>

#include "geoBuilder/uv_geomSurfaceClosestPoint.h"

namespace dtOO {	
	dtSurface::dtSurface() {
		
	}

	dtSurface::~dtSurface() {
		
	}
	
  dtPoint2 dtSurface::reparam(dtPoint3 const ppXYZ) const {
    return uv_geomSurfaceClosestPoint(this, ppXYZ).result();
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
    return 
      floatHandling::boundToRange(
        minPara(0) + percent * (maxPara(0)-minPara(0)),
        minPara(0), maxPara(0)
      );
	}

	float dtSurface::uPercent_u(float const uu) const {
		return 
      floatHandling::boundToRange(
        (uu-minPara(0))/(maxPara(0)-minPara(0)), 0, 1        
      );
	}
	
	float dtSurface::v_vPercent(float const percent) const {
    return
      floatHandling::boundToRange(
        minPara(1) + percent * (maxPara(1)-minPara(1)),
        minPara(1), maxPara(1)
      );
	}

	float dtSurface::vPercent_v(float const vv) const {
    return
      floatHandling::boundToRange(
        (vv-minPara(1))/(maxPara(1)-minPara(1)), 0, 1        
      );
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
		dt__forAllIndex(uu, ii) {
		  pp[ii] = point( uu[ii], vv[ii] );				
		}
		
		return pp;
	}
	
	std::vector< dtPoint3 > 
	dtSurface::getPointPercent3d( 
		std::vector< float > const & uu, 
		std::vector< float > const & vv
	) const {
		std::vector< dtPoint3 > pp( uu.size() );
		dt__forAllIndex(uu, ii) {
		  pp[ii] = point( u_uPercent(uu[ii]), v_vPercent(vv[ii]) );				
		}
		
		return pp;		
	} 
	
	std::vector< dtPoint3 > 
	dtSurface::getPoint3d( 
		std::vector< float > const * const uuV, 
		std::vector< float > const * const vvV
	) const {
		return getPoint3d(*uuV, *vvV);
	}
	
  dtVector3 dtSurface::firstDerU( float const uu, float const vv) const {
		std::vector<dtVector3> der = firstDer(uu, vv);
		return der[0];
	}
	
  dtVector3 dtSurface::firstDerV( float const uu, float const vv) const {
		std::vector<dtVector3> der = firstDer(uu, vv);
		return der[1];		
	}
	
  dtVector3 dtSurface::secondDerUU( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[0];		
	}
	
  dtVector3 dtSurface::secondDerVV( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[2];				
	}
	
  dtVector3 dtSurface::secondDerUV( float const uu, float const vv) const {
		std::vector<dtVector3> der = secondDer(uu, vv);
		return der[1];				
	}
	
	dtVector3 dtSurface::normalPercent( float const uP, float const vP ) const {
		return normal( u_uPercent(uP), v_vPercent(vP) );
	}
	
	dtPoint2 dtSurface::uv_uvPercent( dtPoint2 const uvPercent) const {
	  return dtPoint2( u_uPercent(uvPercent.x()), v_vPercent(uvPercent.y()) );
	}
	
	int dtSurface::nControlPointsU( void ) const {
		return nControlPoints(0);
	}
	
	int dtSurface::nControlPointsV( void ) const {
		return nControlPoints(1);
	}
	
	dtCurve * dtSurface::segmentConstU( float const uu) const {
		return segmentConstU( uu, minPara(1), maxPara(1) );
	}
	
	dtCurve * dtSurface::segmentConstV( float const vv) const {
		return segmentConstV( vv, minPara(0), maxPara(0) );
	}
	
	dtCurve * dtSurface::segmentConstUPercent( float const uu) const {
		return segmentConstU( u_uPercent(uu) );
	}
	
	dtCurve * dtSurface::segmentConstVPercent( float const vv) const {
		return segmentConstV( v_vPercent(vv) );
	}
	
	dtCurve * dtSurface::segmentConstUPercent( 
    float const uu, float const vvMin, float const vvMax
  ) const {
		return segmentConstU( 
		  u_uPercent(uu), 
			v_vPercent(vvMin), 
			v_vPercent(vvMax) 
	  );
	}
	
	dtCurve * dtSurface::segmentConstVPercent( 
    float const vv, float const uuMin, float const uuMax
  ) const {
		return segmentConstV( 
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
  dtPoint3 dtSurface::controlPoint( int const uI, int const vI ) const {
		dt__throw(controlPoint(), <<"Not possible on this kind of surface.");
	}
	
  void dtSurface::setControlPoint( 
    int const uI, int const vI, dtPoint3 const point 
  ) {
		dt__throw(setControlPoint(), <<"Not possible on this kind of surface.");
	}
	
  int dtSurface::nControlPoints( int const dim ) const {
		switch (dim) {
			case 0:
				return 0;
			case 1:
				return 0;
			default:
				dt__throw(
					nControlPoints(),
					<< dt__eval(dim) << std::endl
					<< "dim should be 0 or 1."
				);
		}
	}
		
	void dtSurface::dump(void) const {
    dt__info(
			dump(), 
		  << dt__eval(virtualClassName()) << std::endl
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << std::endl
      << "vv = [ " <<  minPara(1) << ", " << maxPara(1) << "]" << std::endl
      << dt__eval(closed(0)) << std::endl
      << dt__eval(closed(1)) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);
	}	
  
	std::string dtSurface::dumpToString(void) const {
		std::stringstream ss;
		
		ss << dt__dumpToString( << "" );
		
		return ss.str();
	}  
	
	void dtSurface::offsetNormal(float const nn) {
		dt__throwUnexpected(offsetNormal());
	}
  
  dtCurve * dtSurface::segmentConst( 
    int const & dir, float const at, float const from, float const to
  ) const {
    if      (dir==0) return segmentConstU(at, from, to);
    else if (dir==1) return segmentConstV(at, from, to);
    else dt__throwUnexpected(segmentConst());
  }
  
  dtCurve * dtSurface::segmentConst( 
    int const & dir, float const at
  ) const {
    if      (dir==0) return segmentConstU(at);
    else if (dir==1) return segmentConstV(at);
    else dt__throwUnexpected(segmentConst());    
  }
  
  dtCurve * dtSurface::segmentConstPercent( 
    int const & dir, float const at
  ) const {
    if      (dir==0) return segmentConstUPercent(at);
    else if (dir==1) return segmentConstVPercent(at);
    else dt__throwUnexpected(segmentConstPercent());        
  }
  
  dtCurve * dtSurface::segmentConstPercent( 
    int const & dir, float const at, float const from, float const to
  ) const {
    if      (dir==0) return segmentConstUPercent(at, from, to);
    else if (dir==1) return segmentConstVPercent(at, from, to);
    else dt__throwUnexpected(segmentConstPercent());            
  }
  
  dt__C_addCloneForpVH(dtSurface);
}
