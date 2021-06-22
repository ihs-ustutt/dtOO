#include "map3dTo3d.h"

#include "map2dTo3d.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <solid3dLine.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <analyticGeometryHeaven/aGBuilder/uvw_map3dTo3dClosestPointToPoint.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dBiLinearTwoD.h>
#include "vec3dTwoDInMap3dTo3d.h"
#include "vec3dOneDInMap3dTo3d.h"
#include <discrete3dVector.h>

#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO { 
  float map3dTo3d::_deltaPer 
  = 
  staticPropertiesHandler::getInstance()->getOptionFloat("map3dTo3d_deltaPer");

  map3dTo3d::map3dTo3d() : analyticGeometry() {
    
  }

  map3dTo3d::map3dTo3d(const map3dTo3d& orig) : analyticGeometry(orig) {

  }

  map3dTo3d::~map3dTo3d() {
  }
  
  int map3dTo3d::dim( void ) const {
    return 3;
  }
    
  dtPoint3 map3dTo3d::getPoint( float const * const uvw ) const {
    return getPoint( uvw[0], uvw[1], uvw[2] );
  }
  
	vectorHandling< renderInterface * > map3dTo3d::getRender( void ) const {
		vectorHandling< renderInterface * > rV;
    //
    // calculate interval to create points
    //
    float intU = (getUMax() - getUMin()) / (getRenderResolutionU()-1);
    float intV = (getVMax() - getVMin()) / (getRenderResolutionV()-1);
    float intW = (getWMax() - getWMin()) / (getRenderResolutionW()-1);
		
		vectorHandling< dtPoint3 > pp;
    //
    // get grid
    //
		// v=0, w=0
		for(int ii=0; ii<getRenderResolutionU(); ii++) {
			float uu = (getUMin() + ii * intU);
			float vv = getVMin();// + jj * intV);
			float ww = getWMin();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.26, 0.10, 0.10) - getPointPercent(0.25, 0.10, 0.10), 
        getPointPercent(0.25, 0.10, 0.10)
      ) 
    );
		pp.clear();
		// u=0, w=0
		for(int ii=0; ii<getRenderResolutionV(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMin() + ii * intV;
			float ww = getWMin();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.10, 0.26, 0.10) - getPointPercent(0.10, 0.25, 0.10), 
        getPointPercent(0.10, 0.25, 0.10)
      ) 
    );    
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.10, 0.51, 0.10) - getPointPercent(0.10, 0.50, 0.10), 
        getPointPercent(0.10, 0.50, 0.10)
      ) 
    );        
		pp.clear();		
		// u=0, v=0
		for(int ii=0; ii<getRenderResolutionW(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMin();// + ii * intV;
			float ww = getWMin() + ii * intW;
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.10, 0.10, 0.26) - getPointPercent(0.10, 0.10, 0.25), 
        getPointPercent(0.10, 0.10, 0.25)
      ) 
    );    
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.10, 0.10, 0.51) - getPointPercent(0.10, 0.10, 0.50), 
        getPointPercent(0.10, 0.10, 0.50)
      ) 
    );    
		rV.push_back( 
      new discrete3dVector(
        getPointPercent(0.00, 0.00, 0.76) - getPointPercent(0.00, 0.00, 0.75), 
        getPointPercent(0.10, 0.10, 0.75)
      ) 
    );        
		pp.clear();				
		
    // u
    pp.push_back( getPointPercent(0.10, 0.10, 0.10) );
    pp.push_back( getPointPercent(0.25, 0.10, 0.10) );
    rV.push_back( new solid3dLine(pp) );
    pp.clear();
    // v
    pp.push_back( getPointPercent(0.10, 0.10, 0.10) );
    pp.push_back( getPointPercent(0.10, 0.25, 0.10) );
    pp.push_back( getPointPercent(0.10, 0.50, 0.10) );
    rV.push_back( new solid3dLine(pp) );
    pp.clear();
    // v
    pp.push_back( getPointPercent(0.10, 0.10, 0.10) );
    pp.push_back( getPointPercent(0.10, 0.10, 0.25) );
    pp.push_back( getPointPercent(0.10, 0.10, 0.50) );
    pp.push_back( getPointPercent(0.10, 0.10, 0.75) );
    rV.push_back( new solid3dLine(pp) );
    pp.clear();    
		// v=0, w=0
		for(int ii=0; ii<getRenderResolutionU(); ii++) {
			float uu = (getUMin() + ii * intU);
			float vv = getVMax();// + jj * intV);
			float ww = getWMax();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();
		// u=0, w=0
		for(int ii=0; ii<getRenderResolutionV(); ii++){
			float uu = getUMax();// + ii * intU);
			float vv = getVMin() + ii * intV;
			float ww = getWMax();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();		
		// u=0, v=0
		for(int ii=0; ii<getRenderResolutionW(); ii++){
			float uu = getUMax();// + ii * intU);
			float vv = getVMax();// + ii * intV;
			float ww = getWMin() + ii * intW;
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();			

		// v=0, w=0
		for(int ii=0; ii<getRenderResolutionU(); ii++){
			float uu = (getUMin() + ii * intU);
			float vv = getVMin();// + jj * intV);
			float ww = getWMax();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();
		// u=0, w=0
		for(int ii=0; ii<getRenderResolutionV(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMin() + ii * intV;
			float ww = getWMax();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();		
		// u=0, v=0
		for(int ii=0; ii<getRenderResolutionW(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMax();// + ii * intV;
			float ww = getWMin() + ii * intW;
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();			
		
		// v=0, w=0
		for(int ii=0; ii<getRenderResolutionU(); ii++){
			float uu = (getUMin() + ii * intU);
			float vv = getVMax();// + jj * intV);
			float ww = getWMin();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();
		// u=0, w=0
		for(int ii=0; ii<getRenderResolutionV(); ii++){
			float uu = getUMax();// + ii * intU);
			float vv = getVMin() + ii * intV;
			float ww = getWMin();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();		
		// u=0, v=0
		for(int ii=0; ii<getRenderResolutionW(); ii++){
			float uu = getUMax();// + ii * intU);
			float vv = getVMin();// + ii * intV;
			float ww = getWMin() + ii * intW;
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		pp.clear();					

    return rV;		
	}
	
  std::vector< dtVector3 > map3dTo3d::firstDer( 
    float const & uu, float const & vv, float const & ww
  ) const {
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float wP = percent_w(ww);
    
    std::vector< dtVector3 > ret(3);
    if (uP<_deltaPer) {
      ret[0] 
      = 
      (getPointPercent(_deltaPer, vP, wP) - getPointPercent(0., vP, wP))
      /
      (u_percent(_deltaPer) - u_percent(0.) );      
    }
    else if ( (uP>=_deltaPer) && (uP<=(1.-_deltaPer)) ) {
      ret[0]
      =
      ( 
          getPointPercent(uP+_deltaPer, vP, wP) 
        - getPointPercent(uP-_deltaPer, vP, wP) 
      )
      /
      ( u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) );
    }
    else if (uP>(1.-_deltaPer)) {
      ret[0]
      =
      (getPointPercent(1., vP, wP) - getPointPercent(1.-_deltaPer, vP, wP))
      /
      (u_percent(1.) - u_percent(1.-_deltaPer) );      
    }
    
    if (vP<_deltaPer) {
      ret[1]
      =
      (getPointPercent(uP, _deltaPer, wP) - getPointPercent(uP, 0., wP))
      /
      (v_percent(_deltaPer) - v_percent(0.) );      
    }
    else if ( (vP>=_deltaPer) && (vP<=(1.-_deltaPer)) ) {
      ret[1]
      =
      ( 
          getPointPercent(uP, vP+_deltaPer, wP) 
        - getPointPercent(uP, vP-_deltaPer, wP) 
      )
      /
      ( v_percent(vP+_deltaPer) - v_percent(vP-_deltaPer) );
    }
    else if (vP>(1.-_deltaPer)) {
      ret[1]
      =
      (getPointPercent(uP, 1., wP) - getPointPercent(uP, 1.-_deltaPer, wP))
      /
      (v_percent(1.) - v_percent(1.-_deltaPer) );      
    }

    if (wP<_deltaPer) {
      ret[2]
      =
      (getPointPercent(uP, vP, _deltaPer) - getPointPercent(uP, vP, 0.))
      /
      (w_percent(_deltaPer) - w_percent(0.) );
    }
    else if ( (wP>=_deltaPer) && (wP<=(1.-_deltaPer)) ) {
      ret[2]
      =
      ( 
          getPointPercent(uP, vP, wP+_deltaPer) 
        - getPointPercent(uP, vP, wP-_deltaPer) 
      )
      /
      ( w_percent(wP+_deltaPer) - w_percent(wP-_deltaPer) );
    }
    else if (wP>(1.-_deltaPer)) {
      ret[2]
      =
      (getPointPercent(uP, vP, 1.) - getPointPercent(uP, vP, 1.-_deltaPer))
      /
      (w_percent(1.) - w_percent(1.-_deltaPer));      
    }

    return ret;
  }
  
  dtVector3 map3dTo3d::firstDerU( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return firstDer(uu, vv, ww)[0];
  }

  dtVector3 map3dTo3d::firstDerV( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return firstDer(uu, vv, ww)[1];
  }

  dtVector3 map3dTo3d::firstDerW( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return firstDer(uu, vv, ww)[2];
  }
  
  dtPoint3 map3dTo3d::reparamInVolume(dtPoint3 const & ppXYZ) const {
		return reparamInVolume(ppXYZ, dtVector3(0,0,0));
	}
	
  dtPoint3 map3dTo3d::reparamPercentInVolume(dtPoint3 const & ppXYZ) const {
    dtPoint3 ppUVW = reparamInVolume(ppXYZ);
    
    return percent_uvw(ppUVW);    
	}

  dtPoint3 map3dTo3d::approxPercentInVolume(dtPoint3 const & ppXYZ) const {
    dtPoint3 ppUVW = approxPercentInVolume(ppXYZ);
    
    return percent_uvw(ppUVW);    
	}    
		 	
  int map3dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }

  int map3dTo3d::getRenderResolutionV( void ) const { 
    return analyticGeometry::getRenderResolution(1);
  }
  
  int map3dTo3d::getRenderResolutionW( void ) const { 
    return analyticGeometry::getRenderResolution(2);
  }
	
  bool map3dTo3d::isClosedU( void ) const {
    return isClosed(0);
  }
  
  bool map3dTo3d::isClosedV( void ) const {
    return isClosed(1);
  }
  
  bool map3dTo3d::isClosedW( void ) const {
    return isClosed(2);
  }
  
  float map3dTo3d::getUMin( void ) const {
    return getMin(0);
  }
  
  float map3dTo3d::getUMax( void ) const {
    return getMax(0);
  }
  
  float map3dTo3d::getVMin( void ) const {
    return getMin(1);
  }
  
  float map3dTo3d::getVMax( void ) const {
    return getMax(1);
  }
  
  float map3dTo3d::getWMin( void ) const {
    return getMin(2);
  }
  
  float map3dTo3d::getWMax( void ) const {
    return getMax(2);
  }

  bool map3dTo3d::inRange( dtPoint3 const & pUVW ) const {
    if ( pUVW.x() > getUMax() ) return false;
    if ( pUVW.x() < getUMin() ) return false;
    if ( pUVW.y() > getVMax() ) return false;
    if ( pUVW.y() < getVMin() ) return false;
    if ( pUVW.z() > getWMax() ) return false;
    if ( pUVW.z() < getWMin() ) return false;    
    return true;
  }
  
  dtPoint3 map3dTo3d::getPointPercent( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return getPoint( u_percent(uu), v_percent(vv), w_percent(ww) );
  }
  
  dtPoint3 map3dTo3d::getPoint( dtPoint3 const & ppUVW ) const {
		return getPoint(ppUVW.x(), ppUVW.y(), ppUVW.z());
	}
	
  dtPoint3 map3dTo3d::getPointPercent( dtPoint3 const & ppUVW ) const {
		return getPoint( uvw_percent(ppUVW) );
	}
		
  float map3dTo3d::u_percent(float const & uu) const {
    return (getUMin() +  (getUMax() - getUMin() ) * uu);    
  }
  
  float map3dTo3d::v_percent(float const & vv) const {
    return (getVMin() +  (getVMax() - getVMin() ) * vv);    
  }
  
  float map3dTo3d::w_percent(float const & ww) const {
    return (getWMin() +  (getWMax() - getWMin() ) * ww);    
  }
  
  float map3dTo3d::percent_u(float const & uu) const {
    return ( (uu - getUMin()) / (getUMax() - getUMin()) );
  }  

  float map3dTo3d::percent_v(float const & vv) const {
    return ( (vv - getVMin()) / (getVMax() - getVMin()) );
  }  

  float map3dTo3d::percent_w(float const & ww) const {
    return ( (ww - getWMin()) / (getWMax() - getWMin()) );
  }   
  
  dtPoint3 map3dTo3d::percent_uvw(dtPoint3 const & pUVW) const {
    return dtPoint3( 
      percent_u(pUVW.x()), percent_v(pUVW.y()), percent_w(pUVW.z()) 
    );
  }  
	
  dtPoint3 map3dTo3d::uvw_percent(dtPoint3 const & pp) const {
    return dtPoint3( u_percent(pp.x()), v_percent(pp.y()), w_percent(pp.z()) );
  }
	
	map1dTo3d * map3dTo3d::segment( 
    dtPoint3 const & p0, dtPoint3 const & p1 
  ) const {
		ptrHandling< dtCurve > dtC( 
		  trimmedCurve_twoPointsConnectConstructOCC(p0, p1).result() 
		);	
		vec3dCurveOneD v3d1d(dtC.get());		
		return new vec3dOneDInMap3dTo3d(&v3d1d, this);
	}	
	
	map2dTo3d * map3dTo3d::segment( 
    twoDArrayHandling< dtPoint3 > const & pp 
  ) const {
		ptrHandling< dtSurface > dtS( 
      bezierSurface_pointConstructOCC(pp).result() 
    );
		
		vec3dSurfaceTwoD v3d2d(dtS.get());
		
		return new vec3dTwoDInMap3dTo3d(&v3d2d, this);
	}

	map2dTo3d * map3dTo3d::segmentConstU( float const & uu ) const {
	  dtPoint3 p30(uu, getVMin(), getWMin());
		dtPoint3 p31(uu, getVMax(), getWMin());
		dtPoint3 p32(uu, getVMax(), getWMax());
		dtPoint3 p33(uu, getVMin(), getWMax());
		
		return segment(p30, p31, p32, p33);
	}

	map2dTo3d * map3dTo3d::segmentConstV( float const & vv ) const {
	  dtPoint3 p30(getUMin(), vv, getWMin());
		dtPoint3 p31(getUMax(), vv, getWMin());
		dtPoint3 p32(getUMax(), vv, getWMax());
		dtPoint3 p33(getUMin(), vv, getWMax());
		
		return segment(p30, p31, p32, p33);		
	}
  
	map2dTo3d * map3dTo3d::segmentConstW( float const & ww ) const {
	  dtPoint3 p30(getUMin(), getVMin(), ww);
		dtPoint3 p31(getUMax(), getVMin(), ww);
		dtPoint3 p32(getUMax(), getVMax(), ww);
		dtPoint3 p33(getUMin(), getVMax(), ww);
		
		return segment(p30, p31, p32, p33);		
	}
	
	/**
	 *   (w)
	 *    A
	 *    |
 	 *            (1)
	 *    +--------+
	 *    |        |
	 *    |        |
	 *    |        |
	 *    |        |
	 *    +--------+ --> (v)
	 *   (0)      
   */	
	map2dTo3d * map3dTo3d::segmentConstU( 
    float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  dtPoint3 p30(uu, p0.x(), p0.y());
		dtPoint3 p31(uu, p1.x(), p0.y());
		dtPoint3 p32(uu, p1.x(), p1.y());
		dtPoint3 p33(uu, p0.x(), p1.y());
		
		return segment(p30, p31, p32, p33);
	}

	/**
	 *   (w)
	 *    A
	 *    |
 	 *            (1)
	 *    +--------+
	 *    |        |
	 *    |        |
	 *    |        |
	 *    |        |
	 *    +--------+ --> (u)
	 *   (0)      
   */	  
	map2dTo3d * map3dTo3d::segmentConstV( 
    float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  dtPoint3 p30(p0.x(), vv, p0.y());
		dtPoint3 p31(p1.x(), vv, p0.y());
		dtPoint3 p32(p1.x(), vv, p1.y());
		dtPoint3 p33(p0.x(), vv, p1.y());
		
		return segment(p30, p31, p32, p33);		
	}
  
	/**
	 *   (v)
	 *    A
	 *    |
 	 *            (1)
	 *    +--------+
	 *    |        |
	 *    |        |
	 *    |        |
	 *    |        |
	 *    +--------+ --> (u)
	 *   (0)      
   */		
	map2dTo3d * map3dTo3d::segmentConstW( 
    float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  dtPoint3 p30(p0.x(), p0.y(), ww);
		dtPoint3 p31(p1.x(), p0.y(), ww);
		dtPoint3 p32(p1.x(), p1.y(), ww);
		dtPoint3 p33(p0.x(), p1.y(), ww);
		
		return segment(p30, p31, p32, p33);		
	}
	
	map2dTo3d * map3dTo3d::segmentConstUPercent( 
    float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  float const uP = u_percent(uu);
		dtPoint2 const p0P( v_percent(p0.x()), w_percent(p0.y()) );
		dtPoint2 const p1P( v_percent(p1.x()), w_percent(p1.y()) );
		
		return segmentConstU(uP, p0P, p1P);
	}

	map2dTo3d * map3dTo3d::segmentConstVPercent( 
    float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  float const vP = v_percent(vv);
		dtPoint2 const p0P( u_percent(p0.x()), u_percent(p0.y()) );
		dtPoint2 const p1P( w_percent(p1.x()), w_percent(p1.y()) );
		
		return segmentConstV(vP, p0P, p1P);
	}
  
	map2dTo3d * map3dTo3d::segmentConstWPercent( 
    float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
	  float const wP = w_percent(ww);
		dtPoint2 const p0P( u_percent(p0.x()), v_percent(p0.y()) );
		dtPoint2 const p1P( u_percent(p1.x()), v_percent(p1.y()) );
		
		return segmentConstW(wP, p0P, p1P);	
	}	

	map2dTo3d * map3dTo3d::segmentConstUPercent( float const & uu ) const {		
		return segmentConstU(u_percent(uu));
	}

	map2dTo3d * map3dTo3d::segmentConstVPercent( float const & vv ) const {
	  return segmentConstV(v_percent(vv));
	}
  
	map2dTo3d * map3dTo3d::segmentConstWPercent( float const & ww ) const {
	  return segmentConstW(w_percent(ww));
	}		
  
	/**
	 *   (v)
	 *    A
	 *    |
	 * 
 	 *   (3)      (2)
	 *    +--------+
	 *    |        |
	 *    |        |
	 *    |        |
	 *    |        |
	 *    +--------+ --> (u)
	 *   (0)      (1)
   */
	map2dTo3d * map3dTo3d::segment( 
		dtPoint3 const & p0, dtPoint3 const & p1, 
		dtPoint3 const & p2, dtPoint3 const & p3 
	) const {
    vec3dBiLinearTwoD v3d(p0, p1, p2, p3);
    return new vec3dTwoDInMap3dTo3d(&v3d, this);
	}
	
	map2dTo3d * map3dTo3d::segmentPercent( 
    twoDArrayHandling< dtPoint3 > const & pp 
  ) const {
		twoDArrayHandling< dtPoint3 > ppUVW(pp.size(0), pp.size(1));
		for ( int ii=0; ii<pp.size(0); ii++) {
			for ( int jj=0; jj<pp.size(1); jj++) {
        ppUVW[ii][jj] = uvw_percent(pp[ii][jj]);
			}
		}
		return segment(ppUVW);
	}
	
	map1dTo3d * map3dTo3d::segmentPercent( 
    dtPoint3 const & p0, dtPoint3 const & p1 
  ) const {
    return segment(uvw_percent(p0), uvw_percent(p1));
	}	
  
	map2dTo3d * map3dTo3d::segmentPercent( 
		dtPoint3 const & p0, dtPoint3 const & p1, 
		dtPoint3 const & p2, dtPoint3 const & p3 
	) const {
    return segment(
			uvw_percent(p0), uvw_percent(p1), uvw_percent(p2), uvw_percent(p3)
		);
	}	
			
  dtPoint3 map3dTo3d::reparamInVolume(
    dtPoint3 const & ppXYZ, dtVector3 const & uvwExtPercent
  ) const {
		double X = ppXYZ.x();
    double Y = ppXYZ.y();
    double Z = ppXYZ.z();
    double U;
    double V;
    double W;
//    int const NumInitGuess = 11;
//    double initU[NumInitGuess] 
//    = 
//    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
//    double initV[NumInitGuess] 
//    = 
//    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};        
//    double initW[NumInitGuess] 
//    = 
//    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};        
    int const NumInitGuess = 3;
    double initU[NumInitGuess] = {0.5, 0.75, 0.25};
    double initV[NumInitGuess] = {0.5, 0.75, 0.25};
    double initW[NumInitGuess] = {0.5, 0.75, 0.25};
    int maxRestarts 
    = 
    staticPropertiesHandler::getInstance()->getOptionInt("reparam_restarts");
    int maxInternalRestarts 
    = 
    staticPropertiesHandler
      ::getInstance()->getOptionInt("reparam_internalRestarts");    
    float restartIncreasePrec
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "reparam_restartIncreasePrecision"
    );
    float internalRestartDecreasePrec
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "reparam_internalRestartDecreasePrecision"
    );    
    float currentPrec = 1.;  
    float dist = 1.E+99;
    dt__forFromToIndex(0, maxRestarts+1, thisRun) {    
      dt__forFromToIndex(0, NumInitGuess, ii) {
        dt__forFromToIndex(0, NumInitGuess, jj) {       
          dt__forFromToIndex(0, NumInitGuess, kk) {   
            //
            // do reparameterization
            //
            U = initU[ii];
            V = initV[jj];
            W = initW[kk];

            dt__forFromToIndex(0, maxInternalRestarts, thisRestart) {
              double stepU = .01;
              double stepV = .01;
              double stepW = .01;
              double prec = 1.;
              double uMin = 0.;
              double uMax = 1.;
              double vMin = 0.;
              double vMax = 1.;            
              double wMin = 0.;
              double wMax = 1.;                        
              try {
                XYZtoUVWPercent(
                  X, Y, Z, 
                  U, V, W, uMin, uMax, vMin, vMax, wMin, wMax,
                  stepU, stepV, stepW, prec
                );
              }
              catch( eGeneral & eGen ) {
                dt__warning(
                  reparamInVolume(),
                  << eGen.what() << std::endl
                  << logMe::dtFormat(
                    "Error for initU = %12.4e initV = %12.4e initW = %12.4e "
                    "at internalRestart = %i"
                  ) 
                  % initU[ii] % initV[jj] % initW[kk] % thisRestart
                );
              }
              //
              // increase precision for restart
              //
              prec = internalRestartDecreasePrec * prec;

              //
              // check if point is precise enough
              //
              if (
                analyticGeometry::inXYZTolerance(
                  ppXYZ, getPointPercent(U, V, W), &dist, false, currentPrec
                )
              ) return uvw_percent( dtPoint3(U, V, W) );            
            }
          }
        }
      }
      dt__warning(
        reparamInVolume(), 
        << "Increasing reparamInVolume tolerance. Multiply inital precision by " 
        << restartIncreasePrec * currentPrec
      );        
      currentPrec = restartIncreasePrec * currentPrec;      
    }
    dt__throw(
      reparamInVolume(), 
      << "Reparameterization of " << dt__point3d(ppXYZ) << " fails." 
      << std::endl
      << dumpToString()
    );
  }
  
  dtPoint3 map3dTo3d::approxInVolume( dtPoint3 const & ppXYZ ) const {
    return uvw_map3dTo3dClosestPointToPoint(this, ppXYZ).result();
  }  
	
//	bool map3dTo3d::XYZtoUVW(
//		double X, double Y, double Z, 
//		double &U, double &V, double &W,
//		double relax, double extU, double extV, double extW,
//		std::vector< float > &itVal
//	) const {
//    double const Precision
//    =
//    static_cast<double>(
//      staticPropertiesHandler::getInstance()->getOptionFloat(
//        "reparamInVolume_precision"
//      )
//    );    
//    double const precXYZ
//    =
//    static_cast<double>(
//      staticPropertiesHandler::getInstance()->getOptionFloat(
//        "reparamInVolume_precisionXYZ"
//      )
//    );      		
//    const int MaxIter = 25;
//    const int NumInitGuess = 9;
//
//    double Unew = 0., Vnew = 0., Wnew = 0., err, err2;
//    int iter;
//    double umin, umax, vmin, vmax, wmin, wmax;
//    // don't use 0.9, 0.1 it fails with ruled surfaces
//    double initu[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};
//    double initv[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};
//    double initw[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};
//
//    
//    umax = static_cast<double>(getUMax());
//		umin = static_cast<double>(getUMin());
//		double udiff = extU;
//		umax = umax + 0.1*udiff;
//		umin = umin - 0.1*udiff;
//    vmin = static_cast<double>(getVMin());
//    vmax = static_cast<double>(getVMax());
//		double vdiff = extV;
//		vmax = vmax + 0.1*vdiff;		
//		vmin = vmin - 0.1*vdiff;
//    wmin = static_cast<double>(getWMin());
//    wmax = static_cast<double>(getWMax());
//		double wdiff = extW;
//		wmax = wmax + 0.1*wdiff;		
//		wmin = wmin - 0.1*wdiff;
//    
//    const double tol 
//    = 
//    Precision * (
//        (umax - umin)*(umax - umin) 
//      + (vmax - vmin)*(vmax - vmin) 
//      + (wmax - wmin)*(wmax - wmin)
//    );
//    for(int i = 0; i < NumInitGuess; i++) {
//      initu[i] = umin + initu[i] * (umax - umin);
//      initv[i] = vmin + initv[i] * (vmax - vmin);
//      initw[i] = wmin + initw[i] * (wmax - wmin);
//    }
//
//    for(int i = 0; i < NumInitGuess; i++){
//			U = initu[i];
//      for(int j = 0; j < NumInitGuess; j++){
//				V = initv[j];
//        for(int k = 0; k < NumInitGuess; k++){
//					W = initw[k];
//  				err = 1.0;
//					iter = 1;
//					
//          try { /* -------------------------------------------------------TRY */
//  					dtPoint3 P 
//						= 
//						getPoint(
//							static_cast<float>(U), 
//							static_cast<float>(V), 
//							static_cast<float>(W)
//						);
//						err2 
//            = 
//            sqrt(
//                (X - P.x())*(X - P.x()) 
//              + (Y - P.y())*(Y - P.y())
//              + (Z - P.z())*(Z - P.z())
//            );
//  //      if (err2 < 1.e-8 * CTX::instance()->lc) return;
//
//						while(err > tol && iter < MaxIter) {
//							P 
//							= 
//							getPoint(
//								static_cast<float>(U), 
//                static_cast<float>(V), 
//                static_cast<float>(W)
//							);
//							dtVector3 derU 
//							= 
//							firstDerU(
//								static_cast<float>(U), 
//                static_cast<float>(V), 
//                static_cast<float>(W)
//							);
//							dtVector3 derV 
//							= 
//							firstDerV(
//								static_cast<float>(U), 
//                static_cast<float>(V), 
//                static_cast<float>(W)
//							);
//							dtVector3 derW 
//							= 
//							firstDerW(
//								static_cast<float>(U), 
//                static_cast<float>(V), 
//                static_cast<float>(W)
//							);
//							dtMatrix mat(3,3);
//							mat(0,0) = derU.x(); mat(0,1) = derU.y(); mat(0,2) = derU.z();
//							mat(1,0) = derV.x(); mat(1,1) = derV.y(); mat(1,2) = derV.z();
//							mat(2,0) = derW.x(); mat(2,1) = derW.y(); mat(2,2) = derW.z();            
//							dtMatrix jac = dtLinearAlgebra::invertMatrix(mat);
//
//							Unew = U + relax *
//								(jac(0,0) * (X - P.x()) + jac(1,0) * (Y - P.y()) +
//								 jac(2,0) * (Z - P.z()));
//							Vnew = V + relax *
//								(jac(0,1) * (X - P.x()) + jac(1,1) * (Y - P.y()) +
//								 jac(2,1) * (Z - P.z()));
//							Wnew = W + relax *
//								(jac(0,2) * (X - P.x()) + jac(1,2) * (Y - P.y()) +
//								 jac(2,2) * (Z - P.z()));
//              
//							// don't remove this test: it is important
//							if((Unew > umax+tol || Unew < umin-tol) ||
//								 (Vnew > vmax+tol || Vnew < vmin-tol) ||
//								 (Wnew > wmax+tol || Wnew < wmin-tol)) {
//								break;
//							}
//
//              if ( isnan(Unew) ) break;
//              if ( isnan(Vnew) ) break;
//              if ( isnan(Wnew) ) break;
//              
//							err 
//              = 
//                (Unew - U)*(Unew - U) 
//              + (Vnew - V)*(Vnew - V) 
//              + (Wnew - W)*(Wnew - W);
//							err2 
//              = 
//              sqrt(
//                  (X - P.x())*(X - P.x()) 
//                + (Y - P.y())*(Y - P.y()) 
//                + (Z - P.z())*(Z - P.z())
//              );
//
//							iter++;
//							U = Unew;
//							V = Vnew;
//							W = Wnew;
//						}
//
//						itVal.push_back(static_cast<float>(i) );
//						itVal.push_back(static_cast<float>(j) );
//						itVal.push_back(static_cast<float>(k) );																	
//						itVal.push_back(static_cast<float>(err2) );
//						itVal.push_back(static_cast<float>(err) );
//						itVal.push_back(static_cast<float>(iter) );
//							
//					bool inRange = (Unew <= umax) && (Vnew <= vmax) && (Wnew <= wmax)
//					               && (Unew >= umin) && (Vnew >= vmin) && (Wnew >= wmin);
//					bool uvwConv = (err <= tol);
//					bool xyzConv = (err2 <= precXYZ);
//					
//					if( ( (iter<MaxIter) && inRange && xyzConv ) 
//						|| ( (iter<MaxIter) && inRange && uvwConv )  
//						) {
//							return true;
//						}
//					}     /* ---------------------------------------------------END TRY */
//					catch (eGeneral & eGenRef) {
//						itVal.push_back(static_cast<float>(i) );
//						itVal.push_back(static_cast<float>(j) );
//						itVal.push_back(static_cast<float>(k) );																								
//						itVal.push_back(static_cast<float>(err2) );
//						itVal.push_back(static_cast<float>(err) );
//						itVal.push_back(static_cast<float>(-iter) );						
//						dt__warning(
//						  XYZtoUVW(), 
//							<< "Break initial guess (" << i << ", " << j << ", " << k 
//							<< ") and try next one." << std::endl
//							<< eGenRef.what());
//					  break;
//					}    						
//				}
//      }
//    }
//
//    if(relax < 1.e-6) {
//      dt__info(XYZtoUVW(), << "Could not converge: surface mesh could be wrong");
//      return false;
//    }
//    else {
//      return XYZtoUVW(X, Y, Z, U, V, W, 0.75 * relax, extU, extV, extW, itVal);
//    }
//  }    

  bool map3dTo3d::XYZtoUVWPercent(
    double X, double Y, double Z, 
    double &U, double &V, double &W,
    double const uMin, double const uMax, 
    double const vMin, double const vMax, 
    double const wMin, double const wMax,     
    double const stepU, double const stepV, double const stepW, 
    double const prec    
  ) const {
    _pXYZ = dtPoint3(X, Y, Z);
		// 
		// multidimensional minimization
		//
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(
        staticPropertiesHandler::getInstance()->getOption(
          "reparamInVolume_minimizer"
        )
      )
    );    
		::ROOT::Math::Functor toMin(
			this, &map3dTo3d::F, 3 
		);			
		min->SetFunction(toMin);

		//
		// set bounds
		//
    min->SetVariable( 0, "U", U, stepU );
    min->SetVariableLimits(0, uMin, uMax);	
    min->SetVariable( 1, "V", V, stepV );
    min->SetVariableLimits(1, vMin, vMax);	
    min->SetVariable( 2, "W", W, stepW );
    min->SetVariableLimits(2, wMin, wMax);	
    
		//
		// minimizer options
		//
		min->SetMaxFunctionCalls(
      staticPropertiesHandler::getInstance()->getOptionInt(
        "root_maxFunctionCalls"
      )        
    );
		min->SetMaxIterations(
      staticPropertiesHandler::getInstance()->getOptionInt(
        "root_maxIterations"
      )      
    );
		min->SetTolerance(
      staticPropertiesHandler::getInstance()->getOptionFloat(
        "reparamInVolume_precision"
      ) * prec   
    );					
		min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt(
        "root_printLevel"
      ) 
    );

		//
		// minimize
		//
   	bool converged = min->Minimize();

    double const * const theRoot = min->X( );

    U = theRoot[0];//std::max<double>( std::min<double>( theRoot[0], getUMax() ), getUMin());
    V = theRoot[1];//std::max<double>( std::min<double>( theRoot[1], getVMax() ), getVMin());
    W = theRoot[2];//std::max<double>( std::min<double>( theRoot[2], getWMax() ), getWMin());
    
    return converged;
	}
  
	double map3dTo3d::F(double const * xx) const {	
    dtPoint3 uvw(xx[0], xx[1], xx[2]);
    double objective;
//    if ( inRange(uvw) ) {
      objective = dtLinearAlgebra::length( _pXYZ - getPointPercent(uvw) );
//    }
//    else dt__throwUnexpected(F());
    
    return objective;    
	}
  
	double map3dTo3d::FWrap(
    double const & x0, double const & x1, double const & x2 
  ) const {	
    std::vector< double > uvw(3);
    uvw[0] = x0;
    uvw[1] = x1;
    uvw[2] = x2;
    
    return F(&(uvw[0]));
	}	     
}
