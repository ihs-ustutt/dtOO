/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtSurface2d.h"
#include <logMe/dtMacros.h>

#include <logMe/logMe.h>
#include "dtCurve2d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <vector>

namespace dtOO {	
	dtSurface2d::dtSurface2d() {
		
	}

	dtSurface2d::~dtSurface2d() {
		
	}
	
	dtReal dtSurface2d::minU ( void ) const {
	  return minPara(0);	
	}
	
	dtReal dtSurface2d::maxU ( void ) const {
		return maxPara(0);
	}
	
	dtReal dtSurface2d::minV ( void ) const {
		return minPara(1);
	}
	
	dtReal dtSurface2d::maxV ( void ) const {
		return maxPara(1);
	}
	
	bool dtSurface2d::closedU( void ) const {
		return closed(0);
	}
	
	bool dtSurface2d::closedV( void ) const {
		return closed(1);
	}
	
	dtReal dtSurface2d::u_uPercent(dtReal const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	dtReal dtSurface2d::uPercent_u(dtReal const uu) const {
    dtReal uMin = minPara(0);
		dtReal uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}
	
	dtReal dtSurface2d::v_vPercent(dtReal const percent) const {
    return minPara(1) + percent * (maxPara(1)-minPara(1));
	}

	dtReal dtSurface2d::vPercent_v(dtReal const vv) const {
    dtReal vMin = minPara(1);
		dtReal vMax = maxPara(1);
		
		return (vv-vMin)/(vMax-vMin);
	}
	
	dtPoint2 dtSurface2d::point( dtPoint2 const & pUV) const {
		return point( pUV.x(), pUV.y() );
	}
	
	dtPoint2 dtSurface2d::pointPercent(dtReal const uP, dtReal const vP) const {
		return point(u_uPercent(uP), v_vPercent(vP));
	}
	
  dtVector2 dtSurface2d::firstDerU( dtReal const uu, dtReal const vv) const {
		std::vector<dtVector2> der = firstDer(uu, vv);
		return der[0];
	}
	
  dtVector2 dtSurface2d::firstDerV( dtReal const uu, dtReal const vv) const {
		std::vector<dtVector2> der = firstDer(uu, vv);
		return der[1];		
	}
	
  dtVector2 dtSurface2d::secondDerUU( dtReal const uu, dtReal const vv) const {
		std::vector<dtVector2> der = secondDer(uu, vv);
		return der[0];		
	}
	
  dtVector2 dtSurface2d::secondDerVV( dtReal const uu, dtReal const vv) const {
		std::vector<dtVector2> der = secondDer(uu, vv);
		return der[2];				
	}
	
  dtVector2 dtSurface2d::secondDerUV( dtReal const uu, dtReal const vv) const {
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
	
	dtCurve2d * dtSurface2d::segmentConstU( dtReal const uu) const {
		return segmentConstU( uu, minPara(1), maxPara(1) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstV( dtReal const vv) const {
		return segmentConstV( vv, minPara(0), maxPara(0) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstUPercent( dtReal const uu) const {
		return segmentConstU( u_uPercent(uu) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstVPercent( dtReal const vv) const {
		return segmentConstV( v_vPercent(vv) );
	}
	
	dtCurve2d * dtSurface2d::segmentConstUPercent( 
    dtReal const uu, dtReal const vvMin, dtReal const vvMax
  ) const {
		return segmentConstU( 
		  u_uPercent(uu), 
			v_vPercent(vvMin), 
			v_vPercent(vvMax) 
	  );
	}
	
	dtCurve2d * dtSurface2d::segmentConstVPercent( 
    dtReal const vv, dtReal const uuMin, dtReal const uuMax
  ) const {
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
  dtPoint2 dtSurface2d::controlPoint( dtInt const uI, dtInt const vI ) const {
		dt__throw(controlPoint(), <<"Not possible on this kind of surface.");
	}
	
  void dtSurface2d::setControlPoint( 
    dtInt const uI, dtInt const vI, dtPoint2 const point 
  ) {
		dt__throw(setControlPoint(), <<"Not possible on this kind of surface.");
	}
	
  dtInt dtSurface2d::nControlPoints( dtInt const dim ) const {
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
		
	void dtSurface2d::dump( void ) const {
    dt__info(
			dump(), 
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << std::endl
      << "vv = [ " <<  minPara(1) << ", " << maxPara(1) << "]" << std::endl
      << dt__eval(closed(0)) << std::endl
      << dt__eval(closed(1)) << std::endl      
			<< "dumpToString()" << std::endl
			<< dumpToString()      
		);
	}		
  
	std::string dtSurface2d::dumpToString(void) const {
		std::stringstream ss;
		
		ss << dt__dumpToString( << "" );
		
		return ss.str();
	}   
}
