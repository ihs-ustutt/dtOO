#include "bSplineCurve_curveConnectConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
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
    cVPtrVec const * const cV,  
    aFPtrVec const * const aF,  
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {   
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );

    vectorHandling< dtCurve const * > ccV;
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), wElement
    ) {
      dt__ptrAss(
        analyticCurve * s3, 
        analyticCurve::DownCast(
          dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
        )
      );
      ccV.push_back(s3->ptrConstDtCurve()->clone());
      delete s3;
    }
    
    ptrHandling< dtCurve > dtC;
    if ( !dtXmlParserBase::hasAttribute("tolerance", toBuild) ) {
      dtC.reset(
        bSplineCurve_curveConnectConstructOCC(ccV).result()
      );
    }
    else {
      dtC.reset(
        bSplineCurve_curveConnectConstructOCC(
          ccV, 
          dtXmlParserBase::getAttributeFloatMuParse("tolerance", toBuild, cV)
        ).result()
      );      
    }
    
    result->push_back( new analyticCurve(dtC.get()) );
    ccV.destroy();
  }
}