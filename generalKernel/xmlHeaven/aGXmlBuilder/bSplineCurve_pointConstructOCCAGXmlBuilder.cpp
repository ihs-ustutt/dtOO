#include "bSplineCurve_pointConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_pointConstructOCCAGXmlBuilder
    ::bSplineCurve_pointConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {
    //
		// check input
		//  
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());

    int const order = dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV, aF);
    std::vector< dtPoint3 > workingPointP;
    ::QDomElement wElement = dtXmlParserBase::getChild("Point_3", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
      wElement = dtXmlParserBase::getNextSibling("Point_3", wElement);
    }
    result->push_back( 
      new splineCurve3d(
        bSplineCurve_pointConstructOCC(workingPointP, order).result()
      )
    );
  }
}