#include "vec3dOneDInMap3dTo3d.h"

#include <logMe/logMe.h>
#include "map1dTo3d.h"
#include "map3dTo3d.h"
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <interfaceHeaven/ptrVectorHandling.h>
#include <geometryEngine/dtCurve.h>
#include <logMe/dtMacros.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include "map1dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO {    
  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d() : map1dTo3d() {
  }

  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(
    const vec3dOneDInMap3dTo3d& orig
  ) : map1dTo3d(orig) { 
    _v1d.reset( orig._v1d->clone() );
    _m3d.reset( orig._m3d->clone() );
		_percentF = orig._percentF;
  }

  vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(    
	  vec3dOneD const * const v1d, map3dTo3d const * const m3d, bool percentF
  ) {
    _v1d.reset( v1d->clone() );
    _m3d.reset( m3d->clone() );
		_percentF = percentF;
	}
	
  vec3dOneDInMap3dTo3d::~vec3dOneDInMap3dTo3d() {
  }
  
  dtPoint3 vec3dOneDInMap3dTo3d::getPoint( float const & uu ) const {
    dtPoint3 pUVW = _v1d->YdtPoint3(uu);
		if (!_percentF) {
      return _m3d->getPoint( pUVW.x(), pUVW.y(), pUVW.z() );
		}
		else {
			return _m3d->getPointPercent( pUVW.x(), pUVW.y(), pUVW.z() );
		}				
  }
  
  bool vec3dOneDInMap3dTo3d::isClosed( int const & dir) const {
    return _v1d->closed(dir);
  }
  
  float vec3dOneDInMap3dTo3d::getMin( int const & dir) const {
    return _v1d->xMin(dir);    
  }

  float vec3dOneDInMap3dTo3d::getMax( int const & dir) const {
    return _v1d->xMax(dir);    
  }
  
  vec3dOneDInMap3dTo3d * vec3dOneDInMap3dTo3d::create( void ) const {
    return new vec3dOneDInMap3dTo3d();
  }
  
  vec3dOneDInMap3dTo3d * vec3dOneDInMap3dTo3d::clone( void ) const {
    return new vec3dOneDInMap3dTo3d(*this);
  }

	vec3dOneDInMap3dTo3d * vec3dOneDInMap3dTo3d::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map1dTo3dTransformed<vec3dOneDInMap3dTo3d>(*this, dtT);
	}  
    
  vec3dOneD const * vec3dOneDInMap3dTo3d::refToVec3dOneD( void ) const {
    return _v1d.get();
  }
  
  map3dTo3d const * vec3dOneDInMap3dTo3d::refToMap3dTo3d( void ) const {
    return _m3d.get();
  }  

  vectorHandling< renderInterface * > vec3dOneDInMap3dTo3d::getExtRender( void ) const {
		vectorHandling< dtVector3 > vv;
		vectorHandling< dtPoint3 > vp;
//		vectorHandling< dtPoint3 > pp;
    //
    // get spline direction
    //
    dtPoint3 startPoint = getPointPercent(0.05);
    dtPoint3 topPoint = getPointPercent(0.1);
    dtVector3 uu = topPoint - startPoint;

    //
    // add direction vector to container
    //    
		vv.push_back(uu);
		vp.push_back(startPoint);

    //
    // get control points
    //
//		vec3dCurveOneD const * const v3dC1d = vec3dCurveOneD::ConstDownCast(_v1d.get());
//		if (v3dC1d) {
//			dtCurve const * const dtC = v3dC1d->ptrDtCurve();
//			int numPointsU = dtC->getNControlPoints();
//			for (int ii=0; ii<numPointsU; ii++) {
//			  pp.push_back( _m3d->getPoint( dtC->getControlPoint3d(ii) ) );
//			}
//		}

		vectorHandling< renderInterface * > retVec(1);
//		retVec[0] = new discrete3dPoints(pp);
		retVec[0] = new discrete3dVector(vv, vp);
		return retVec;
  }	
}
