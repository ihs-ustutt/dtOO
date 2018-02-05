#include "rotate.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3dTransformed.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

namespace dtOO {
  rotate::rotate() : dtTransformerInvThreeD() {
  }

  rotate::~rotate() {
  }
	
	rotate::rotate(const rotate& orig) : dtTransformerInvThreeD(orig) {
		_angle = orig._angle;
		_origin = orig._origin;
		_rotVector = orig._rotVector;
	}
	
  dtTransformerInvThreeD * rotate::clone( void ) const {
	  return new rotate(*this);	
	}
	
  dtTransformerInvThreeD * rotate::create( void ) const {
		return new rotate();
	}	

	std::vector< dtPoint3 > rotate::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		dtAffTransformation3 rot 
    = 
    dtLinearAlgebra::getRotation(_rotVector, _angle);
	
		std::vector< dtPoint3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {
			dtVector3 vv = rot.transform( toTrans->at(ii) - _origin );
		  ret[ii] = _origin + vv;
		}
		
		return ret;
	}
  
	std::vector< dtPoint3 > rotate::retract( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		dtAffTransformation3 rot 
    = 
    dtLinearAlgebra::getRotation(_rotVector, -1.*_angle);
	
		std::vector< dtPoint3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {
			dtVector3 vv = rot.transform( toTrans->at(ii) - _origin );
		  ret[ii] = _origin + vv;

      dt__debug(
        retract(),
        << "Retracting point " << dt__point3d(toTrans->at(ii)) << std::endl
        << "to point " << dt__point3d(ret[ii])
      );      
		}
		
		return ret;
	}  
  
	std::vector< dtVector3 > rotate::apply( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
		dtAffTransformation3 rot 
    = 
    dtLinearAlgebra::getRotation(_rotVector, _angle);
	
		std::vector< dtVector3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {
			dtVector3 vv = rot.transform( toTrans->at(ii) );
		  ret[ii] = vv;
		}
		
		return ret;
	}
  
	std::vector< dtVector3 > rotate::retract( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
		dtAffTransformation3 rot 
    = 
    dtLinearAlgebra::getRotation(_rotVector, -1.*_angle);
	
		std::vector< dtVector3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {
			dtVector3 vv = rot.transform( toTrans->at(ii) );
		  ret[ii] = vv;

      dt__debug(
        retract(),
        << "Retracting vector " << dt__point3d(toTrans->at(ii)) << std::endl
        << "to point " << dt__point3d(ret[ii])
      );      
		}
		
		return ret;
	}  
	
  aGPtrVec rotate::apply( 
	  aGPtrVec const * const aGeoVecP 
	) const {
    aGPtrVec retAGeo;

    dt__forAllConstIter(aGPtrVec, *aGeoVecP, it) {
			//
			// clone and cast analyticGeometry
			//
      map1dTo3d const * const m1d = map1dTo3d::ConstDownCast(*it);      
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(*it);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(*it);
      
      if (m1d) {
        analyticCurve const * const s3 = analyticCurve::ConstDownCast(*it);
        if ( s3 &&  !s3->isTransformed() ) {
          retAGeo.push_back( 
            new analyticCurve(
              dt__tmpPtr(
                dtCurve,
                geomCurve_curveRotateConstructOCC( 
                  s3->ptrConstDtCurve(), _origin, _rotVector, _angle 
                ).result()
              )
            ) 
          );
        }
        else {
          retAGeo.push_back( m1d->cloneTransformed(this) );
        }
      }
			else if (m2d) {
        analyticSurface const * const aS = analyticSurface::ConstDownCast(*it);
        if ( aS &&  !aS->isTransformed() ) {
          retAGeo.push_back( 
            new analyticSurface(
              dt__tmpPtr(
                dtSurface,
                geomSurface_surfaceRotateConstructOCC( 
                  aS->ptrDtSurface(), _origin, _rotVector, _angle 
                ).result()
              )
            ) 
          );
        }
        else {        
          retAGeo.push_back( m2d->cloneTransformed(this) );
        }
      }
			else if (m3d) {
        retAGeo.push_back( m3d->cloneTransformed(this) );
      }
      else dt__throwUnexpected(apply());
    }
    return retAGeo;
  }

  bool rotate::isNecessary( void ) const {
    return true;
  }

  void rotate::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    if (dtXmlParserBase::hasAttribute("origin", *tE)) {
      _origin 
			= 
			dtXmlParserBase::getDtPoint3(
				dtXmlParserBase::getAttributeStr("origin", *tE), bC 
			);
    }
    if (dtXmlParserBase::hasAttribute("rotation_vector", *tE)) {
      _rotVector
			= 
			dtXmlParserBase::getDtVector3(
				dtXmlParserBase::getAttributeStr("rotation_vector", *tE), bC 
			);
    }
    if (dtXmlParserBase::hasAttribute("angle", *tE)) {
      _angle 
			= 
			dtXmlParserBase::getAttributeFloatMuParse("angle", *tE, cV, aF);
    }
    dt__debug(
			init(),
      << dt__point3d(_origin) << std::endl
      << dt__vector3d(_rotVector) << std::endl
      << dt__eval(_angle) 
		);
  }
}