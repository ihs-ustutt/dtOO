#include "vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>
#include <analyticFunctionHeaven/vec3dMultiThreeD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder
    ::vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder() {
  }

  vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder
    ::~vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder() {

  }

  void vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder::buildPart(
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
    // create multi trilinear interpolation
    //
    vec3dMultiThreeD< vec3dTriLinearThreeD > * v3d 
    = 
    new vec3dMultiThreeD< vec3dTriLinearThreeD >();

    //
    // iterate points
    //
    dt__forAllIndex(pA, ii) {
      if (ii==0) continue;
      
      v3d->add( 
        new vec3dTriLinearThreeD( 
          pA[ii-1], pA[ii], dtPoint3(ii-1, ii-1, ii-1), dtPoint3(ii, ii, ii) 
        )
      );
    }
    result->push_back(v3d);
  }
}
