#include "analyticSurface.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h>
#include "splineCurve3d.h"
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include "map1dTo3d.h"
#include <geometryEngine/dtCurve2d.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include "vec2dOneDInMap2dTo3d.h"
#include <discrete3dPoints.h>
#include <discrete3dVector.h>

namespace dtOO {
  analyticSurface::analyticSurface() : map2dTo3d() {
  }

  analyticSurface::analyticSurface(analyticSurface const & orig) : map2dTo3d(orig) {
    _dtS.reset( orig._dtS->clone() );
  }

  analyticSurface::analyticSurface(dtSurface const * const surface) : map2dTo3d() {
    _dtS.reset( surface->clone() );
  }
 
  analyticSurface * analyticSurface::clone( void ) const {
    return new analyticSurface( *this );
  }

  analyticSurface * analyticSurface::create( void ) const {
    return new analyticSurface();
  }

  analyticSurface::~analyticSurface() {
  }

  dtPoint3 analyticSurface::getPoint(float const & uu, float const & vv) const {
    return _dtS->point(uu, vv);
  }
  
  std::vector< dtPoint3 > analyticSurface::getPoint( std::vector< float > const & uu, std::vector< float > const & vv ) const {
    return _dtS->getPoint3d(uu, vv);
  }
	
  vectorHandling< renderInterface * > analyticSurface::getExtRender( void ) const {
		vectorHandling< dtVector3 > vecV;
		vectorHandling< dtPoint3 > vecP;
		vectorHandling< dtVector3 > norV;
		vectorHandling< dtPoint3 > norP;
		vectorHandling< dtPoint3 > pp;		
    //
    // get surface directions
    //
    dtPoint3 startPointU = map2dTo3d::getPointPercent(0.05, 0.); //getDtSislSurf()->getPointPercent3d(0.05, 0.);
    dtPoint3 topPointU = map2dTo3d::getPointPercent(0.1, 0.);//getDtSislSurf()->getPointPercent3d(0.1, 0.);
    dtPoint3 startPointV = map2dTo3d::getPointPercent(0., 0.05);//getDtSislSurf()->getPointPercent3d(0., 0.05);
    dtPoint3 topPointV = map2dTo3d::getPointPercent(0., 0.10);//getDtSislSurf()->getPointPercent3d(0., 0.10);
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

    //
    // get control points
    //
    int numPoints = _dtS->nControlPoints();
    dtPoint3 tmpControlPoint;
    for (int ii=0; ii<numPoints; ii++) {
      tmpControlPoint = _dtS->controlPoint(ii);
			pp.push_back(tmpControlPoint);
    }
		pp.push_back(map2dTo3d::getPointPercent(0., .5));
		pp.push_back(map2dTo3d::getPointPercent(.5, 0.));

		vectorHandling< renderInterface * > retVec(3);
		retVec[0] = new discrete3dPoints(pp);
		retVec[1] = new discrete3dVector(norV, norP);
		retVec[2] = new discrete3dVector(vecV, vecP);
		
		return retVec;
  }

  dtSurface * analyticSurface::ptrDtSurface( void ) const {
    return _dtS.get();
  }

  dtSurface const * analyticSurface::constPtrDtSurface( void ) const {
    return _dtS.get();
  }   
  
  void analyticSurface::revert( void ) {
    _dtS->revert();
  }
  
  void analyticSurface::offsetNormal( float const nn ) {
    _dtS->offsetNormal( nn );
  }
    
  bool analyticSurface::isClosed( int const & dir) const {
    return _dtS->closed(dir);
  }
  
  float analyticSurface::getMin( int const & dir) const {
		return _dtS->minPara(dir);
  }

  float analyticSurface::getMax( int const & dir) const {
    return _dtS->maxPara(dir);
  }
  
  dtVector3 analyticSurface::normal( float const & uu, float const & vv) const {
    return _dtS->normal(uu, vv);
  }
    
  dtVector3 analyticSurface::firstDerU( float const & uu, float const & vv) const {
    return _dtS->getVectorDdU3d(uu, vv);
  }
  
  dtVector3 analyticSurface::firstDerV( float const & uu, float const & vv) const {
    return _dtS->getVectorDdV3d(uu, vv);
  }
  
  dtVector3 analyticSurface::secondDerUU( float const & uu, float const & vv) const {
    return _dtS->getVectorDdUDdU3d(uu, vv);
  }
  
  dtVector3 analyticSurface::secondDerVV( float const & uu, float const & vv) const {
    return _dtS->getVectorDdVDdV3d(uu, vv);
  }
    
  dtVector3 analyticSurface::secondDerUV( float const & uu, float const & vv) const {
    return _dtS->getVectorDdUDdV3d(uu, vv);
  }
  
  dtPoint2 analyticSurface::reparamOnFace(dtPoint3 const & ppXYZ) const {
    return _dtS->reparam( ppXYZ );
  }
  
  map1dTo3d * analyticSurface::pickConstUPercent(
    float const & uu, float const & vv0, float const & vv1
  ) const {
    ptrHandling< dtCurve > cc(
      _dtS->getCurveConstUPercent(uu, vv0, vv1)
    );
    return new splineCurve3d(cc.get());
  }
  
  map1dTo3d * analyticSurface::pickConstVPercent(
      float const & vv, float const & uu0, float const & uu1
    ) const {
      ptrHandling< dtCurve > cc(
        _dtS->getCurveConstVPercent(vv, uu0, uu1)
      );
    return new splineCurve3d(cc.get());
  }
  
  map1dTo3d * analyticSurface::pickLinearUV( float const & uu0, float const & vv0,
                                             float const & uu1, float const & vv1 
  ) const {
		dtPoint2 p0(uu0, vv0);
		dtPoint2 p1(uu1, vv1);
		ptrHandling<dtCurve2d> dtC2d(
		  trimmedCurve2d_twoPointsConnectConstructOCC(p0, p1).result()
		);
		ptrHandling<vec2dCurve2dOneD> v2d1d( new vec2dCurve2dOneD(dtC2d.get()) );
		
		return new vec2dOneDInMap2dTo3d(v2d1d.get(), this);
  }
  
  map2dTo3d * analyticSurface::pickPercent(
      float const & uu0, float const & uu1, 
      float const & vv0, float const & vv1
  ) const {
    ptrHandling< dtSurface > ss(		
			rectangularTrimmedSurface_uvBounds(
				_dtS.get(), 
				dtPoint2(u_percent(uu0), v_percent(vv0)), 
				dtPoint2(u_percent(uu1), v_percent(vv1))
			).result()
		);		
    return new analyticSurface(ss.get());
  }
}
