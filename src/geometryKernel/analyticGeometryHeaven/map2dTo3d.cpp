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

#include "map2dTo3d.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include "map1dTo3d.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <solid3dSurface.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dBiLinearTwoD.h>
#include "vec2dOneDInMap2dTo3d.h"
#include "vec2dTwoDInMap2dTo3d.h"

#include <dtAnalysis.h>
#include <Math/Functor.h>

namespace dtOO {  
  dtReal map2dTo3d::_deltaPer 
  = 
  staticPropertiesHandler::getInstance()->getOptionFloat("map2dTo3d_deltaPer");
  
  map2dTo3d::map2dTo3d() : analyticGeometry() {
    isUVGridPrecomputed = false;
  }

  map2dTo3d::map2dTo3d(const map2dTo3d& orig) : analyticGeometry(orig) {
    isUVGridPrecomputed = false;
  }

  map2dTo3d::~map2dTo3d() {
  }
  
  dtInt map2dTo3d::dim( void ) const {
    return 2;
  }

  dtPoint3 map2dTo3d::getPoint( dtReal const * const uvw ) const {
    return getPoint( uvw[0], uvw[1] );
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
  
  dtReal map2dTo3d::getUMin( void ) const {
    return getMin(0);
  }
  
  dtReal map2dTo3d::getUMax( void ) const {
    return getMax(0);
  }
  
  dtReal map2dTo3d::getVMin( void ) const {
    return getMin(1);
  }
  
  dtReal map2dTo3d::getVMax( void ) const {
    return getMax(1);
  }
  
  bool map2dTo3d::inRange( dtPoint2 const & pUV ) const {
    if ( pUV.x() > getUMax() ) return false;
    if ( pUV.x() < getUMin() ) return false;
    if ( pUV.y() > getVMax() ) return false;
    if ( pUV.y() < getVMin() ) return false;
    return true;
  }
  
  dtPoint3 map2dTo3d::getPointPercent( 
    dtReal const & uu, dtReal const & vv 
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
    dtReal intervalFirst = (getUMax() - getUMin()) / (renderResU-1.);
    dtReal intervalSecond = (getVMax() - getVMin()) / (renderResV-1.);  

    twoDArrayHandling< dtPoint3 > surfacePoints(renderResU, renderResV);
    for (int jj=0; jj<renderResU; jj++) {
      for(int ii=0; ii<renderResV; ii++) {
				dtReal uu;
				dtReal vv;				
        if (jj == (renderResU-1) ) {
          uu = getUMax();
        }
        else {
          uu = (dtReal) (getUMin() + jj * intervalFirst);
        }
        if (ii == (renderResV-1) ) {      
          vv = getVMax();
        }
        else {
          vv = (dtReal) (getVMin() + ii * intervalSecond);
        }
        surfacePoints[jj][ii] = getPoint(uu, vv);
      }
    }
		
		vectorHandling< renderInterface * > rV;
		rV.push_back( new solid3dSurface(surfacePoints) );
		
    //
    // get surface directions
    //    
		vectorHandling< dtVector3 > vecV;
		vectorHandling< dtPoint3 > vecP;
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
    
    rV.push_back( new discrete3dVector(vecV, vecP) );
		return rV;
	}
	
  vectorHandling< renderInterface * > map2dTo3d::getExtRender( void ) const {
		vectorHandling< dtVector3 > norV;
		vectorHandling< dtPoint3 > norP;
		

    //
    // add normals
    //
		norV.push_back(map2dTo3d::normalPercent(.25, .25)); 
		norP.push_back(map2dTo3d::getPointPercent(.25, .25));
		norV.push_back(map2dTo3d::normalPercent(.25, .75)); 
		norP.push_back(map2dTo3d::getPointPercent(.25, .75));		
		norV.push_back(map2dTo3d::normalPercent(.75, .25)); 
		norP.push_back(map2dTo3d::getPointPercent(.75, .25));		
		norV.push_back(map2dTo3d::normalPercent(.75, .75)); 
		norP.push_back(map2dTo3d::getPointPercent(.75, .75));		
		
		vectorHandling< renderInterface * > retVec(1);
		retVec[0] = new discrete3dVector(norV, norP);		
		return retVec;
  }
   
  dtInt map2dTo3d::getRenderResolutionU( void ) const { 
    return analyticGeometry::getRenderResolution(0);
  }

  dtInt map2dTo3d::getRenderResolutionV( void ) const { 
    return analyticGeometry::getRenderResolution(1);
  }  
  
  dtVector3 map2dTo3d::getPointPercentVector( 
    dtReal const & uu, dtReal const & vv 
  ) const {
    return dtLinearAlgebra::toDtVector3( getPointPercent(uu, vv) );
  }
  
  dtVector3 map2dTo3d::normal( dtReal const & uu, dtReal const & vv) const {
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
  
  map2dTo3d::UVGridData map2dTo3d::precomputeUVGrid() const {
      
	  int grid_size_u = 10, grid_size_v = 10;
	  int total_points = grid_size_u * grid_size_v;

	  map2dTo3d::UVGridData gridData;
	  gridData.U_grid.resize(total_points);
	  gridData.V_grid.resize(total_points);
	  gridData.X_grid.resize(total_points);
	  gridData.Y_grid.resize(total_points);
	  gridData.Z_grid.resize(total_points);

	  for (int i = 0; i < grid_size_u; ++i) {
	      for (int j = 0; j < grid_size_v; ++j) {
		  int idx = i * grid_size_v + j;

		  double U = static_cast<double>(i) / (grid_size_u - 1);
		  double V = static_cast<double>(j) / (grid_size_v - 1);

		  dtPoint3 xyz = getPointPercent(U, V);  // Could this throw an exception?

		  gridData.U_grid[idx] = U;
		  gridData.V_grid[idx] = V;
		  gridData.X_grid[idx] = xyz.x();
		  gridData.Y_grid[idx] = xyz.y();
		  gridData.Z_grid[idx] = xyz.z();
	      }
	  }

	  std::cout << "Precomputed UV-XYZ grid with " << total_points << " points.\n";
	  return gridData;
  }


  std::pair<double, double> map2dTo3d::findNearestUV(double X, double Y, double Z, const UVGridData& grid) const {

      size_t N = grid.X_grid.size();
      std::vector<double> distances(N);

      // #pragma omp parallel for
      for (size_t i = 0; i < N; ++i) {
	  double dx = X - grid.X_grid[i];
	  double dy = Y - grid.Y_grid[i];
	  double dz = Z - grid.Z_grid[i];
	  distances[i] = dx * dx + dy * dy + dz * dz;
      }

      size_t best_index = std::distance(
	  distances.begin(),
	  std::min_element(distances.begin(), distances.end())
      );
      if ( 
        (best_index > (N-1)) 
        ||
        (best_index < 0)
      ) {
        dt__throw(findNearestUV(), << "best_index = " << best_index );
      }
      return {grid.U_grid[best_index], grid.V_grid[best_index]};
  }

// End trent extenstion

 
  dtPoint2 map2dTo3d::reparamOnFace(dtPoint3 const & ppXYZ) const {
    double X = ppXYZ.x();
    double Y = ppXYZ.y();
    double Z = ppXYZ.z();
    double U;
    double V;
    double UBest = 1.E+99;
    double VBest = 1.E+99;
    
    if (isUVGridPrecomputed != true){
      grid = precomputeUVGrid();
      isUVGridPrecomputed = true;
      }
    dtInt const NumInitGuess = 3;
    double initU[NumInitGuess] = {0.5, 0.75, 0.25};
    double initV[NumInitGuess] = {0.5, 0.75, 0.25};
    dtInt maxRestarts 
    = 
    staticPropertiesHandler::getInstance()->getOptionInt("reparam_restarts");
    dtInt maxInternalRestarts 
    = 
    staticPropertiesHandler
      ::getInstance()->getOptionInt("reparam_internalRestarts");    
    dtReal restartIncreasePrec
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "reparam_restartIncreasePrecision"
    );
    dtReal internalRestartDecreasePrec
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "reparam_internalRestartDecreasePrecision"
    );    
    dtReal currentPrec = 1.;
    dtReal dist = 1.E+99;
    dt__forFromToIndex(0, maxRestarts+1, thisRun) {

      std::pair<double, double> initUV = findNearestUV(X, Y, Z, grid);
      double U = initUV.first;
      double V = initUV.second;

      dt__forFromToIndex(0, maxInternalRestarts, thisRestart) {
            double stepU = .01;
            double stepV = .01;
            double prec = 1.;
            double uMin = 0.;
            double uMax = 1.;
            double vMin = 0.;
            double vMax = 1.;            
            try {
              XYZtoUVPercent(
                X, Y, Z, 
                U, V, uMin, uMax, vMin, vMax, 
                stepU, stepV, prec
              );
            }
            catch( eGeneral & eGen ) {
              dt__warning(
                reparamOnFace(),
                << logMe::dtFormat(
                  "Error for initU = %12.4e initV = %12.4e at internalRestart = %i"
                ) 
     //           % initU[ii] % initV[jj] % thisRestart 
                << std::endl
                << "eGen.what() = " << eGen.what()
              );
            }
            //
            // increase precision for restart
            //
            prec = internalRestartDecreasePrec * prec;
            dtPoint3 tP = getPointPercent(U,V);
            //
            // check if point is precise enough
            //
            dtReal cDist;
            if (
              analyticGeometry::inXYZTolerance(
                ppXYZ, tP, &cDist, false, currentPrec
              )
            ) return uv_percent( dtPoint2(U, V) );
            if (cDist <= dist) {
              UBest = U;
              VBest = V;
              dist = cDist;
            }
          }
      dt__warning(
        reparamOnFace(), 
        << "Increasing reparamOnFace tolerance. Multiply initial precision by "
        << restartIncreasePrec * currentPrec
      );        
      currentPrec = restartIncreasePrec * currentPrec;
    }
    dt__throw(
      reparamOnFace(), 
      << "Reparameterization of " << dt__point3d(ppXYZ) << " fails." 
      << std::endl
      << "(0,0) -> " << getPointPercent(0,0) << std::endl
      << "(1,0) -> " << getPointPercent(1,0) << std::endl
      << "(0,1) -> " << getPointPercent(0,1) << std::endl
      << "(1,1) -> " << getPointPercent(1,1) << std::endl
      << "UBest = " << UBest << std::endl
      << "VBest = " << VBest << std::endl
      << "dist = " << dist << std::endl
      << dumpToString()
    );
  }

