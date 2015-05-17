#include "bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructArcLengthParaOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder
    ::bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder() {
  }

  bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder
    ::~bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder::buildPart(
    QDomElement const & toBuild,
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
		dt__throwIf(!dtXmlParserBase::hasAttribute("tolerance", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("max_order", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("num_segments", toBuild), buildPart());

    int const maxOrder = dtXmlParserBase::getAttributeIntMuParse("max_order", toBuild, cV, aF);
    int const nSegments 
    = 
    dtXmlParserBase::getAttributeIntMuParse("num_segments", toBuild, cV, aF);
    float const tol 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("tolerance", toBuild, cV, aF);

    QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    analyticGeometry * aG_t 
    =
    dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG);
    dt__ptrAss(splineCurve3d const * sC3, splineCurve3d::ConstDownCast(aG_t));
    result->push_back( 
      new splineCurve3d(
        bSplineCurve_pointConstructArcLengthParaOCC(
          sC3->ptrConstDtCurve(), tol, maxOrder, nSegments
        ).result()
      )
    );
  }
}