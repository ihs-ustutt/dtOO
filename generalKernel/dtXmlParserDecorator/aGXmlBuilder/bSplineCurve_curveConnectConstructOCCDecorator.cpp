#include "bSplineCurve_curveConnectConstructOCCDecorator.h"

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
  bSplineCurve_curveConnectConstructOCCDecorator
    ::bSplineCurve_curveConnectConstructOCCDecorator() {
  }

  bSplineCurve_curveConnectConstructOCCDecorator
    ::~bSplineCurve_curveConnectConstructOCCDecorator() {
  }

  void bSplineCurve_curveConnectConstructOCCDecorator::buildPart(
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

    QDomElement wElement = getChild("analyticGeometry", toBuild);
    vectorHandling< dtCurve const * > ccV;
    while ( !wElement.isNull() ) {
      dt__ptrAss(
        splineCurve3d const * s3, 
        splineCurve3d::ConstDownCast(
          createAnalyticGeometry(&wElement, bC, cV, aF, aG)
        )
      );					
      ccV.push_back(s3->ptrConstDtCurve());
      wElement = getNextSibling("analyticGeometry", wElement);
    }
    ptrHandling< dtCurve > dtC(
      bSplineCurve_curveConnectConstructOCC(ccV).result()
    );
    result->push_back( new splineCurve3d(dtC.get()) );
  }
}