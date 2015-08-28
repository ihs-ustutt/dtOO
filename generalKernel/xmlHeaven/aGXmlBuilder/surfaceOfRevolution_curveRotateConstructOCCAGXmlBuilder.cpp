#include "surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder.h"
#include "geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h"
#include <xmlHeaven/dtXmlParserBase.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder
    ::surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder() {
  }

  surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder
    ::~surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder() {
  }

  void surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasChild("Vector_3", toBuild), buildPart());
		
    ::QDomElement wElement;
    //
    // spline curve
    //
    wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    dt__pH(analyticGeometry) aG_t(
      dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
    );
    dt__ptrAss(splineCurve3d * s3, splineCurve3d::DownCast(aG_t.get()) );
    dtCurve const * cc = s3->ptrConstDtCurve();
    //
    // vector
    //
    wElement = dtXmlParserBase::getChild("Vector_3", toBuild);
    dtVector3 vv
    =
    dtXmlParserBase::createDtVector3(&wElement, bC, cV, aF, aG);
    //
    // point
    //
    wElement = dtXmlParserBase::getChild("Point_3", toBuild);
    dtPoint3 pp 
    = 
    dtXmlParserBase::createDtPoint3(&wElement, bC, cV, aF, aG);
    //
    // angle
    //
    bool hasAngle = dtXmlParserBase::hasChild("float", toBuild);
    float angle = 2.*M_PI;
    if ( hasAngle ) {
      angle
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "value", 
        dtXmlParserBase::getChild("float", toBuild), cV,  aF
      );
    }
    ptrHandling<dtSurface> dtS;

    if (!hasAngle) {
      dtS.reset(
        surfaceOfRevolution_curveRotateConstructOCC(*cc, pp, vv).result()
      );
    }
    else {
      dtS.reset(
        rectangularTrimmedSurface_curveRotateConstructOCC(*cc, pp, vv, angle).result()
      );
    }
    rotatingSpline * rS = new rotatingSpline(*dtS, pp, vv, angle);

    rS->correctOrigin();

    result->push_back(rS);
  }
}