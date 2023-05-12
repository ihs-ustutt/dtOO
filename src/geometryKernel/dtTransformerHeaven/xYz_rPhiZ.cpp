#include "xYz_rPhiZ.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool xYz_rPhiZ::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(xYz_rPhiZ, new xYz_rPhiZ())
  );
  
  xYz_rPhiZ::xYz_rPhiZ() : dtTransformer() {
  }

  xYz_rPhiZ::xYz_rPhiZ( xYz_rPhiZ const & orig ) : dtTransformer(orig) {
  }
  
  xYz_rPhiZ::xYz_rPhiZ( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
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
      dtReal coordRef1 = aPoint.x() * cos(aPoint.y());
      dtReal coordRef2 = aPoint.x() * sin(aPoint.y());
      dtReal coordZ = aPoint.z();

      retVec.push_back( dtPoint3(coordRef1, coordRef2, coordZ) );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > xYz_rPhiZ::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
    dtVector3 rotAxis = config().lookup<dtVector3>("_rotAxis");
    dtVector3 refAxis = config().lookup<dtVector3>("_refAxis");
    dtPoint3 origin = config().lookup<dtPoint3>("_origin");
		dt__forAllRefAuto(*toRetract, aPoint) {
      // define vector from origin to point
      dtVector3 dd = aPoint - origin;
      
      //
      // z coordinate
      //
      dtReal coordZ = dtLinearAlgebra::dotProduct(rotAxis, dd);

      //
      // r coordinate
      //      
      dtVector3 rr = dd - coordZ * rotAxis;
      dtReal coordR = dtLinearAlgebra::length(rr);
      
      //
      // reference and reference2 axis
      //
      dtVector3 ref2Axis 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(rotAxis, refAxis)
      );
            
      //
      // phi coordinate
      //
      dtReal coordPhi 
      = 
      atan2( 
        dtLinearAlgebra::dotProduct(ref2Axis, rr), 
        dtLinearAlgebra::dotProduct(refAxis, rr)
      );          

			retVec.push_back( dtPoint3(coordR, coordPhi, coordZ) );
		}
		
		return retVec;
	}
  
  dtVector3 const xYz_rPhiZ::rotationAxis( void ) const {
    return config().lookup<dtVector3>("_rotAxis");
  }
  
  dtVector3 const xYz_rPhiZ::referenceAxis( void ) const {
    return config().lookup<dtVector3>("_refAxis");
  }
  
  dtPoint3 const xYz_rPhiZ::origin( void ) const {
    return config().lookup<dtPoint3>("_origin");
  }
    
  bool xYz_rPhiZ::isNecessary( void ) const {
    return true;
  }

  void xYz_rPhiZ::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);
  }  
  
  void xYz_rPhiZ::init( 
    ::QDomElement const * tE, 
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG 
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
		
    jsonPrimitive config;
    
    config.append(
      "_origin",
      dtXmlParserBase::getDtPoint3(
        dtXmlParserBase::getAttributeStr("origin", *tE), bC
      )
    );
    config.append(
      "_rotAxis",
      dtLinearAlgebra::normalize(
        dtXmlParserBase::getDtVector3(
          dtXmlParserBase::getAttributeStr("rotationAxis", *tE), bC
        )
      )
    );
    config.append(
      "_refAxis",
      dtLinearAlgebra::normalize(
        dtXmlParserBase::getDtVector3(
          dtXmlParserBase::getAttributeStr("referenceAxis", *tE), bC
        )
      )
    );  
    
    jInit(config, bC, cV, aF, aG);
  }
}