#include "vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
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
  bool vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder, 
      new vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder()
    )
  );
  
  vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder
    ::vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder() {
  }

  vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder
    ::~vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder() {

  }

  void vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF,
		lvH_analyticFunction * result
	) const {
    dt__throwIf( !dtXmlParserBase::hasChild("Point_D", toBuildP), buildPart() );
    
    std::vector< dtPointD > pA;
    std::vector< ::QDomElement > wEl 
    = 
    dtXmlParserBase::getChildVector("Point_D", toBuildP);

    dt__forAllRefAuto(wEl, anEl) {
      std::vector< dtPointD > wPoint;
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
    for( dtInt ii=0; ii<pA.size(); ii = ii + 2 ) {
      dt__throwIf( pA[ii  ].dimension()!=6, buildPart() );
      dt__throwIf( pA[ii+1].dimension()!=6, buildPart() );
      
      dt__info(
        buildPart(),
        << "Create function component with points:" << std::endl
        << logMe::dtFormat("Y[0] = (%f, %f, %f)") 
          % pA[ii  ][3] % pA[ii  ][4] % pA[ii  ][5] << std::endl
        << logMe::dtFormat("Y[1] = (%f, %f, %f)") 
          % pA[ii+1][3] % pA[ii+1][4] % pA[ii+1][5] << std::endl
        << logMe::dtFormat("X[0] = (%f, %f, %f)") 
          % pA[ii  ][0] % pA[ii  ][1] % pA[ii  ][2] << std::endl
        << logMe::dtFormat("X[1] = (%f, %f, %f)") 
          % pA[ii+1][0] % pA[ii+1][1] % pA[ii+1][2] << std::endl        
      );
      v3d->add(
        new vec3dTriLinearThreeD(
          dtPoint3( pA[ii  ][3], pA[ii  ][4], pA[ii  ][5] ),
          dtPoint3( pA[ii+1][3], pA[ii+1][4], pA[ii+1][5] ),      
          dtPoint3( pA[ii  ][0], pA[ii  ][1], pA[ii  ][2] ),
          dtPoint3( pA[ii+1][0], pA[ii+1][1], pA[ii+1][2] ) 
        )
      );
    }
    result->push_back(v3d);
  }
}
