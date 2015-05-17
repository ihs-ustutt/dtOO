#include "bezierCurve_pointConstructOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bezierCurve_pointConstructOCCDecorator
    ::bezierCurve_pointConstructOCCDecorator() {
  }

  bezierCurve_pointConstructOCCDecorator
    ::~bezierCurve_pointConstructOCCDecorator() {
  }

  void bezierCurve_pointConstructOCCDecorator::buildPart(
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
    dt__throwIf(!hasChild("Point_3", toBuild), buildPart());

    std::vector< dtPoint3 > workingPointP;
    QDomElement wElement = getChild("Point_3", toBuild);
    while ( !wElement.isNull() ) {
      this->createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
      wElement = getNextSibling("Point_3", wElement);
    }
    result->push_back( 
      new splineCurve3d(
        bezierCurve_pointConstructOCC(workingPointP).result()
      )
    );
  }
}