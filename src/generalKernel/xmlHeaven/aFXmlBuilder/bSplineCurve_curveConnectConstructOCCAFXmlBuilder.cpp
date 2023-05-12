#include "bSplineCurve_curveConnectConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_curveConnectConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_curveConnectConstructOCCAFXmlBuilder, 
      new bSplineCurve_curveConnectConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineCurve_curveConnectConstructOCCAFXmlBuilder
    ::bSplineCurve_curveConnectConstructOCCAFXmlBuilder() {
  }

  bSplineCurve_curveConnectConstructOCCAFXmlBuilder
    ::~bSplineCurve_curveConnectConstructOCCAFXmlBuilder() {
  }

  void bSplineCurve_curveConnectConstructOCCAFXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,  
    lvH_analyticFunction const * const aF,  
    lvH_analyticFunction * result 
	) const {   
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuild), 
      buildPart()
    );

    vectorHandling< dtCurve const * > ccV;
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticFunction", toBuild), wElement
    ) {
      dt__ptrAss(
        vec3dCurveOneD * s3, 
        vec3dCurveOneD::DownCast(
          dtXmlParserBase::createAnalyticFunction(&wElement, bC, cV, aF)
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
    
    result->push_back( new vec3dCurveOneD(dtC.get()) );
    ccV.destroy();
  }
}