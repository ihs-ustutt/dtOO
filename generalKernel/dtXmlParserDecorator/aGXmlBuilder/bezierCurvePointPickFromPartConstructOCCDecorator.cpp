#include "bezierCurvePointPickFromPartConstructOCCDecorator.h"
#include "analyticGeometryHeaven/map2dTo3d.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bezierCurvePointPickFromPartConstructOCCDecorator
    ::bezierCurvePointPickFromPartConstructOCCDecorator() {
  }

  bezierCurvePointPickFromPartConstructOCCDecorator
    ::~bezierCurvePointPickFromPartConstructOCCDecorator() {
  }

  void bezierCurvePointPickFromPartConstructOCCDecorator::buildPart(
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
    dt__throwIf(!hasChild("Point_2", toBuild), buildPart());
    dt__throwIf(!hasAttribute("part_label", toBuild), buildPart());
		
    std::vector< dtPoint2 > p2;
    QDomElement wElement = getChild("Point_2", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &p2 );
      wElement = getNextSibling("Point_2", wElement);
    }

    //
    // get mapping
    //
    dt__ptrAss(
      map2dTo3d * map,
      map2dTo3d::DownCast(
        aG->get( getAttributeStr("part_label", toBuild) )
      )
    );

    std::vector< dtPoint3 > p3(p2.size());
    dt__forAllIndex(p2, ii) p3[ii] = map->getPoint( p2[ii] );

    result->push_back( 
      new splineCurve3d( bezierCurve_pointConstructOCC(p3).result() )
    );
  }
}