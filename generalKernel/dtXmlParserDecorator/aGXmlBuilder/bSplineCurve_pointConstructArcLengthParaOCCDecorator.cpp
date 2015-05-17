#include "bSplineCurve_pointConstructArcLengthParaOCCDecorator.h"

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
  bSplineCurve_pointConstructArcLengthParaOCCDecorator
    ::bSplineCurve_pointConstructArcLengthParaOCCDecorator() {
  }

  bSplineCurve_pointConstructArcLengthParaOCCDecorator
    ::~bSplineCurve_pointConstructArcLengthParaOCCDecorator() {
  }

  void bSplineCurve_pointConstructArcLengthParaOCCDecorator::buildPart(
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
    dt__throwIf(!hasChild("analyticGeometry", toBuild), buildPart());
		dt__throwIf(!hasAttribute("tolerance", toBuild), buildPart());
		dt__throwIf(!hasAttribute("max_order", toBuild), buildPart());
		dt__throwIf(!hasAttribute("num_segments", toBuild), buildPart());

    int const maxOrder = getAttributeIntMuParse("max_order", toBuild, cV, aF);
    int const nSegments 
    = 
    getAttributeIntMuParse("num_segments", toBuild, cV, aF);
    float const tol 
    = 
    getAttributeFloatMuParse("tolerance", toBuild, cV, aF);

    QDomElement wElement = getChild("analyticGeometry", toBuild);
    analyticGeometry * aG_t 
    =
    createAnalyticGeometry(&wElement, bC, cV, aF, aG);
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