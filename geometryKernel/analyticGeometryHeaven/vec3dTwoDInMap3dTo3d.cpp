#include "vec3dTwoDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map3dTo3d.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include "map2dTo3dTransformed.h"
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <progHelper.h>

namespace dtOO {    
  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d() : map2dTo3d() {
  }

  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(
    const vec3dTwoDInMap3dTo3d& orig
  ) : map2dTo3d(orig) { 
    _v2d.reset( orig._v2d->clone() );
    _m3d.reset( orig._m3d->clone() );
		_percentF = orig._percentF;
		
		labelHandling::setLabel( _v2d->getLabel()+"_in_"+_m3d->getLabel() );
  }
	
  vec3dTwoDInMap3dTo3d::vec3dTwoDInMap3dTo3d(    
	  vec3dTwoD const * const v2d, map3dTo3d const * const m3d, bool percentF
  ) {
    _v2d.reset( v2d->clone() );
    _m3d.reset( m3d->clone() );
		_percentF = percentF;
		
		labelHandling::setLabel( _v2d->getLabel()+"_in_"+_m3d->getLabel() );		
	}

  vec3dTwoDInMap3dTo3d::~vec3dTwoDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dTwoDInMap3dTo3d::getPoint( 
    float const & uu, float const & vv 
  ) const {
		aFX xx(2,0);
		xx[0] = uu; xx[1] = vv;
    dtPoint3 pUVW = _v2d->YdtPoint3(xx);
		if (!_percentF) {
      return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
		}
		else {
			return _m3d->getPointPercent( pUVW.x(), pUVW.y(), pUVW.z() );
		}		
  }
  
  bool vec3dTwoDInMap3dTo3d::isClosed( int const & dir) const {
		return _v2d->closed(dir);
  }
  
  float vec3dTwoDInMap3dTo3d::getMin( int const & dir) const {
    return _v2d->xMin(dir);    
  }

  float vec3dTwoDInMap3dTo3d::getMax( int const & dir) const {
    return _v2d->xMax(dir);    
  }

  vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::clone( void ) const {
    return new vec3dTwoDInMap3dTo3d(*this);
  }
	
  vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::create( void ) const {
    return new vec3dTwoDInMap3dTo3d();
  }
	
	vec3dTwoDInMap3dTo3d * vec3dTwoDInMap3dTo3d::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
    return new map2dTo3dTransformed<vec3dTwoDInMap3dTo3d>(*this, dtT);
  }
  
  vec3dTwoD const * vec3dTwoDInMap3dTo3d::refToVec3dTwoD( void ) const {
    return _v2d.get();
  }
  
  map3dTo3d const * vec3dTwoDInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  

  vectorHandling< renderInterface * > vec3dTwoDInMap3dTo3d::getExtRender( 
    void 
  ) const {
		vectorHandling< dtVector3 > vecV;
		vectorHandling< dtPoint3 > vecP;
		vectorHandling< dtVector3 > norV;
		vectorHandling< dtPoint3 > norP;
//		vectorHandling< dtPoint3 > pp;		
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

    //
    // get control points
    //
//		dtSurface const * const dtS = _v2d->ptrDtSurface();
//    int numPointsU = dtS->nControlPoints(0);
//		int numPointsV = dtS->nControlPoints(1);
//    for (int ii=0; ii<numPointsU; ii++) {
//			for (int jj=0; jj<numPointsV; jj++) {
//				pp.push_back( _m3d->getPoint( dtS->controlPoint(ii, jj) ) );
//			}
//    }
		vectorHandling< renderInterface * > retVec(2);
//		retVec[0] = new discrete3dPoints(pp);
		retVec[0] = new discrete3dVector(norV, norP);
		retVec[1] = new discrete3dVector(vecV, vecP);
		
		return retVec;
  }	
  
	std::string vec3dTwoDInMap3dTo3d::dumpToString( void ) const {
		std::stringstream ss;
		
		ss 
		<< dt__eval(_m3d->virtualClassName()) << std::endl
	  << _m3d->dumpToString() << std::endl
    << dt__eval(_v2d->virtualClassName()) << std::endl
		<< _v2d->dumpToString() << std::endl
    << dt__eval(_percentF);
		
		return ss.str();
	}
  
  dtPoint2 vec3dTwoDInMap3dTo3d::reparamOnFace(dtPoint3 const & ppXYZ) const {
    dtPoint3 ppUVW = _m3d->reparamInVolume(ppXYZ);
    aFX xx = _v2d->invY( analyticFunction::aFYThreeD(ppUVW) );
    
    dtPoint3 ppXYZReparam;
    if (!_percentF) ppXYZReparam = getPoint(xx[0], xx[1]);
    else ppXYZReparam = getPointPercent(xx[0], xx[1]);
    
    analyticGeometry::inXYZTolerance(ppXYZ, ppXYZReparam);

    return dtPoint2(xx[0], xx[1]);
  }
}
