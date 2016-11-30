#include "xYz_rPhiZ.h"
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
  xYz_rPhiZ::xYz_rPhiZ() : dtTransformer() {
  }

  xYz_rPhiZ::xYz_rPhiZ( xYz_rPhiZ const & orig ) : dtTransformer(orig) {
    _rotAxis = orig._rotAxis;
    _refAxis = orig._refAxis;
    _origin = orig._origin;
  }
  
  xYz_rPhiZ::xYz_rPhiZ( 
    dtPoint3 origin, dtVector3 rotAxis, dtVector3 refAxis 
  ) : dtTransformer() {
    _rotAxis = rotAxis;
    _refAxis = refAxis;
    _origin = origin;
  }  
	
  xYz_rPhiZ::~xYz_rPhiZ() {
		
  }

  dtTransformer * xYz_rPhiZ::clone( void ) const {
	  return new xYz_rPhiZ(*this);
	}
	
  dtTransformer * xYz_rPhiZ::create( void ) const {
		return new xYz_rPhiZ();
	}
	
	std::vector< dtPoint3 > xYz_rPhiZ::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllRefAuto(*toTrans, aPoint) {
      float coordRef1 = aPoint.x() * cos(aPoint.y());
      float coordRef2 = aPoint.x() * sin(aPoint.y());
      float coordZ = aPoint.z();

      retVec.push_back( dtPoint3(coordRef1, coordRef2, coordZ) );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > xYz_rPhiZ::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllRefAuto(*toRetract, aPoint) {
      // define vector from origin to point
      dtVector3 dd = aPoint - _origin;
      
      //
      // z coordinate
      //
      float coordZ = dtLinearAlgebra::dotProduct(_rotAxis, dd);

      //
      // r coordinate
      //      
      dtVector3 rr = dd - coordZ * _rotAxis;
      float coordR = dtLinearAlgebra::length(rr);
      
      //
      // reference and reference2 axis
      //
      dtVector3 ref2Axis 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(_rotAxis, _refAxis)
      );
            
      //
      // phi coordinate
      //
      float coordPhi 
      = 
      atan2( 
        dtLinearAlgebra::dotProduct(ref2Axis, rr), 
        dtLinearAlgebra::dotProduct(_refAxis, rr)
      );          

			retVec.push_back( dtPoint3(coordR, coordPhi, coordZ) );
		}
		
		return retVec;
	}
  
  dtVector3 const & xYz_rPhiZ::rotationAxis( void ) const {
    return _rotAxis;
  }
  
  dtVector3 const & xYz_rPhiZ::referenceAxis( void ) const {
    return _refAxis;
  }
  
  dtPoint3 const & xYz_rPhiZ::origin( void ) const {
    return _origin;
  }
    
  bool xYz_rPhiZ::isNecessary( void ) const {
    return true;
  }

  void xYz_rPhiZ::init( 
    ::QDomElement const * tE, 
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG 
	) {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("origin", *tE)
      ||
      !dtXmlParserBase::hasAttribute("rotationAxis", *tE)
      ||
      !dtXmlParserBase::hasAttribute("referenceAxis", *tE),
      init()
    );
    
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    _origin
    =
	  dtXmlParserBase::getDtPoint3(
      dtXmlParserBase::getAttributeStr("origin", *tE), bC
    );
    _rotAxis
    =
	  dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("rotationAxis", *tE), bC
      )
    );
    _refAxis
    =
    dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("referenceAxis", *tE), bC
      )
    );    
  }
}