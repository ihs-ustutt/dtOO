#include "bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include "geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder
    ::bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("Point_2", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasAttribute("part_label", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());
		
    std::vector< dtPoint2 > p2;
    QDomElement wElement = dtXmlParserBase::getChild("Point_2", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &p2 );
      wElement = dtXmlParserBase::getNextSibling("Point_2", wElement);
    }

    //
    // get mapping
    //
    dt__ptrAss(
      map2dTo3d const * const map,
      map2dTo3d::ConstDownCast(
        aG->get( dtXmlParserBase::getAttributeStr("part_label", toBuild) )
      )
    );

    std::vector< dtPoint3 > p3(p2.size());
    dt__forAllIndex(p2, ii) p3[ii] = map->getPoint( p2[ii] );

    int order = dtXmlParserBase::getAttributeInt("order", toBuild);
    result->push_back( 
      new splineCurve3d(
        bSplineCurve_pointConstructOCC(p3, order).result()
      )
    );
  }
}