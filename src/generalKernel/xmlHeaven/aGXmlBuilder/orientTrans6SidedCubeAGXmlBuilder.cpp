#include "orientTrans6SidedCubeAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <geometryEngine/dtSurface.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  orientTrans6SidedCubeAGXmlBuilder::orientTrans6SidedCubeAGXmlBuilder() {
  }

  orientTrans6SidedCubeAGXmlBuilder::~orientTrans6SidedCubeAGXmlBuilder() {
  }

  void orientTrans6SidedCubeAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    cVPtrVec const * const cV,  
    aFPtrVec const * const aF,  
    aGPtrVec const * const aG,
    aGPtrVec * result
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild),
      buildPart()
    );
       
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector( "analyticGeometry", toBuild ), anE
    ) {
      //
      // create and cast analyticGeometry
      //
      trans6SidedCube * t6
      =
      trans6SidedCube::MustDownCast(
        dtXmlParserBase::createAnalyticGeometry( anE, bC, cV, aF, aG )
      );
      
      //
      // modify
      //
      dt__forAllRefAuto(
        dtXmlParserBase::getAttributeStrVector("orient", anE), anOrient 
      ) {
        if (anOrient == "exchangeVW") t6->exchangeVW();
        else if (anOrient == "exchangeUV") t6->exchangeUV();
        else if (anOrient == "exchangeUW") t6->exchangeUW();
        else if (anOrient == "reverseU") t6->reverseU();
        else if (anOrient == "reverseV") t6->reverseV();
        else if (anOrient == "reverseW") t6->reverseW();
        else dt__throwUnexpected(buildPart()); 
      }
      
      //
      // add to result
      //
      result->push_back( t6 );
    }    
  }
}
