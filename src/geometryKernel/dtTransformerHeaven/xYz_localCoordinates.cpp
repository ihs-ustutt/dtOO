#include "xYz_localCoordinates.h"
#include "interfaceHeaven/systemHandling.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>

namespace dtOO {
  xYz_localCoordinates::xYz_localCoordinates() : dtTransformer() {
  }

  xYz_localCoordinates::xYz_localCoordinates( xYz_localCoordinates const & orig ) : dtTransformer(orig) {
    _e1 = orig._e1;
    _e2 = orig._e2;
    _e3 = orig._e3;
    _origin = orig._origin;
  }
  
  xYz_localCoordinates::xYz_localCoordinates( 
    dtPoint3 origin, dtVector3 e1, dtVector3 e2 
  ) : dtTransformer() {
    _origin = origin;
    _e1 = e1;
    _e2 = e2;
  }  
	
  xYz_localCoordinates::~xYz_localCoordinates() {
		
  }

  dtTransformer * xYz_localCoordinates::clone( void ) const {
	  return new xYz_localCoordinates(*this);
	}
	
  dtTransformer * xYz_localCoordinates::create( void ) const {
		return new xYz_localCoordinates();
	}
	
	std::vector< dtPoint3 > xYz_localCoordinates::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllRefAuto(*toTrans, aPoint) {
      retVec.push_back(
        _origin + aPoint.x() * _e1 + aPoint.y() * _e2 + aPoint.z() * _e3
      );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > xYz_localCoordinates::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllRefAuto(*toRetract, aPoint) {
      dtVector3 cc = aPoint - _origin;
      retVec.push_back(
        dtPoint3(
          dtLinearAlgebra::dotProduct(cc, _e1),
          dtLinearAlgebra::dotProduct(cc, _e2),
          dtLinearAlgebra::dotProduct(cc, _e3)
        )
      );
		}
		
		return retVec;
	}
    
  bool xYz_localCoordinates::isNecessary( void ) const {
    return true;
  }

  void xYz_localCoordinates::init( 
    ::QDomElement const * tE, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG 
	) {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("origin", *tE)
      ||
      !dtXmlParserBase::hasAttribute("e1", *tE)
      ||
      !dtXmlParserBase::hasAttribute("e2", *tE),
      init()
    );
    
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    _origin
    =
	  dtXmlParserBase::getDtPoint3(
      dtXmlParserBase::getAttributeStr("origin", *tE), bC
    );
    _e1
    =
	  dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("e1", *tE), bC
      )
    );
    _e2
    =
    dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("e2", *tE), bC
      )
    );    
    
    _e3 = dtLinearAlgebra::crossProduct(_e1, _e2);
  }
}