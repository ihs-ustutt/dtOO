#include "bSplineCurve_curveConnectConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_curveConnectConstructOCCAGXmlBuilder
    ::bSplineCurve_curveConnectConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_curveConnectConstructOCCAGXmlBuilder
    ::~bSplineCurve_curveConnectConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_curveConnectConstructOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());

    ::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    vectorHandling< dtCurve const * > ccV;
    while ( !wElement.isNull() ) {
      dt__ptrAss(
        splineCurve3d * s3, 
        splineCurve3d::DownCast(
          dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
        )
      );
      ccV.push_back(s3->ptrConstDtCurve()->clone());
      delete s3;
      wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
    }
    ptrHandling< dtCurve > dtC(
      bSplineCurve_curveConnectConstructOCC(ccV).result()
    );
    result->push_back( new splineCurve3d(dtC.get()) );
    ccV.destroy();
  }
}