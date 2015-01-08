#include "map3dTo3d.h"

#include "map2dTo3d.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <solid3dLine.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <progHelper.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <functionHeaven/vec3dOneD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include "vec3dTwoDInMap3dTo3d.h"
#include "vec3dOneDInMap3dTo3d.h"
#include <discrete3dVector.h>

#define SQU(a)      ((a)*(a))

namespace dtOO { 
  map3dTo3d::map3dTo3d() : analyticGeometry() {
  }

  map3dTo3d::map3dTo3d(const map3dTo3d& orig) : analyticGeometry(orig) {
  }

  map3dTo3d::~map3dTo3d() {
  }
  
	vectorHandling< renderInterface * > map3dTo3d::getRender( void ) const {
		vectorHandling< renderInterface * > rV;
    //
    //calculate interval to create points
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
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]) );
		pp.clear();
		// u=0, w=0
		for(int ii=0; ii<getRenderResolutionV(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMin() + ii * intV;
			float ww = getWMin();// + kk * intW);
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]) );
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]+0.5*(pp[1]-pp[0])) );
		pp.clear();		
		// u=0, v=0
		for(int ii=0; ii<getRenderResolutionW(); ii++){
			float uu = getUMin();// + ii * intU);
			float vv = getVMin();// + ii * intV;
			float ww = getWMin() + ii * intW;
			pp.push_back( getPoint(uu, vv, ww) );
		}
		rV.push_back( new solid3dLine(pp) );
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]) );
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]+0.5*(pp[1]-pp[0])) );
		rV.push_back( new discrete3dVector(pp[1]-pp[0], pp[0]+1.0*(pp[1]-pp[0])) );
		pp.clear();				
		
		// v=0, w=0
		for(int ii=0; ii<getRenderResolutionU(); ii++){
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
	
  dtVector3 map3dTo3d::firstDerU( float const & uu, float const & vv, float const & ww) const {
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float wP = percent_w(ww);
    float const deltaPer = 0.01;
    
    if (uP<deltaPer) {
      return (
        (getPointPercent(deltaPer, vP, wP) - getPointPercent(0., vP, wP))
        /
        (u_percent(deltaPer) - u_percent(0.) )
      );      
    }
    else if ( (uP>=deltaPer) && (uP<=(1.-deltaPer)) ) {
      return (
        ( getPointPercent(uP+deltaPer, vP, wP) - getPointPercent(uP-deltaPer, vP, wP) )
        /
        ( u_percent(uP+deltaPer) - u_percent(uP-deltaPer) )
      );
    }
    else if (uP>(1.-deltaPer)) {
      return (
        (getPointPercent(1., vP, wP) - getPointPercent(1.-deltaPer, vP, wP))
        /
        (u_percent(1.) - u_percent(1.-deltaPer) )
      );      
    }
  }

  dtVector3 map3dTo3d::firstDerV( float const & uu, float const & vv, float const & ww) const {
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float wP = percent_w(ww);
    float const deltaPer = 0.01;
    
    if (vP<deltaPer) {
      return (
        (getPointPercent(uP, deltaPer, wP) - getPointPercent(uP, 0., wP))
        /
        (v_percent(deltaPer) - v_percent(0.) )
      );      
    }
    else if ( (vP>=deltaPer) && (vP<=(1.-deltaPer)) ) {
      return (
        ( getPointPercent(uP, vP+deltaPer, wP) - getPointPercent(uP, vP-deltaPer, wP) )
        /
        ( v_percent(vP+deltaPer) - v_percent(vP-deltaPer) )
      );
    }
    else if (vP>(1.-deltaPer)) {
      return (
        (getPointPercent(uP, 1., wP) - getPointPercent(uP, 1.-deltaPer, wP))
        /
        (v_percent(1.) - v_percent(1.-deltaPer) )
      );      
    }
  }  

  dtVector3 map3dTo3d::firstDerW( float const & uu, float const & vv, float const & ww) const {
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float wP = percent_w(ww);
    float const deltaPer = 0.01;
    
    if (wP<deltaPer) {
      return (
        (getPointPercent(uP, vP, deltaPer) - getPointPercent(uP, vP, 0.))
        /
        (w_percent(deltaPer) - w_percent(0.) )
      );      
    }
    else if ( (wP>=deltaPer) && (wP<=(1.-deltaPer)) ) {
      return (
        ( getPointPercent(uP, vP, wP+deltaPer) - getPointPercent(uP, vP, wP-deltaPer) )
        /
        ( w_percent(wP+deltaPer) - w_percent(wP-deltaPer) )
      );
    }
    else if (wP>(1.-deltaPer)) {
      return (
        (getPointPercent(uP, vP, 1.) - getPointPercent(uP, vP, 1.-deltaPer))
        /
        (w_percent(1.) - w_percent(1.-deltaPer) )
      );      
    }
  }  
  
  dtPoint3 map3dTo3d::reparamInVolume(dtPoint3 const & ppXYZ) const {
		double X = ppXYZ.x();
    double Y = ppXYZ.y();
    double Z = ppXYZ.z();
    double U;
    double V;
    double W;
    double relax = 1.;
    std::vector<float> itVal;  
    bool converged = XYZtoUVW(X, Y, Z, U, V, W, relax, itVal);
    
    std::vector< std::string > header;
		header.push_back("i");
		header.push_back("j");
		header.push_back("k");
    header.push_back("deltaXYZ");
    header.push_back("deltaUV");
		header.push_back("nIter");

    std::vector< std::string > addInfo;
		addInfo.push_back( "tolerance = ");
    addInfo[0] 
		+= 
		stringPrimitive::floatToString(
		  staticPropertiesHandler::getInstance()->getOptionFloat(
        "reparamInVolume_precision"
      )
		);  
		addInfo.push_back(
			"p_xyz = ("
			+ stringPrimitive::floatToString(ppXYZ.x()) 
			+ ", " 
			+ stringPrimitive::floatToString(ppXYZ.y())
			+ ", "
			+ stringPrimitive::floatToString(ppXYZ.z())
			+ ")"
		);

		dtPoint3 ppRep_xyz = getPoint(U,V,W);
		dtVector3 dist = ppXYZ - ppRep_xyz;
		float minPDist 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );    		
		if ( sqrt(dist.squared_length()) > minPDist) {
			DTDEBUGWF(
				reparamInVolume(), 
				<< logMe::floatVecToTable(addInfo, header, itVal) << LOGDEL 
				<< DTLOGEVAL(converged) << LOGDEL
				<< "p_xyz = (" << ppXYZ.x() << ", " << ppXYZ.y() << ", " << ppXYZ.z() << ")" << LOGDEL
				<< "V(p_uvw) = (" << ppRep_xyz.x() << ", " << ppRep_xyz.y() << ", " << ppRep_xyz.z() << ")" << LOGDEL
				<< "p_uvw = (" << U << ", " << V << ", " << W << ")" << LOGDEL
				<< "distance = (" << sqrt(dist.squared_length()) << ")"
			);
    }
    return dtPoint3(U, V, W);      
  }
  
  dtPoint3 map3dTo3d::reparamPercentInVolume(dtPoint3 const & ppXYZ) const {
    dtPoint3 ppUVW = reparamInVolume(ppXYZ);
    
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
  
  dtPoint3 map3dTo3d::getPointPercent( float const & uu, float const & vv, float const & ww ) const {
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
    return dtPoint3( percent_u(pUVW.x()), percent_v(pUVW.y()), percent_w(pUVW.z()) );
  }  
	
  dtPoint3 map3dTo3d::uvw_percent(dtPoint3 const & pp) const {
    return dtPoint3( u_percent(pp.x()), v_percent(pp.y()), w_percent(pp.z()) );
  }
	
	map1dTo3d * map3dTo3d::segment( dtPoint3 const & p0, dtPoint3 const & p1 ) const {
		ptrHandling< dtCurve > dtC( 
		  trimmedCurve_twoPointsConnectConstructOCC(p0, p1).result() 
		);	
		vec3dCurveOneD v3d1d(dtC.get());		
		return new vec3dOneDInMap3dTo3d(&v3d1d, this);
	}	
	
	map2dTo3d * map3dTo3d::segment( twoDArrayHandling< dtPoint3 > const & pp ) const {
		ptrHandling< dtSurface > dtS( bezierSurface_pointConstructOCC(pp).result() );
		
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
		dtPoint3 p31(getUMax(), getVMax(), ww);
		dtPoint3 p32(getUMax(), getVMax(), ww);
		dtPoint3 p33(getUMin(), getVMin(), ww);
		
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
	map2dTo3d * map3dTo3d::segmentConstU( float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
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
	map2dTo3d * map3dTo3d::segmentConstV( float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
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
	map2dTo3d * map3dTo3d::segmentConstW( float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
	  dtPoint3 p30(p0.x(), p0.y(), ww);
		dtPoint3 p31(p1.x(), p0.y(), ww);
		dtPoint3 p32(p1.x(), p1.y(), ww);
		dtPoint3 p33(p0.x(), p1.y(), ww);
		
		return segment(p30, p31, p32, p33);		
	}
	
	map2dTo3d * map3dTo3d::segmentConstUPercent( float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
	  float const uP = u_percent(uu);
		dtPoint2 const p0P( v_percent(p0.x()), w_percent(p0.y()) );
		dtPoint2 const p1P( v_percent(p1.x()), w_percent(p1.y()) );
		
		return segmentConstU(uP, p0P, p1P);
	}

	map2dTo3d * map3dTo3d::segmentConstVPercent( float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
	  float const vP = v_percent(vv);
		dtPoint2 const p0P( u_percent(p0.x()), u_percent(p0.y()) );
		dtPoint2 const p1P( w_percent(p1.x()), w_percent(p1.y()) );
		
		return segmentConstV(vP, p0P, p1P);
	}
  
	map2dTo3d * map3dTo3d::segmentConstWPercent( float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 ) const {
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
		twoDArrayHandling< dtPoint3 > pp(2,2);
		
		pp[0][0] = p0;
		pp[1][0] = p1;
		pp[0][1] = p3;
		pp[1][1] = p2;
		
		return segment(pp);
	}
	
	map2dTo3d * map3dTo3d::segmentPercent( twoDArrayHandling< dtPoint3 > const & pp ) const {
		twoDArrayHandling< dtPoint3 > ppUVW(pp.size(0), pp.size(1));
		for ( int ii=0; ii<pp.size(0); ii++) {
			for ( int jj=0; jj<pp.size(1); jj++) {
        ppUVW[ii][jj] = uvw_percent(pp[ii][jj]);
			}
		}
		return segment(ppUVW);
	}
	
	map1dTo3d * map3dTo3d::segmentPercent( dtPoint3 const & p0, dtPoint3 const & p1 ) const {
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
			
  bool map3dTo3d::XYZtoUVW(double X, double Y, double Z, double &U, double &V, double &W,
                          double relax, std::vector< float > &itVal ) const {
    double const Precision
    =
    static_cast<double>(
      staticPropertiesHandler::getInstance()->getOptionFloat(
        "reparamInVolume_precision"
      )
    );    
    double const precXYZ
    =
    static_cast<double>(
      staticPropertiesHandler::getInstance()->getOptionFloat(
        "reparamInVolume_precisionXYZ"
      )
    );      		
    const int MaxIter = 25;
    const int NumInitGuess = 9;

    double Unew = 0., Vnew = 0., Wnew = 0., err, err2;
    int iter;
    double umin, umax, vmin, vmax, wmin, wmax;
    // don't use 0.9, 0.1 it fails with ruled surfaces
    double initu[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};
    double initv[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};
    double initw[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 1.0, 0.0};

    
    umax = static_cast<double>(getUMax());
		umin = static_cast<double>(getUMin());
		double udiff = umax - umin;
		umax = umax + 0.1*udiff;
		umin = umin - 0.1*udiff;
    vmin = static_cast<double>(getVMin());
    vmax = static_cast<double>(getVMax());
		double vdiff = vmax - vmin;
		vmax = vmax + 0.1*vdiff;		
		vmin = vmin - 0.1*vdiff;
    wmin = static_cast<double>(getWMin());
    wmax = static_cast<double>(getWMax());
		double wdiff = wmax - wmin;
		wmax = wmax + 0.1*wdiff;		
		wmin = wmin - 0.1*wdiff;
    
    const double tol = Precision * (SQU(umax - umin) + SQU(vmax-vmin) + SQU(wmax-wmin));
    for(int i = 0; i < NumInitGuess; i++) {
      initu[i] = umin + initu[i] * (umax - umin);
      initv[i] = vmin + initv[i] * (vmax - vmin);
      initw[i] = wmin + initw[i] * (wmax - wmin);
    }

    for(int i = 0; i < NumInitGuess; i++){
			U = initu[i];
      for(int j = 0; j < NumInitGuess; j++){
				V = initv[j];
        for(int k = 0; k < NumInitGuess; k++){
					W = initw[k];
  				err = 1.0;
					iter = 1;
					
          try { /* -------------------------------------------------------TRY */
  					dtPoint3 P 
						= 
						getPoint(
							static_cast<float>(U), 
							static_cast<float>(V), 
							static_cast<float>(W)
						);
						err2 = sqrt(SQU(X - P.x()) + SQU(Y - P.y()) + SQU(Z - P.z()));
  //      if (err2 < 1.e-8 * CTX::instance()->lc) return;

						while(err > tol && iter < MaxIter) {
							P 
							= 
							getPoint(
								static_cast<float>(U), static_cast<float>(V), static_cast<float>(W)
							);
							dtVector3 derU 
							= 
							firstDerU(
								static_cast<float>(U), static_cast<float>(V), static_cast<float>(W)
							);
							dtVector3 derV 
							= 
							firstDerV(
								static_cast<float>(U), static_cast<float>(V), static_cast<float>(W)
							);
							dtVector3 derW 
							= 
							firstDerW(
								static_cast<float>(U), static_cast<float>(V), static_cast<float>(W)
							);
							dtMatrix mat(3,3);
							mat(0,0) = derU.x(); mat(0,1) = derU.y(); mat(0,2) = derU.z();
							mat(1,0) = derV.x(); mat(1,1) = derV.y(); mat(1,2) = derV.z();
							mat(2,0) = derW.x(); mat(2,1) = derW.y(); mat(2,2) = derW.z();            
							dtMatrix jac = dtLinearAlgebra::invertMatrix(mat);

							Unew = U + relax *
								(jac(0,0) * (X - P.x()) + jac(1,0) * (Y - P.y()) +
								 jac(2,0) * (Z - P.z()));
							Vnew = V + relax *
								(jac(0,1) * (X - P.x()) + jac(1,1) * (Y - P.y()) +
								 jac(2,1) * (Z - P.z()));
							Wnew = W + relax *
								(jac(0,2) * (X - P.x()) + jac(1,2) * (Y - P.y()) +
								 jac(2,2) * (Z - P.z()));

							// don't remove this test: it is important
							if((Unew > umax+tol || Unew < umin-tol) ||
								 (Vnew > vmax+tol || Vnew < vmin-tol) ||
								 (Wnew > wmax+tol || Wnew < wmin-tol)) {
								break;
							}

							err = SQU(Unew - U) + SQU(Vnew - V) + SQU(Wnew - W);
							err2 = sqrt(SQU(X - P.x()) + SQU(Y - P.y()) + SQU(Z - P.z()));

							iter++;
							U = Unew;
							V = Vnew;
							W = Wnew;
						}

						itVal.push_back(static_cast<float>(i) );
						itVal.push_back(static_cast<float>(j) );
						itVal.push_back(static_cast<float>(k) );																	
						itVal.push_back(static_cast<float>(err2) );
						itVal.push_back(static_cast<float>(err) );
						itVal.push_back(static_cast<float>(iter) );
							
					bool inRange = (Unew <= umax) && (Vnew <= vmax) && (Wnew <= wmax)
					               && (Unew >= umin) && (Vnew >= vmin) && (Wnew >= wmin);
					bool uvwConv = (err <= tol);
					bool xyzConv = (err2 <= precXYZ);
					
					if( ( (iter<MaxIter) && inRange && xyzConv ) 
//						|| ( (iter<MaxIter) && inRange && uvwConv )  
						) {
							return true;
						}
					}     /* ---------------------------------------------------END TRY */
					catch (eGeneral & eGenRef) {
						itVal.push_back(static_cast<float>(i) );
						itVal.push_back(static_cast<float>(j) );
						itVal.push_back(static_cast<float>(k) );																								
						itVal.push_back(static_cast<float>(err2) );
						itVal.push_back(static_cast<float>(err) );
						itVal.push_back(static_cast<float>(-iter) );						
						DTWARNINGWF(
						  XYZtoUVW(), 
							<< "Break initial guess (" << i << ", " << j << ", " << k 
							<< ") and try next one." << LOGDEL
							<< eGenRef.what());
					  break;
					}    						
				}
      }
    }

    if(relax < 1.e-6) {
      DTINFOWF(XYZtoUVW(), << "Could not converge: surface mesh could be wrong");
      return false;
    }
    else {
      return XYZtoUVW(X, Y, Z, U, V, W, 0.75 * relax, itVal);
    }
  }    
}