  dtPoint2 map2dTo3d::approxOnFace(dtPoint3 const & ppXYZ) const {
    return uv_map2dTo3dClosestPointToPoint(this, ppXYZ).result();
  }  
  
  dtVector3 map2dTo3d::normalPercent( 
    dtReal const & uu, dtReal const & vv 
  ) const {
    return normal(u_percent(uu), v_percent(vv));  
  }
	
	std::vector< dtVector3 > map2dTo3d::firstDer( 
    dtReal const & uu, dtReal const & vv
  ) const {
		//
		// dU
		//
    dtReal uP = percent_u(uu);
    dtReal vP = percent_v(vv);
    dtReal const deltaPerInv = 1. - _deltaPer;
		std::vector< dtVector3 > dd(2);
    
    if (uP<_deltaPer) {
      dd[0] 
			= 
			(getPointPercent(_deltaPer, vP) - getPointPercent(0., vP))
			/
			(u_percent(_deltaPer) - u_percent(0.) );      
    }
    else if ( (uP>=_deltaPer) && (uP<=deltaPerInv) ) {
      dd[0] 
			= 
			( getPointPercent(uP+_deltaPer, vP) - getPointPercent(uP-_deltaPer, vP) )
			/
			( u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) );
    }
    else if (uP>deltaPerInv) {
      dd[0] 
			= 
			(getPointPercent(1., vP) - getPointPercent(1.-_deltaPer, vP))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );    
    }
		
		//
		// dV
		//
    if (vP<_deltaPer) {
      dd[1] 
			= 
			(getPointPercent(uP, _deltaPer) - getPointPercent(uP, 0.))
			/
			(v_percent(_deltaPer) - v_percent(0.) );
    }
    else if ( (vP>=_deltaPer) && (vP<=deltaPerInv) ) {
      dd[1] 
			= 
			(getPointPercent(uP, vP+_deltaPer) - getPointPercent(uP, vP-_deltaPer))
			/
			(v_percent(vP+_deltaPer) - v_percent(vP-_deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[1] 
			= 
			(getPointPercent(uP, 1.) - getPointPercent(uP, 1.-_deltaPer))
			/
			(v_percent(1.) - v_percent(deltaPerInv) );      
    }
		
		return dd;
	}
  
  std::vector< dtVector3 > map2dTo3d::firstDer( dtPoint2 const & pp ) const {
		return firstDer(pp.x(), pp.y());
	}
	
  dtVector3 map2dTo3d::firstDerU( dtReal const & uu, dtReal const & vv) const {
		return firstDer(uu, vv)[0];
  }
  
  dtVector3 map2dTo3d::firstDerV( dtReal const & uu, dtReal const & vv) const {
    return firstDer(uu, vv)[1];		
  }

  std::vector< dtVector3 > map2dTo3d::secondDer( 
    dtReal const & uu, dtReal const & vv
  ) const {
    dtReal uP = percent_u(uu);
    dtReal vP = percent_v(vv);
    dtReal const deltaPerInv = 1. - _deltaPer;		
    std::vector< dtVector3 > dd(3);
		
		//
		// dUU
		//
    if (uP<_deltaPer) {
      dd[0]
			=
			(firstDerU(_deltaPer, vP) - firstDerU(0., vP))
			/
			(u_percent(_deltaPer) - u_percent(0.) );      
    }
    else if ( (uP>=_deltaPer) && (uP<=deltaPerInv) ) {
      dd[0]
			=
			(firstDerU(uP+_deltaPer, vP) - firstDerU(uP-_deltaPer, vP))
			/
			(u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) );
    }
    else if (uP>deltaPerInv) {
      dd[0]
			=
			(firstDerU(1., vP) - firstDerU(1.-_deltaPer, vP))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );      
    }       

		//
		// ddUV
		//
    if (vP<_deltaPer) {
      dd[1]
			=
			(firstDerV(uP, _deltaPer) - firstDerV(uP, 0.))
			/
			(u_percent(_deltaPer) - u_percent(0.) );      
    }
    else if ( (vP>=_deltaPer) && (vP<=deltaPerInv) ) {
      dd[1]
			=
			(firstDerV(uP, vP+_deltaPer) - firstDerV(uP, vP-_deltaPer))
			/
			(u_percent(uP+_deltaPer) - u_percent(uP-_deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[1]
			=
			(firstDerV(uP, 1.) - firstDerV(uP, 1.-_deltaPer))
			/
			(u_percent(1.) - u_percent(deltaPerInv) );      
    }       
	
		//
		// dVV
		//
    if (vP<_deltaPer) {
      dd[2]
			=
			(firstDerV(uP, _deltaPer) - firstDerV(uP, 0.))
			/
			(v_percent(_deltaPer) - v_percent(0.) );      
    }
    else if ( (vP>=_deltaPer) && (vP<=deltaPerInv) ) {
      dd[2]
			=
			(firstDerV(uP, vP+_deltaPer) - firstDerU(uP, vP-_deltaPer))
			/
			(v_percent(vP+_deltaPer) - v_percent(vP-_deltaPer) );
    }
    else if (vP>deltaPerInv) {
      dd[2]
			=
			(firstDerV(uP, 1.) - firstDerV(uP, 1.-_deltaPer))
			/
			(v_percent(1.) - v_percent(deltaPerInv) );      
    }    		
	
	  return dd;
	}
	
  dtVector3 map2dTo3d::secondDerUU( dtReal const & uu, dtReal const & vv) const {
		return secondDer(uu, vv)[0];
  }
  
  dtVector3 map2dTo3d::secondDerVV( dtReal const & uu, dtReal const & vv) const {
		return secondDer(uu, vv)[2];
  }
  
  dtVector3 map2dTo3d::secondDerUV( dtReal const & uu, dtReal const & vv) const {
		return secondDer(uu, vv)[1];
  }
  
	dtMatrix map2dTo3d::jacobi( dtReal const & uu, dtReal const & vv) const {
		return dtLinearAlgebra::createMatrixGiveColumns( firstDer(uu, vv) );
	}
	
	dtMatrix map2dTo3d::jacobi( dtPoint2 const & pp ) const {
		return jacobi(pp.x(), pp.y());
	}
	
  dtPoint2 map2dTo3d::reparamPercentOnFace( dtPoint3 const & ppXYZ ) const {
    dtPoint2 ppUV = reparamOnFace(ppXYZ);
    
    return percent_uv(ppUV);
  }

  dtPoint2 map2dTo3d::approxPercentOnFace( dtPoint3 const & ppXYZ ) const {
    dtPoint2 ppUV = approxOnFace(ppXYZ);
    
    return percent_uv(ppUV);
  }  

  dtPoint2 map2dTo3d::uv_percent(dtPoint2 const & pUV) const {
    return dtPoint2( u_percent(pUV.x()), v_percent(pUV.y()) );
  }
    
  dtPoint2 map2dTo3d::uv_percent(dtReal const & uu, dtReal const & vv) const {
    return dtPoint2( u_percent(uu), v_percent(vv) );
  }
    
  dtReal map2dTo3d::u_percent(dtReal const & uu) const {
    return floatHandling::boundToRange( 
      getUMin() +  (getUMax() - getUMin() ) * uu, getUMin(), getUMax()
    );
  }
    
  dtReal map2dTo3d::v_percent(dtReal const & vv) const {
    return floatHandling::boundToRange( 
      getVMin() +  (getVMax() - getVMin() ) * vv, getVMin(), getVMax()
    );
  }
  
  dtPoint2 map2dTo3d::percent_uv(dtPoint2 const & pUV) const {
    return dtPoint2( percent_u(pUV.x()), percent_v(pUV.y()) );
  }
  
  dtReal map2dTo3d::percent_u(dtReal const & uu) const {
    return floatHandling::boundToRange( 
      (uu - getUMin()) / (getUMax() - getUMin()), 0., 1.
    );
  }
  
  dtReal map2dTo3d::percent_v(dtReal const & vv) const {
    return floatHandling::boundToRange( 
      (vv - getVMin()) / (getVMax() - getVMin()), 0., 1.
    );
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
    dtReal const & uu, dtReal const & p0, dtReal const & p1 
  ) const {
	  dtPoint2 p20(uu, p0);
		dtPoint2 p21(uu, p1);
		
		return segment(p20, p21);
	}

	map1dTo3d * map2dTo3d::segmentConstV( 
    dtReal const & vv, dtReal const & p0, dtReal const & p1 
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
	
	map1dTo3d * map2dTo3d::segmentConstU( dtReal const & uu ) const {		
		return segmentConstU(uu, getVMin(), getVMax());
	}

	map1dTo3d * map2dTo3d::segmentConstV( dtReal const & vv ) const {
		return segmentConstV(vv, getUMin(), getUMax());
	}
  
	map1dTo3d * map2dTo3d::segmentConstUPercent( 
    dtReal const & uu, dtReal const & p0, dtReal const & p1 
  ) const {		
		return segmentConstU(u_percent(uu), v_percent(p0), v_percent(p1));
	}

	map1dTo3d * map2dTo3d::segmentConstVPercent( 
    dtReal const & vv, dtReal const & p0, dtReal const & p1 
  ) const {
		return segmentConstV( v_percent(vv), u_percent(p0), u_percent(p1) );
	}

	map1dTo3d * map2dTo3d::segmentConstUPercent( dtReal const & uu ) const {		
		return segmentConstU(u_percent(uu));
	}

	map1dTo3d * map2dTo3d::segmentConstVPercent( dtReal const & vv ) const {
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
	
//  bool map2dTo3d::XYZtoUVPercentGmsh(
//    double X, double Y, double Z, double &U, double &V, 
//    double const uMin, double const uMax, 
//    double const vMin, double const vMax, 
//    double const stepU, double const stepV, double const prec
//  ) const {
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
//    const dtInt MaxIter = 25;
//    const dtInt NumInitGuess = 11;
//
//    double Unew = 0., Vnew = 0., err, err2;
//    dtInt iter;
//      double umin, umax, vmin, vmax;
//    // don't use 0.9, 0.1 it fails with ruled surfaces
//    double initu[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
//    double initv[NumInitGuess] = {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};
//
//    umin = uMin;//static_cast<double>(getUMin());
//    umax = uMax;//static_cast<double>(getUMax());
//		double udiff = umax - umin;
//		umax = umax + 0.1*udiff;
//		umin = umin - 0.1*udiff;		
//    vmin = vMin;//static_cast<double>(getVMin());
//    vmax = vMax;//static_cast<double>(getVMax());
//		double vdiff = vmax - vmin;
//		vmax = vmax + 0.1*vdiff;
//		vmin = vmin - 0.1*vdiff;		
//    const double tol = Precision * ((umax-umin)*(umax-umin) + (vmax-vmin)*(vmax-vmin));
//    const double relax = prec;
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
//					dtPoint3 P = getPoint(static_cast<dtReal>(U), static_cast<dtReal>(V));
//					err2 
//          = 
//          sqrt(
//            (X - P.x())*(X - P.x()) 
//            + 
//            (Y - P.y())*(Y - P.y()) 
//            + 
//            (Z - P.z())*(Z - P.z())
//          );
//		//      if (err2 < 1.e-8 * CTX::instance()->lc) return;
//
//					while(err > tol && iter < MaxIter) {
//						P = getPoint(static_cast<dtReal>(U), static_cast<dtReal>(V));
//						dtVector3 derU = firstDerU(static_cast<dtReal>(U), static_cast<dtReal>(V));
//						dtVector3 derV = firstDerV(static_cast<dtReal>(U), static_cast<dtReal>(V));
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
//						err = (Unew - U)*(Unew - U) + (Vnew - V)*(Vnew - V);
//						err2 //= sqrt(SQU(X - P.x()) + SQU(Y - P.y()) + SQU(Z - P.z()));
//            =
//            sqrt(
//              (X - P.x())*(X - P.x()) 
//              + 
//              (Y - P.y())*(Y - P.y()) 
//              + 
//              (Z - P.z())*(Z - P.z())
//            );
//
//						iter++;
//						U = Unew;
//						V = Vnew;
//					}
//
////					itVal.push_back( static_cast<dtReal>(i) );
////					itVal.push_back( static_cast<dtReal>(j) );
////					itVal.push_back( static_cast<dtReal>(err2) );
////					itVal.push_back( static_cast<dtReal>(err) );
////					itVal.push_back( static_cast<dtReal>(iter) );
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
////						itVal.push_back(static_cast<dtReal>(Unew) );
////						itVal.push_back(static_cast<dtReal>(Vnew) );
////						itVal.push_back(static_cast<dtReal>(err2) );
////						itVal.push_back(static_cast<dtReal>(err) );
////						itVal.push_back(static_cast<dtReal>(tol) );            
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
////					itVal.push_back( static_cast<dtReal>(i) );
////					itVal.push_back( static_cast<dtReal>(j) );
////					itVal.push_back( static_cast<dtReal>(err2) );
////					itVal.push_back( static_cast<dtReal>(err) );
////					itVal.push_back( static_cast<dtReal>(iter) );					
//					break;
//				}				
//      }
//    }
//
////    if(relax < 1.e-6) {
////      dt__info(XYZtoUV(), << "Could not converge: surface mesh could be wrong");
////      return false;
////    }
////    else {
////      return XYZtoUV(X, Y, Z, U, V, 0.75 * relax, itVal);
////    }
//    U = percent_u( U );
//    V = percent_v( V );
//    
//    return true;
//  }
  
  bool map2dTo3d::XYZtoUVPercent(
    double X, double Y, double Z, double &U, double &V, 
    double const uMin, double const uMax, double const vMin, double const vMax, 
    double const stepU, double const stepV, double const prec
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
    min->SetVariable( 0, "U", U, stepU );
    min->SetVariableLimits(0, uMin, uMax);	
    min->SetVariable( 1, "V", V, stepV );
    min->SetVariableLimits(1, vMin, vMax);	
    
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
      ) * prec
    );			
		min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
    );
    
		//
		// minimize
		//
   	bool converged = min->Minimize();

    double const * const theRoot = min->X();

    U = theRoot[0];//std::max<double>( std::min<double>( theRoot[0], getUMax() ), getUMin());
    V = theRoot[1];//std::max<double>( std::min<double>( theRoot[1], getVMax() ), getVMin());
    
    return converged;
	}
  
	double map2dTo3d::F(double const * xx) const {
    return dtLinearAlgebra::length( 
      _pXYZ - getPointPercent( dtPoint2(xx[0], xx[1]) ) 
    );
	}	   	

	double map2dTo3d::FWrap(double const & x0, double const & x1) const {	
    std::vector< double > uv(2);
    uv[0] = x0;
    uv[1] = x1;
    
    return F(&(uv[0]));
	}	   
  
	dtPoint2 map2dTo3d::operator%(const dtPoint2 &percent) const {
		return dtPoint2( this->uv_percent(percent) );
	}
	
  map2dTo3d * new_clone(map2dTo3d const & m2d){
    return m2d.clone();
  }
}
