#include "trans6SidedCube_splitTrans6SidedCube.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h>

#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
	trans6SidedCube_splitTrans6SidedCube
	  ::trans6SidedCube_splitTrans6SidedCube(
      trans6SidedCube const * const t6, dtPoint3 const & splitPoint
	) {
    std::vector< bool > keep(3, false);
    // keep u
    dt__forFromToIndex(0, 3, i) {
      if ( (splitPoint[i] == 0.) || (splitPoint[i] == 1.) ) keep[i] = true;
    }
    dt__throwIf( 
      std::count(keep.begin(), keep.end(), false)!=1, 
      trans6SidedCube_splitTrans6SidedCube()
    );
    
    // u split
    if (keep[0] == false) {
      dt__pH(analyticCurve) e0(
        analyticCurve::MustDownCast(
          t6->constRefMap2dTo3d()[0]->segmentConstUPercent( splitPoint.x() )
        )
      );
      dt__pH(analyticCurve) e2(
        analyticCurve::MustDownCast(
          t6->constRefMap2dTo3d()[2]->segmentConstUPercent( splitPoint.x() )
        )
      );      
      dt__pH(analyticCurve) e4(
        analyticCurve::MustDownCast(
          t6->constRefMap2dTo3d()[4]->segmentConstUPercent( splitPoint.x() )
        )
      );      
      dt__pH(analyticCurve) e5(
        analyticCurve::MustDownCast(
          t6->constRefMap2dTo3d()[5]->segmentConstUPercent( splitPoint.x() )
        )
      );
      dt__pH(analyticSurface) splitFace(
        new analyticSurface(
          dt__pH(dtSurface)(
            bSplineSurface_geomCurveFillConstructOCC(
              e2->ptrConstDtCurve(), e5->ptrConstDtCurve(),
              e0->ptrConstDtCurve(), e4->ptrConstDtCurve()
            ).result()
          ).get()
        )
      );
      
      // 2
      dt__pH(analyticSurface) f2_0(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[2]->segmentRectanglePercent(
            dtPoint2(0,0), dtPoint2(splitPoint.x(),1) 
          )
        )
      );
      dt__pH(analyticSurface) f2_1(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[2]->segmentRectanglePercent(
            dtPoint2(splitPoint.x(),0), dtPoint2(1,1)
          )
        )
      );  
      // 0
      dt__pH(analyticSurface) f0_0(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[0]->segmentRectanglePercent(
            dtPoint2(0,0), dtPoint2(splitPoint.x(),1) 
          )
        )
      );
      dt__pH(analyticSurface) f0_1(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[0]->segmentRectanglePercent(
            dtPoint2(splitPoint.x(),0), dtPoint2(1,1)
          )
        )
      );  
      // 4
      dt__pH(analyticSurface) f4_0(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[4]->segmentRectanglePercent(
            dtPoint2(0,0), dtPoint2(splitPoint.x(),1) 
          )
        )
      );
      dt__pH(analyticSurface) f4_1(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[4]->segmentRectanglePercent(
            dtPoint2(splitPoint.x(),0), dtPoint2(1,1)
          )
        )
      );
      // 5
      dt__pH(analyticSurface) f5_0(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[5]->segmentRectanglePercent(
            dtPoint2(0,0), dtPoint2(splitPoint.x(),1) 
          )
        )
      );
      dt__pH(analyticSurface) f5_1(
        analyticSurface::MustDownCast(
          t6->constRefMap2dTo3d()[5]->segmentRectanglePercent(
            dtPoint2(splitPoint.x(),0), dtPoint2(1,1)
          )
        )
      );      
      
      _aG.push_back( e2->clone() );
      _aG.push_back( e5->clone() );
      _aG.push_back( e0->clone() );
      _aG.push_back( e4->clone() );
      _aG.push_back( splitFace->clone() );
      _aG.push_back( f2_0->clone() );
      _aG.push_back( f2_1->clone() );
      _aG.push_back( f5_0->clone() );
      _aG.push_back( f5_1->clone() );
      _aG.push_back( f0_0->clone() );
      _aG.push_back( f0_1->clone() );
      _aG.push_back( f4_0->clone() );
      _aG.push_back( f4_1->clone() );      
      
      _t6.push_back(
        new trans6SidedCube(
          f0_0.get(), t6->constRefMap2dTo3d()[1],
          f2_0.get(), splitFace.get(),
          f4_0.get(), f5_0.get()
        )
      );
      _t6.push_back(
        new trans6SidedCube(
          f0_1.get(), splitFace.get(),
          f2_1.get(), t6->constRefMap2dTo3d()[3],
          f4_1.get(), f5_1.get()
        )
      );
    }
    
//    if ( (splitPoint.y() == 0.) || (splitPoint.y() == 1.) ) keep[1] = true;    
//    if ( (splitPoint.z() == 0.) || (splitPoint.z() == 1.) ) keep[2] = true;    
    
