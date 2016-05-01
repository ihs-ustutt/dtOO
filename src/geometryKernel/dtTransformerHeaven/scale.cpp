#include "scale.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3dTransformed.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

namespace dtOO {
  scale::scale() : dtStrongTransformer() {
  }

  scale::~scale() {
  }
	
	scale::scale(const scale& orig) : dtStrongTransformer(orig) {
		_scale = orig._scale;
	}
	
  dtStrongTransformer * scale::clone( void ) const {
	  return new scale(*this);	
	}
	
  dtStrongTransformer * scale::create( void ) const {
		return new scale();
	}	

	std::vector< dtPoint3 > scale::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {      
		  ret[ii] 
      = 
      dtPoint3(
        _scale.x() * toTrans->at(ii).x(),
        _scale.y() * toTrans->at(ii).y(),
        _scale.z() * toTrans->at(ii).z()
      );
		}
		
		return ret;
	}
  
	std::vector< dtPoint3 > scale::retract( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {      
		  ret[ii]
      =
      dtPoint3(
        toTrans->at(ii).x() / _scale.x(),
        toTrans->at(ii).y() / _scale.y(),
        toTrans->at(ii).z() / _scale.z()
      );
		}
		
		return ret;
	}  
  
	std::vector< dtVector3 > scale::apply( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
		std::vector< dtVector3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {      
		  ret[ii] 
      = 
      dtVector3(
        _scale.x() * toTrans->at(ii).x(),
        _scale.y() * toTrans->at(ii).y(),
        _scale.z() * toTrans->at(ii).z()
      );
		}
		
		return ret;
	}
  
	std::vector< dtVector3 > scale::retract( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
		std::vector< dtVector3 > ret(toTrans->size());
		dt__forAllIndex(*toTrans, ii) {      
		  ret[ii]
      =
      dtVector3(
        toTrans->at(ii).x() / _scale.x(),
        toTrans->at(ii).y() / _scale.y(),
        toTrans->at(ii).z() / _scale.z()
      );
		}
		
		return ret;
	}  
	
  bool scale::isNecessary( void ) const {
    return true;
  }

  void scale::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtStrongTransformer::init(tE, bC, cV, aF, aG);
		
    dt__throwIf( !dtXmlParserBase::hasAttribute("scale_vector", *tE), init() );

    _scale 
    =
    dtXmlParserBase::getDtVector3(
		  dtXmlParserBase::getAttributeStr("scale_vector", *tE), bC 
	  );
    dt__info(
      init(),
      << "Init transfomer with " << dt__eval(_scale) << "."
    );
  }
}