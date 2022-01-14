#include "map2dTo3d_fullExtentInMap3dTo3d.h"

#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/twoDArrayHandling.h>

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h>

namespace dtOO {
	map2dTo3d_fullExtentInMap3dTo3d
	  ::map2dTo3d_fullExtentInMap3dTo3d(
      map2dTo3d const * const m2d, int const & m2dParDir, int const & toM3dParDir
	) {
    vec3dTwoDInMap3dTo3d const * const vInMap
		=
		vec3dTwoDInMap3dTo3d::ConstDownCast(m2d);
		dt__throwIf(!vInMap, map2dTo3d_fullExtentInMap3dTo3d());
		map3dTo3d const * const m3d = vInMap->refToMap3dTo3d();
		vec3dTwoD const * const v3d = vInMap->refToVec3dTwoD();
		vec3dSurfaceTwoD const * const v3dS = vec3dSurfaceTwoD::ConstDownCast(v3d);
		dt__throwIf(!v3dS, map2dTo3d_fullExtentInMap3dTo3d());

		if ( (m2dParDir == 0) && (toM3dParDir == 1) ) {
			dt__pH(dtSurface) dtS0;
			dt__pH(dtSurface) dtS( v3dS->ptrDtSurface()->clone() );
      dt__pH(dtSurface) dtS1;
			
      //
      // get derivative and approximate straight extension to the front
      //
		  std::vector< dtPoint3 > from(2);
			vectorHandling< dtCurve const * > dtC;
      from[0] = v3d->YdtPoint3Percent(0., 0.);      
			from[1] = v3d->YdtPoint3Percent(0., 1.);
			
			std::vector< dtVector3 > dY0 
			= 
			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(0., 0.) );			
      std::vector< dtVector3 > dY1 
			= 
			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(0., 1.) );

      bool isPos = false;
      if ( (dY0[0].x()>0.) && (dY0[1].x()>0.) ) isPos = true;
      
			dtReal t0 = (from[0].y() - m3d->getVMin())/dY0[0].y();
			dtReal t1 = (m3d->getVMin() - from[1].y())/dY1[0].y();
			dtReal u0max = from[0].x()-t0*dY0[0].x();
			dtReal u1max = from[1].x()+t1*dY1[0].x();
      dtReal u0 = u0max;
      dtReal u1 = u1max;      
      if (isPos) {
        u0 = std::min(u0max, u1max);
        u1 = u0;
      }
      else {
        u0 = std::max(u0max, u1max);
        u1 = u0;
      }
      
      //
      // create 4 curves for inlet extension
      //
			dtC.push_back(
				bSplineCurve_pointConstructOCC(
					dtPoint3(u0, m3d->getVMin(), m3d->getWMin()), 
					from[0]
				).result()
			);
			dtC.push_back( dtS->segmentConstUPercent(0.) );
      dtC.push_back(
				bSplineCurve_pointConstructOCC(
					from[1], 
					dtPoint3(u1, m3d->getVMin(), m3d->getWMax())
				).result()
			);			
      dtC.push_back(
				bSplineCurve_pointConstructOCC(
					dtPoint3(u1, m3d->getVMin(), m3d->getWMax()), 
					dtPoint3(u0, m3d->getVMin(), m3d->getWMin())
				).result()
			);			
      
      //
      // create inlet surface
      //
			dtS0.reset( bSplineSurface_bSplineCurveFillConstructOCC(dtC).result() );
	
      //
      // get derivative and approximate straight extension to the back
      //      
			dtC.destroy();
      from[0] = v3d->YdtPoint3Percent(1., 0.);      
			from[1] = v3d->YdtPoint3Percent(1., 1.);

			dY0 
			= 
			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(1., 0.) );			
      dY1 
			= 
			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(1., 1.) );

      isPos = false;
      if ( (dY0[0].x()>0.) && (dY0[1].x()>0.) ) isPos = true;
      
			t0 = (from[0].y() - m3d->getVMax())/dY0[0].y();
			t1 = (m3d->getVMax() - from[1].y())/dY1[0].y();
			u0max = from[0].x()-t0*dY0[0].x();
      u1max = from[1].x()+t1*dY1[0].x();
      if (isPos) {
        u0 = std::min(u0max, u1max);
        u1 = u0;
      }
      else {
        u0 = std::max(u0max, u1max);
        u1 = u0;
      }
      
      //
      // create 4 curves for outlet extension
      //
			dtC.push_back(
				bSplineCurve_pointConstructOCC(
					from[0],
					dtPoint3(u0, m3d->getVMax(), m3d->getWMin())
				).result()
			);
      dtC.push_back(
				bSplineCurve_pointConstructOCC(
					dtPoint3(u0, m3d->getVMax(), m3d->getWMin()),			
					dtPoint3(u1, m3d->getVMax(), m3d->getWMax()) 
				).result()
			);						
      dtC.push_back(
				bSplineCurve_pointConstructOCC(
			    dtPoint3(u1, m3d->getVMax(), m3d->getWMax()),
					from[1]
				).result()
			);			
			dtCurve * tmpC = dtS->segmentConstU(dtS->maxU());
			tmpC->revert();
			dtC.push_back(tmpC);
      
      //
      // create surface
      //
			dtS1.reset( bSplineSurface_bSplineCurveFillConstructOCC(dtC).result() );
			
      //
      // create grid of surfaces and composite surface
      //
			twoDArrayHandling< dtSurface const * > grid(3, 1);
			grid[0][0] = dtS0.get();
			grid[1][0] = dtS.get();
			grid[2][0] = dtS1.get();
			dtS.reset( compositeSurface_surfaceConnectConstructOCC(grid).result() );			
			
      //
      // create result surface
      //
			_m2d.reset(
				new vec3dTwoDInMap3dTo3d(
			    dt__tmpPtr(vec3dSurfaceTwoD, new vec3dSurfaceTwoD(dtS.get())), m3d	
		    )			
			);
		}
    else {
			dt__throw(
			  map2dTo3d_fullExtentInMap3dTo3d(),
				<< "Case not yet implemented."
			);
		}
		
	}

	map2dTo3d_fullExtentInMap3dTo3d::~map2dTo3d_fullExtentInMap3dTo3d() {
	}
	
	map2dTo3d * map2dTo3d_fullExtentInMap3dTo3d::result( void ) {
		return _m2d->clone();
	}
}