//    dt__throwUnexpected(trans6SidedCube_splitTrans6SidedCube());
//    vec3dTwoDInMap3dTo3d const * const vInMap
//		=
//		vec3dTwoDInMap3dTo3d::ConstDownCast(m2d);
//		dt__throwIf(!vInMap, trans6SidedCube_splitTrans6SidedCube());
//		map3dTo3d const * const m3d = vInMap->refToMap3dTo3d();
//		vec3dTwoD const * const v3d = vInMap->refToVec3dTwoD();
//		vec3dSurfaceTwoD const * const v3dS = vec3dSurfaceTwoD::ConstDownCast(v3d);
//		dt__throwIf(!v3dS, trans6SidedCube_splitTrans6SidedCube());
//
//		if ( (m2dParDir == 0) && (toM3dParDir == 1) ) {
//			dt__pH(dtSurface) dtS0;
//			dt__pH(dtSurface) dtS( v3dS->ptrDtSurface()->clone() );
//      dt__pH(dtSurface) dtS1;
//			
//      //
//      // get derivative and approximate straight extension to the front
//      //
//		  std::vector< dtPoint3 > from(2);
//			vectorHandling< dtCurve const * > dtC;
//      from[0] = v3d->YdtPoint3Percent(0., 0.);      
//			from[1] = v3d->YdtPoint3Percent(0., 1.);
//			
//			std::vector< dtVector3 > dY0 
//			= 
//			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(0., 0.) );			
//      std::vector< dtVector3 > dY1 
//			= 
//			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(0., 1.) );
//
//      bool isPos = false;
//      if ( (dY0[0].x()>0.) && (dY0[1].x()>0.) ) isPos = true;
//      
//			float t0 = (from[0].y() - m3d->getVMin())/dY0[0].y();
//			float t1 = (m3d->getVMin() - from[1].y())/dY1[0].y();
//			float u0max = from[0].x()-t0*dY0[0].x();
//			float u1max = from[1].x()+t1*dY1[0].x();
//      float u0 = u0max;
//      float u1 = u1max;      
//      if (isPos) {
//        u0 = std::min(u0max, u1max);
//        u1 = u0;
//      }
//      else {
//        u0 = std::max(u0max, u1max);
//        u1 = u0;
//      }
//      
//      //
//      // create 4 curves for inlet extension
//      //
//			dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//					dtPoint3(u0, m3d->getVMin(), m3d->getWMin()), 
//					from[0]
//				).result()
//			);
//			dtC.push_back( dtS->segmentConstUPercent(0.) );
//      dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//					from[1], 
//					dtPoint3(u1, m3d->getVMin(), m3d->getWMax())
//				).result()
//			);			
//      dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//					dtPoint3(u1, m3d->getVMin(), m3d->getWMax()), 
//					dtPoint3(u0, m3d->getVMin(), m3d->getWMin())
//				).result()
//			);			
//      
//      //
//      // create inlet surface
//      //
//			dtS0.reset( bSplineSurface_bSplineCurveFillConstructOCC(dtC).result() );
//	
//      //
//      // get derivative and approximate straight extension to the back
//      //      
//			dtC.destroy();
//      from[0] = v3d->YdtPoint3Percent(1., 0.);      
//			from[1] = v3d->YdtPoint3Percent(1., 1.);
//
//			dY0 
//			= 
//			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(1., 0.) );			
//      dY1 
//			= 
//			v3d->DYdtVector3( (*v3d) % analyticFunction::aFXTwoD(1., 1.) );
//
//      isPos = false;
//      if ( (dY0[0].x()>0.) && (dY0[1].x()>0.) ) isPos = true;
//      
//			t0 = (from[0].y() - m3d->getVMax())/dY0[0].y();
//			t1 = (m3d->getVMax() - from[1].y())/dY1[0].y();
//			u0max = from[0].x()-t0*dY0[0].x();
//      u1max = from[1].x()+t1*dY1[0].x();
//      if (isPos) {
//        u0 = std::min(u0max, u1max);
//        u1 = u0;
//      }
//      else {
//        u0 = std::max(u0max, u1max);
//        u1 = u0;
//      }
//      
//      //
//      // create 4 curves for outlet extension
//      //
//			dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//					from[0],
//					dtPoint3(u0, m3d->getVMax(), m3d->getWMin())
//				).result()
//			);
//      dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//					dtPoint3(u0, m3d->getVMax(), m3d->getWMin()),			
//					dtPoint3(u1, m3d->getVMax(), m3d->getWMax()) 
//				).result()
//			);						
//      dtC.push_back(
//				bSplineCurve_pointConstructOCC(
//			    dtPoint3(u1, m3d->getVMax(), m3d->getWMax()),
//					from[1]
//				).result()
//			);			
//			dtCurve * tmpC = dtS->segmentConstU(dtS->maxU());
//			tmpC->revert();
//			dtC.push_back(tmpC);
//      
//      //
//      // create surface
//      //
//			dtS1.reset( bSplineSurface_bSplineCurveFillConstructOCC(dtC).result() );
//			
//      //
//      // create grid of surfaces and composite surface
//      //
//			twoDArrayHandling< dtSurface const * > grid(3, 1);
//			grid[0][0] = dtS0.get();
//			grid[1][0] = dtS.get();
//			grid[2][0] = dtS1.get();
//			dtS.reset( compositeSurface_surfaceConnectConstructOCC(grid).result() );			
//			
//      //
//      // create result surface
//      //
//			_m2d.reset(
//				new vec3dTwoDInMap3dTo3d(
//			    dt__tmpPtr(vec3dSurfaceTwoD, new vec3dSurfaceTwoD(dtS.get())), m3d	
//		    )			
//			);
//		}
//    else {
//			dt__throw(
//			  trans6SidedCube_splitTrans6SidedCube(),
//				<< "Case not yet implemented."
//			);
//		}
//		
	}

	trans6SidedCube_splitTrans6SidedCube
    ::~trans6SidedCube_splitTrans6SidedCube() {
	}
	
	vectorHandling< trans6SidedCube * > trans6SidedCube_splitTrans6SidedCube
    ::result( void ) {
    vectorHandling< trans6SidedCube * > ret;
		dt__forAllRefAuto( _t6, t6 ) ret.push_back( t6.clone() );
    return ret;
	}
  
	vectorHandling< analyticGeometry * > trans6SidedCube_splitTrans6SidedCube
    ::resultGeometries( void ) {
    vectorHandling< analyticGeometry * > ret;
		dt__forAllRefAuto( _aG, aG ) ret.push_back( aG.clone() );
    return ret;
	}  
}
