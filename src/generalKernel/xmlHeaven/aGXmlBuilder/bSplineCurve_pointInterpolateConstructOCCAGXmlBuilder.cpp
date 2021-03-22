#include "bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder, 
      new bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
  ) const {
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());

    std::vector< dtPoint3 > workingPointP;
    std::vector< ::QDomElement > wElementV
    = 
    dtXmlParserBase::getChildVector("Point_3", toBuild);
    
    dt__forAllRefAuto(wElementV, wElement) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
    }
    
    int minDeg = 3;
    if ( dtXmlParserBase::hasAttribute("orderMin", toBuild) ) {
      minDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMin", toBuild, cV, aF);
    }    
    int maxDeg = 8;
    if ( dtXmlParserBase::hasAttribute("orderMax", toBuild) ) {
      maxDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMax", toBuild, cV, aF);
    }
    
    result->push_back( 
      new analyticCurve(
        bSplineCurve_pointInterpolateConstructOCC(workingPointP, minDeg, maxDeg).result()
      )
    );
  }
}