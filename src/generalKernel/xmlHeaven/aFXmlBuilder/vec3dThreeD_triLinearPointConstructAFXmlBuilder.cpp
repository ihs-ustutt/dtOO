#include "vec3dThreeD_triLinearPointConstructAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  vec3dThreeD_triLinearPointConstructAFXmlBuilder
    ::vec3dThreeD_triLinearPointConstructAFXmlBuilder() {
  }

  vec3dThreeD_triLinearPointConstructAFXmlBuilder
    ::~vec3dThreeD_triLinearPointConstructAFXmlBuilder() {

  }

  std::vector< std::string > vec3dThreeD_triLinearPointConstructAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of
      ("vec3dTriLinearThreeD")
      ("vec3dTriLinearThreeDAFXmlBuilder");
  }
  
  void vec3dThreeD_triLinearPointConstructAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV, 
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticFunction * > * result
	) const {
    dt__throwIf( !dtXmlParserBase::hasChild("Point_3", toBuildP), buildPart() );
    
    std::vector< dtPoint3 > pA;
    std::vector< ::QDomElement > wEl 
    = 
    dtXmlParserBase::getChildVector("Point_3", toBuildP);

    dt__forAllRefAuto(wEl, anEl) {
      std::vector< dtPoint3 > wPoint;
      dtXmlParserBase::createBasic( &anEl, bC, cV, aF, &wPoint );
      dt__forAllRefAuto(wPoint, aPoint) pA.push_back( aPoint );
    }

    //
    // create trilinear interpolation
    //
    if (pA.size() == 8) {
      result->push_back( 
        new vec3dTriLinearThreeD( 
          pA[0], pA[1], pA[2], pA[3], pA[4], pA[5], pA[6], pA[7]  
        ) 
      );			
    }
    else if (pA.size() == 2) {
      result->push_back(new vec3dTriLinearThreeD( pA[0], pA[1] ));			      
    }
    else dt__throwUnexpected(buildPart());
  }
}
