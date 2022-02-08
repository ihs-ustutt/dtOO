#include "bSplineCurve_curveConnectConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
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
  bool bSplineCurve_curveConnectConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_curveConnectConstructOCCAGXmlBuilder, 
      new bSplineCurve_curveConnectConstructOCCAGXmlBuilder()
    )
  );
  
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
    dtReal tolerance = 1.E-07;
    if ( dtXmlParserBase::hasAttribute("tolerance", toBuild) ) {
      tolerance 
      = 
      dtXmlParserBase::getAttributeFloatMuParse("tolerance", toBuild, cV);
    }
    dtInt param = 0;    
    if ( dtXmlParserBase::hasAttribute("parameterization", toBuild) ) {
      param 
      = 
      dtXmlParserBase::getAttributeIntMuParse("parameterization", toBuild, cV);
    }
    dtInt minM = 0;    
    if ( dtXmlParserBase::hasAttribute("minM", toBuild) ) {
      minM 
      = 
      dtXmlParserBase::getAttributeIntMuParse("minM", toBuild, cV);
    }    
    dt__info(
      buildPart(), 
      << "Connect with tol = " << tolerance << ", param = " << param
      << " and minM = " << minM
    );
    dtC.reset(
      bSplineCurve_curveConnectConstructOCC(
        ccV, tolerance, param, minM
      ).result()
    );      
    
    result->push_back( new analyticCurve(dtC.get()) );
    ccV.destroy();
  }
}