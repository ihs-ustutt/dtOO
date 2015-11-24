#include "map2dTo3d.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include "map1dTo3d.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <progHelper.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid3dSurface.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dBiLinearTwoD.h>
#include "vec2dOneDInMap2dTo3d.h"
#include "vec2dTwoDInMap2dTo3d.h"

#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {  
  map2dTo3d::map2dTo3d() : analyticGeometry() {
  }

  map2dTo3d::map2dTo3d(const map2dTo3d& orig) : analyticGeometry(orig) {
  }

  map2dTo3d::~map2dTo3d() {
  }
  
  dtPoint3 map2dTo3d::getPoint( dtPoint2 const & pUV ) const {
    return getPoint(pUV.x(), pUV.y());
  }

  dtPoint3 map2dTo3d::getPoint( dtPoint2 const * const pUV ) const {
    return getPoint(*pUV);
  }  
  
  bool map2dTo3d::isClosedU( void ) const {
    return isClosed(0);
  }
  
  bool map2dTo3d::isClosedV( void ) const {
    return isClosed(1);
  }
  
  float map2dTo3d::getUMin( void ) const {
    return getMin(0);
  }
  
  float map2dTo3d::getUMax( void ) const {
    return getMax(0);
  }
  
  float map2dTo3d::getVMin( void ) const {
    return getMin(1);
  }
  
  float map2dTo3d::getVMax( void ) const {
    return getMax(1);
  }
  
  dtPoint3 map2dTo3d::getPointPercent( 
    float const & uu, float const & vv 
  ) const {
    return getPoint( u_percent(uu), v_percent(vv) );
  }    
	
  dtPoint3 map2dTo3d::getPointPercent( dtPoint2 const & pUV ) const {
    return getPointPercent( pUV.x(), pUV.y() );
  }    
	
	vectorHandling< renderInterface * > map2dTo3d::getRender( void ) const {
		int renderResU = analyticGeometry::getRenderResolution(0);
		int renderResV = analyticGeometry::getRenderResolution(1);		
    
    //
    //calculate interval to create points
    //
    float intervalFirst = (getUMax() - getUMin()) / (renderResU-1.);
    float intervalSecond = (getVMax() - getVMin()) / (renderResV-1.);  

    twoDArrayHandling< dtPoint3 > surfacePoints(renderResU, renderResV);
    for (int jj=0; jj<renderResU; jj++) {
      for(int ii=0; ii<renderResV; ii++) {
				float uu;
				float vv;				
        if (jj == (renderResU-1) ) {
          uu = getUMax();
        }
        else {
          uu = (float) (getUMin() + jj * intervalFirst);
        }
        if (ii == (renderResV-1) ) {      
          vv = getVMax();
        }
        else {
          vv = (float) (getVMin() + ii * intervalSecond);
        }
        surfacePoints[jj][ii] = getPoint(uu, vv);
      }
    }
		
		vectorHandling< renderInterface * > rV;
		rV.push_back( new solid3dSurface(surfacePoints) );
		
		return rV;
	}
	
  vectorHandling< renderInterface * > map2dTo3d::getExtRender( void ) const {
		vectorHandling< dtVector3 > vecV;
		vectorHandling< dtPoint3 > vecP;
		vectorHandling< dtVector3 > norV;
		vectorHandling< dtPoint3 > norP;
		
    //
    // get surface directions
    //
    dtPoint3 startPointU = map2dTo3d::getPointPercent(0.05, 0.);
    dtPoint3 topPointU = map2dTo3d::getPointPercent(0.1, 0.);
    dtPoint3 startPointV = map2dTo3d::getPointPercent(0., 0.05);
    dtPoint3 topPointV = map2dTo3d::getPointPercent(0., 0.10);
    dtVector3 uu = topPointU - startPointU;  
    dtVector3 vv = topPointV - startPointV;  

    //
    // add direction to vector container
    //
		vecV.push_back(uu); vecP.push_back(startPointU); 
		vecV.push_back(vv); vecP.push_back(startPointV);
		vecV.push_back(vv); vecP.push_back(topPointV);
		norV.push_back(map2dTo3d::normalPercent(.25, .25)); 
		norP.push_back(map2dTo3d::getPointPercent(.25, .25));
		norV.push_back(map2dTo3d::normalPercent(.25, .75)); 
		norP.push_back(map2dTo3d::getPointPercent(.25, .75));		
		norV.push_back(map2dTo3d::normalPercent(.75, .25)); 
		norP.push_back(map2dTo3d::getPointPercent(.75, .25));		
		norV.push_back(map2dTo3d::normalPercent(.75, .75)); 
		norP.push_back(map2dTo3d::getPointPercent(.75, .75));		
		
		vectorHandling< renderInterface * > retVec(2);
		retVec[0] = new discrete3dVector(vecV, vecP);
		retVec[1] = new discrete3dVector(norV, norP);		
		return retVec;
  }
   
  int map2dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }

  int map2dTo3d::getRenderResolutionV( void ) const { 
    return analyticGeometry::getRenderResolution(1);
  }  
  
  dtVector3 map2dTo3d::getPointPercentVector( 
    float const & uu, float const & vv 
  ) const {
    return dtLinearAlgebra::toDtVector3( getPointPercent(uu, vv) );
  }
  
  dtVector3 map2dTo3d::normal( float const & uu, float const & vv) const {
    dtVector3 vec(
      dtLinearAlgebra::crossProduct(
        firstDerU(uu, vv), firstDerV(uu, vv)
      )
    );
    return ( 1./sqrt(vec.squared_length()) ) * vec;
  }
	
	dtVector3 map2dTo3d::normal( dtPoint2 const & pp ) const {
		return normal(pp.x(), pp.y());
	}
  
  dtPoint2 map2dTo3d::reparamOnFace(dtPoint3 const & ppXYZ) const {
    double X = ppXYZ.x();
    double Y = ppXYZ.y();
    double Z = ppXYZ.z();
    double U;
    double V;
    int const NumInitGuess = 11;
    double initU[NumInitGuess] 
    = 
    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
    double initV[NumInitGuess] 
    = 
    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};    
    int maxRestarts 
    = 
    staticPropertiesHandler::getInstance()->getOptionInt("reparam_restarts");
    int restartIncreasePrec
    = 
    staticPropertiesHandler::getInstance()->getOptionInt(
      "reparam_restartIncreasePrecision"
    );
    float currentPrec = 1.;
    dt__forFromToIndex(0, maxRestarts+1, thisRun) {
      dt__forFromToIndex(0, NumInitGuess, ii) {
        dt__forFromToIndex(0, NumInitGuess, jj) {       
          //
          // do reparameterization
          //
          U = initU[ii];
          V = initV[jj];
          XYZtoUVPercent(X, Y, Z, U, V);

          //
          // check if point is precise enough
          //
          if (
            analyticGeometry::inXYZTolerance(
              ppXYZ, getPointPercent(U,V), false, currentPrec
            )
          ) {
            analyticGeometry::inXYZTolerance(
              ppXYZ, getPointPercent(U, V), true, currentPrec
            );
            return uv_percent( dtPoint2(U, V) );            
          }
        }
      }
      dt__warning(
        reparamOnFace(), 
        << "Increasing reparamOnFace tolerance. Multiply inital precision by:" 
        << std::endl
        << restartIncreasePrec * currentPrec
      );        
      currentPrec = restartIncreasePrec * currentPrec;
    }
    dt__throw(
      reparamOnFace(), 
      << "Reparametization fails." << std::endl
      << dt__point3d(ppXYZ) << std::endl
      << dumpToString()
    );    
  }
  
  dtVector3 map2dTo3d::normalPercent( 
    float const & uu, float const & vv 
  ) const {
    return normal(u_percent(uu), v_percent(vv));  
  }
	
	std::vector< dtVector3 > map2dTo3d::firstDer( 
    float const & uu, float const & vv
  ) const {
		//
		// dU
		//
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float const deltaPer = 0.01;
    float const deltaPerInv = 1. - deltaPer;
		std::vector< dtVector3 > dd(2);
    
    if (uP<deltaPer) {
      dd[0] 
			= 
			(getPointPercent(deltaPer, vP) - getPointPercent(0., vP))
			/
			(u_percent(deltaPer) - u_percent(0.) );      
    }
    else if ( (uP>=deltaPer) && (uP<=deltaPerInv) ) {
      dd[0] 
			= 
			( getPointPercent(uP+deltaPer, vP) - getPointPercent(uP-deltaPer, vP) )
			/
			( u_percent(uP+deltaPer) - u_percent(uP-deltaPer) );
    }
    else if (uP>deltaPerInv) {
      dd[0] 
			= 
			(getPointPercent(1., vP) - getPointPercent(1.-deltaPer, vP))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );    
    }
		
		//
		// dV
		//
    if (vP<deltaPer) {
      dd[1] 
			= 
			(getPointPercent(uP, deltaPer) - getPointPercent(uP, 0.))
			/
			(v_percent(deltaPer) - v_percent(0.) );
    }
    else if ( (vP>=deltaPer) && (vP<=deltaPerInv) ) {
      dd[1] 
			= 
			(getPointPercent(uP, vP+deltaPer) - getPointPercent(uP, vP-deltaPer))
			/
			(v_percent(vP+deltaPer) - v_percent(vP-deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[1] 
			= 
			(getPointPercent(uP, 1.) - getPointPercent(uP, 1.-deltaPer))
			/
			(v_percent(1.) - v_percent(deltaPerInv) );      
    }
		
		return dd;
	}
  
  std::vector< dtVector3 > map2dTo3d::firstDer( dtPoint2 const & pp ) const {
		return firstDer(pp.x(), pp.y());
	}
	
  dtVector3 map2dTo3d::firstDerU( float const & uu, float const & vv) const {
		return firstDer(uu, vv)[0];
  }
  
  dtVector3 map2dTo3d::firstDerV( float const & uu, float const & vv) const {
    return firstDer(uu, vv)[1];		
  }

  std::vector< dtVector3 > map2dTo3d::secondDer( 
    float const & uu, float const & vv
  ) const {
    float uP = percent_u(uu);
    float vP = percent_v(vv);
    float const deltaPer = 0.01;
    float const deltaPerInv = 1. - deltaPer;		
    std::vector< dtVector3 > dd(3);
		
		//
		// dUU
		//
    if (uP<deltaPer) {
      dd[0]
			=
			(firstDerU(deltaPer, vP) - firstDerU(0., vP))
			/
			(u_percent(deltaPer) - u_percent(0.) );      
    }
    else if ( (uP>=deltaPer) && (uP<=deltaPerInv) ) {
      dd[0]
			=
			(firstDerU(uP+deltaPer, vP) - firstDerU(uP-deltaPer, vP))
			/
			(u_percent(uP+deltaPer) - u_percent(uP-deltaPer) );
    }
    else if (uP>deltaPerInv) {
      dd[0]
			=
			(firstDerU(1., vP) - firstDerU(1.-deltaPer, vP))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );      
    }       

		//
		// ddUV
		//
    if (vP<deltaPer) {
      dd[1]
			=
			(firstDerV(uP, deltaPer) - firstDerV(uP, 0.))
			/
			(u_percent(deltaPer) - u_percent(0.) );      
    }
    else if ( (vP>=deltaPer) && (vP<=deltaPerInv) ) {
      dd[1]
			=
			(firstDerV(uP, vP+deltaPer) - firstDerV(uP, vP-deltaPer))
			/
			(u_percent(uP+deltaPer) - u_percent(uP-deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[1]
			=
			(firstDerV(uP, 1.) - firstDerV(uP, 1.-deltaPer))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );      
    }       
	
		//
		// dVV
		//
    if (vP<deltaPer) {
      dd[2]
			=
			(firstDerV(uP, deltaPer) - firstDerV(uP, 0.))
			/
			(v_percent(deltaPer) - v_percent(0.) );      
    }
    else if ( (vP>=deltaPer) && (vP<=deltaPerInv) ) {
      dd[2]
			=
			(firstDerV(uP, vP+deltaPer) - firstDerU(uP, vP-deltaPer))
			/
			(v_percent(vP+deltaPer) - v_percent(vP-deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[2]
			=
			(firstDerV(uP, 1.) - firstDerV(uP, 1.-deltaPer))
			/
			(v_percent(1.) - v_percent(deltaPerInv) );      
    }    		
	
	  return dd;
	}
	
  dtVector3 map2dTo3d::secondDerUU( float const & uu, float const & vv) const {
		return secondDer(uu, vv)[0];
  }
  
  dtVector3 map2dTo3d::secondDerVV( float const & uu, float const & vv) const {
		return secondDer(uu, vv)[2];
  }
  
  dtVector3 map2dTo3d::secondDerUV( float const & uu, float const & vv) const {
		return secondDer(uu, vv)[1];
  }
  
	dtMatrix map2dTo3d::jacobi( float const & uu, float const & vv) const {
		return dtLinearAlgebra::createMatrixGiveColumns( firstDer(uu, vv) );
	}
	
	dtMatrix map2dTo3d::jacobi( dtPoint2 const & pp ) const {
		return jacobi(pp.x(), pp.y());
	}
	
  dtPoint2 map2dTo3d::reparamPercentOnFace( dtPoint3 const & ppXYZ ) const {
    dtPoint2 ppUV = reparamOnFace(ppXYZ);
    
    return percent_uv(ppUV);
  }

  dtPoint2 map2dTo3d::uv_percent(dtPoint2 const & pUV) const {
    return dtPoint2( u_percent(pUV.x()), v_percent(pUV.y()) );
  }
    
  dtPoint2 map2dTo3d::uv_percent(float const & uu, float const & vv) const {
    return dtPoint2( u_percent(uu), v_percent(vv) );
  }
    
  float map2dTo3d::u_percent(float const & uu) const {
    return (getUMin() +  (getUMax() - getUMin() ) * uu);
  }
    
  float map2dTo3d::v_percent(float const & vv) const {
    return (getVMin() +  (getVMax() - getVMin() ) * vv);
  }
  
  dtPoint2 map2dTo3d::percent_uv(dtPoint2 const & pUV) const {
    return dtPoint2( percent_u(pUV.x()), percent_v(pUV.y()) );
  }
  
  float map2dTo3d::percent_u(float const & uu) const {
    return ( (uu - getUMin()) / (getUMax() - getUMin()) );
  }
  
  float map2dTo3d::percent_v(float const & vv) const {
    return ( (vv - getVMin()) / (getVMax() - getVMin()) );
  }

	map1dTo3d * map2dTo3d::segment( 
    dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
		ptrHandling< dtCurve2d > dtC2d( 
		  trimmedCurve2d_twoPointsConnectConstructOCC(p0, p1).result() 
		);	
		vec2dCurve2dOneD v2d1d(dtC2d.get());		
		return new vec2dOneDInMap2dTo3d(&v2d1d, this);
	}
 
	map1dTo3d * map2dTo3d::segmentConstU( 
    float const & uu, float const & p0, float const & p1 
  ) const {
	  dtPoint2 p20(uu, p0);
		dtPoint2 p21(uu, p1);
		
		return segment(p20, p21);
	}

	map1dTo3d * map2dTo3d::segmentConstV( 
    float const & vv, float const & p0, float const & p1 
  ) const {
	  dtPoint2 p20(p0, vv);
		dtPoint2 p21(p1, vv);
		
		return segment(p20, p21);		
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
  map2dTo3d * map2dTo3d::segment( 
	  dtPoint2 const & p0, dtPoint2 const & p1, 
	  dtPoint2 const & p2, dtPoint2 const & p3 
	) const {				
		return new vec2dTwoDInMap2dTo3d(
			dt__tmpPtr(vec2dTwoD, new vec2dBiLinearTwoD(p0, p1, p2, p3)), this
		);
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
	map2dTo3d * map2dTo3d::segmentRectangle( 
    dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
		return segment(p0, dtPoint2(p1.x(), p0.y()), p1, dtPoint2(p0.x(), p1.y()));
	}	
	
	map1dTo3d * map2dTo3d::segmentConstU( float const & uu ) const {		
		return segmentConstU(uu, getVMin(), getVMax());
	}

	map1dTo3d * map2dTo3d::segmentConstV( float const & vv ) const {
		return segmentConstV(vv, getUMin(), getUMax());
	}
  
	map1dTo3d * map2dTo3d::segmentConstUPercent( 
    float const & uu, float const & p0, float const & p1 
  ) const {		
		return segmentConstU(u_percent(uu), v_percent(p0), v_percent(p1));
	}

	map1dTo3d * map2dTo3d::segmentConstVPercent( 
    float const & vv, float const & p0, float const & p1 
  ) const {
		return segmentConstV( v_percent(vv), u_percent(p0), u_percent(p1) );
	}

	map1dTo3d * map2dTo3d::segmentConstUPercent( float const & uu ) const {		
		return segmentConstU(u_percent(uu));
	}

	map1dTo3d * map2dTo3d::segmentConstVPercent( float const & vv ) const {
	  return segmentConstV(v_percent(vv));
	}
	
	map1dTo3d * map2dTo3d::segmentPercent( 
    dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
		return segment(uv_percent(p0), uv_percent(p1));
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
  map2dTo3d * map2dTo3d::segmentPercent( 
	  dtPoint2 const & p0, dtPoint2 const & p1, 
	  dtPoint2 const & p2, dtPoint2 const & p3 
	) const {
    return segment(
			uv_percent(p0), uv_percent(p1), uv_percent(p2), uv_percent(p3)
		);
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
	map2dTo3d * map2dTo3d::segmentRectanglePercent( 
    dtPoint2 const & p0, dtPoint2 const & p1 
  ) const {
		return segmentRectangle(uv_percent(p0), uv_percent(p1));
	}
	
//  /**
//   * 
//   * @todo: Make precision adjustable. Maybe increase precision automatically.
//   */
//  bool map2dTo3d::XYZtoUV(double X, double Y, double Z, double &U, double &V,
//                          double relax, std::vector< float > &itVal) const {
//    double const Precision
//    =
//    static_cast<double>(
//      staticPropertiesHandler::getInstance()->getOptionFloat(
//        "reparamOnFace_precision"
//      )
//    );      
//    double const precXYZ
//    =
//    static_cast<double>(
//      staticPropertiesHandler::getInstance()->getOptionFloat(
//        "reparamOnFace_precisionXYZ"
//      )
//    );      		
//    const int MaxIter = 25;
//    const int NumInitGuess = 11;
//
//    double Unew = 0., Vnew = 0., err, err2;
//    int iter;
//      double umin, umax, vmin, vmax;
//    // don't use 0.9, 0.1 it fails with ruled surfaces
//    double initu[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
//    double initv[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
//
//    umin = static_cast<double>(getUMin());
//    umax = static_cast<double>(getUMax());
//		double udiff = umax - umin;
//		umax = umax + 0.1*udiff;
//		umin = umin - 0.1*udiff;		
//    vmin = static_cast<double>(getVMin());
//    vmax = static_cast<double>(getVMax());
//		double vdiff = vmax - vmin;
//		vmax = vmax + 0.1*vdiff;
//		vmin = vmin - 0.1*vdiff;		
//    const double tol = Precision * (SQU(umax - umin) + SQU(vmax-vmin));
//		
//    for(int i = 0; i < NumInitGuess; i++) {
//      initu[i] = umin + initu[i] * (umax - umin);
//      initv[i] = vmin + initv[i] * (vmax - vmin);
//    }
//
//    for(int i = 0; i < NumInitGuess; i++){
//		  U = initu[i];			
//      for(int j = 0; j < NumInitGuess; j++){
//		    V = initv[j];				
//			  err = 1.0;
//				iter = 1;				
//        
//				try {
//					dtPoint3 P = getPoint(static_cast<float>(U), static_cast<float>(V));
//					err2 = sqrt(SQU(X - P.x()) + SQU(Y - P.y()) + SQU(Z - P.z()));
//		//      if (err2 < 1.e-8 * CTX::instance()->lc) return;
//
//					while(err > tol && iter < MaxIter) {
//						P = getPoint(static_cast<float>(U), static_cast<float>(V));
//						dtVector3 derU = firstDerU(static_cast<float>(U), static_cast<float>(V));
//						dtVector3 derV = firstDerV(static_cast<float>(U), static_cast<float>(V));
//						dtMatrix mat(2,3);
//						mat(0,0) = derU.x(); mat(0,1) = derU.y(); mat(0,2) = derU.z();
//						mat(1,0) = derV.x(); mat(1,1) = derV.y(); mat(1,2) = derV.z();
//						dtMatrix jac = dtLinearAlgebra::invert2x3Matrix(mat);
//
//						Unew = U + relax *
//							(jac(0,0) * (X - P.x()) + jac(1,0) * (Y - P.y()) +
//							 jac(2,0) * (Z - P.z()));
//						Vnew = V + relax *
//							(jac(0,1) * (X - P.x()) + jac(1,1) * (Y - P.y()) +
//							 jac(2,1) * (Z - P.z()));
////						dtMatrix mat(3,2);
////						mat(0,0) = derU.x(); mat(1,0) = derU.y(); mat(2,0) = derU.z();
////						mat(0,1) = derV.x(); mat(1,1) = derV.y(); mat(2,1) = derV.z();
////						dtMatrix jac = dtLinearAlgebra::invertMatrix(mat);
////
////						Unew = U + relax *
////							(jac(0,0) * (X - P.x()) + jac(0,1) * (Y - P.y()) +
////							 jac(0,2) * (Z - P.z()));
////						Vnew = V + relax *
////							(jac(1,0) * (X - P.x()) + jac(1,1) * (Y - P.y()) +
////							 jac(1,2) * (Z - P.z()));
//
//						// don't remove this test: it is important
//						if((Unew > umax+tol || Unew < umin-tol) ||
//							 (Vnew > vmax+tol || Vnew < vmin-tol)) {
//							break;
//						}
//
//						if ( isnan(Unew) ) break;
//						if ( isnan(Vnew) ) break;
//						
//						err = SQU(Unew - U) + SQU(Vnew - V);
//						err2 = sqrt(SQU(X - P.x()) + SQU(Y - P.y()) + SQU(Z - P.z()));
//
//						iter++;
//						U = Unew;
//						V = Vnew;
//					}
//
//					itVal.push_back( static_cast<float>(i) );
//					itVal.push_back( static_cast<float>(j) );
//					itVal.push_back( static_cast<float>(err2) );
//					itVal.push_back( static_cast<float>(err) );
//					itVal.push_back( static_cast<float>(iter) );
//						
//					bool inRange = (Unew <= umax) && (Vnew <= vmax) 
//					               && (Unew >= umin) && (Vnew >= vmin);
//					bool uvConv = (err <= tol);
//					bool xyzConv = (err2 <= precXYZ);
//					
//					if( ( (iter<MaxIter) && inRange && xyzConv ) 
//						|| ( (iter<MaxIter) && inRange && uvConv )  
//						) {
////						itVal.clear();
////						itVal.push_back(static_cast<float>(Unew) );
////						itVal.push_back(static_cast<float>(Vnew) );
////						itVal.push_back(static_cast<float>(err2) );
////						itVal.push_back(static_cast<float>(err) );
////						itVal.push_back(static_cast<float>(tol) );            
//
//						return true;
//					}
//			  }
//				catch (eGeneral & eGenRef) {
////					eGenRef.clear();
////					itVal.clear();
//					dt__warning(
//						XYZtoUV(), 
//						<< "Break initial guess (" << i << ", " << j 
//						<< ") and try next one." << std::endl
//						<< eGenRef.what());
//					itVal.push_back( static_cast<float>(i) );
//					itVal.push_back( static_cast<float>(j) );
//					itVal.push_back( static_cast<float>(err2) );
//					itVal.push_back( static_cast<float>(err) );
//					itVal.push_back( static_cast<float>(iter) );					
//					break;
//				}				
//      }
//    }
//
//    if(relax < 1.e-6) {
//      dt__info(XYZtoUV(), << "Could not converge: surface mesh could be wrong");
//      return false;
//    }
//    else {
//      return XYZtoUV(X, Y, Z, U, V, 0.75 * relax, itVal);
//    }
//  }
  bool map2dTo3d::XYZtoUVPercent(
    double X, double Y, double Z, double &U, double &V
  ) const {
    _pXYZ = dtPoint3(X, Y, Z);
    
		// 
		// multidimensional minimization
		//
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(
        staticPropertiesHandler::getInstance()->getOption(
          "reparamOnFace_minimizer"
        )
      )
    );        
		::ROOT::Math::Functor toMin(
			this, &map2dTo3d::F, 2 
		);			
		min->SetFunction(toMin);

		//
		// set bounds
		//
    std::vector< double > init(2, 0);
    init[0] = U; init[1] = V;
    for (int ii=0; ii<2; ii++) {
      std::string xStr = "x"+stringPrimitive::intToString(ii);
		  min->SetVariable( ii, xStr, init[ii], 0.01 );			
      min->SetVariableLimits(ii, 0., 1.);	
    }
    
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
        "reparamOnFace_precision"
      )    
    );			
		min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
    );

		//
		// minimize
		//
   	bool converged = min->Minimize();

    double const * const theRoot = min->X( );

    U = theRoot[0];//std::max<double>( std::min<double>( theRoot[0], getUMax() ), getUMin());
    V = theRoot[1];//std::max<double>( std::min<double>( theRoot[1], getVMax() ), getVMin());
    
    return converged;
	}
  
	double map2dTo3d::F(double const * xx) const {	
    double objective 
    = 
    dtLinearAlgebra::length(
      _pXYZ - getPointPercent(dtPoint2(xx[0], xx[1]))
    );
    return objective;
	}	   	
  
	dtPoint2 map2dTo3d::operator%(const dtPoint2 &percent) const {
		return dtPoint2( this->uv_percent(percent) );
	}
	
  map2dTo3d * new_clone(map2dTo3d const & m2d){
    return m2d.clone();
  }
}