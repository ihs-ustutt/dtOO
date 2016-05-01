#include "bSplineSurface_skinConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::~bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  void bSplineSurface_skinConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );

    vectorHandling< dtCurve const * > ccV;    
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anEl
    ) {
      analyticGeometry * aG_t
      =
      dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG);
      dt__ptrAss(
        splineCurve3d const * s3, splineCurve3d::ConstDownCast(aG_t)
      );
      ccV.push_back( s3->ptrConstDtCurve()->clone() );
    }

    int nIter = 0;
    if ( dtXmlParserBase::hasAttribute("nIterations", toBuild) ) {
      nIter 
      = 
      dtXmlParserBase::getAttributeIntMuParse("nIterations", toBuild, cV, aF);
    }
    int minDeg = 1;
    if ( dtXmlParserBase::hasAttribute("orderMin", toBuild) ) {
      minDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMin", toBuild, cV, aF);
    }    
    int maxDeg = 25;
    if ( dtXmlParserBase::hasAttribute("orderMax", toBuild) ) {
      maxDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMax", toBuild, cV, aF);
    }          
    result->push_back( 
      new analyticSurface(
        dt__tmpPtr(
          dtSurface, 
          bSplineSurface_skinConstructOCC(ccV, minDeg, maxDeg, nIter).result()
        )
      )
    );
    ccV.destroy();
  }
}