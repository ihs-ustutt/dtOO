#include "rotate.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

namespace dtOO {
  rotate::rotate() : dtStrongTransformer() {
  }

  rotate::~rotate() {
  }
	
	rotate::rotate(const rotate& orig) : dtStrongTransformer(orig) {
		_angle = orig._angle;
		_origin = orig._origin;
		_rotVector = orig._rotVector;
	}
	
  dtStrongTransformer * rotate::clone( void ) const {
	  return new rotate(*this);	
	}
	
  dtStrongTransformer * rotate::create( void ) const {
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
	
  vectorHandling< analyticGeometry * > rotate::apply( 
	  vectorHandling< analyticGeometry * > const * const aGeoVecP 
	) const {
    vectorHandling< analyticGeometry * > retAGeo;

    dt__forAllConstIter(vectorHandling< analyticGeometry * >, *aGeoVecP, it) {
			//
			// clone and cast analyticGeometry
			//
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(*it);
      analyticSurface const * const aS = analyticSurface::ConstDownCast(*it);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(*it);
      
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
			else if (m2d) retAGeo.push_back( m2d->cloneTransformed(this) );
			else if (m3d) retAGeo.push_back( m3d->cloneTransformed(this) );        
      else dt__throwUnexpected(apply());
    }
    return retAGeo;
  }

  bool rotate::isNecessary( void ) const {
    return true;
  }

  void rotate::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
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