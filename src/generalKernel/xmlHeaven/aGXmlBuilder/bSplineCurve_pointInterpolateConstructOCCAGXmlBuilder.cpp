#include "bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());

    std::vector< dtPoint3 > workingPointP;
    std::vector< ::QDomElement > wElementV
    = 
    dtXmlParserBase::getChildVector("Point_3", toBuild);
    
    dt__forAllRefAuto(wElementV, wElement) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
    }
    result->push_back( 
      new splineCurve3d(
        bSplineCurve_pointInterpolateConstructOCC(workingPointP).result()
      )
    );
  }
}