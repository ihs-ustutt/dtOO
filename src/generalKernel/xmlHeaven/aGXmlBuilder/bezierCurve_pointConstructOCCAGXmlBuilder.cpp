#include "bezierCurve_pointConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bezierCurve_pointConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bezierCurve_pointConstructOCCAGXmlBuilder, 
      new bezierCurve_pointConstructOCCAGXmlBuilder()
    )
  );
  
  bezierCurve_pointConstructOCCAGXmlBuilder
    ::bezierCurve_pointConstructOCCAGXmlBuilder() {
  }

  bezierCurve_pointConstructOCCAGXmlBuilder
    ::~bezierCurve_pointConstructOCCAGXmlBuilder() {
  }

  void bezierCurve_pointConstructOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());

    std::vector< dtPoint3 > workingPointP;
    ::QDomElement wElement = dtXmlParserBase::getChild("Point_3", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
      wElement = dtXmlParserBase::getNextSibling("Point_3", wElement);
    }
    result->push_back( 
      new analyticCurve(
        bezierCurve_pointConstructOCC(workingPointP).result()
      )
    );
  }
}