#include "bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierSurface_bezierCurveFillConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder
    ::bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder() {
  }

  bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder
    ::~bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder() {
  }

  void bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder::buildPart(
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
		
    vectorHandling< analyticGeometry * > aG_t;
    ::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, aG, &aG_t);
      wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
    }

    vectorHandling< dtCurve const * > dtC( aG_t.size() );
    dt__forAllIndex(aG_t, ii) {
      dt__ptrAss(
        splineCurve3d const * s3, splineCurve3d::ConstDownCast(aG_t[ii])
      );
      dtC[ii] = s3->ptrConstDtCurve();
    }

    ptrHandling< dtSurface > dtS(
      bezierSurface_bezierCurveFillConstructOCC(dtC).result()
    );
    result->push_back( new analyticSurface(dtS.get()) );
  }
}