#include "bezierCurvePointPickFromPartConstructOCCAGXmlBuilder.h"
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
  bezierCurvePointPickFromPartConstructOCCAGXmlBuilder
    ::bezierCurvePointPickFromPartConstructOCCAGXmlBuilder() {
  }

  bezierCurvePointPickFromPartConstructOCCAGXmlBuilder
    ::~bezierCurvePointPickFromPartConstructOCCAGXmlBuilder() {
  }

  void bezierCurvePointPickFromPartConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {
    //
		// check input
		//    
    dt__throwIf(!dtXmlParserBase::hasChild("Point_2", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasAttribute("part_label", toBuild), buildPart());
		
    std::vector< dtPoint2 > p2;
    ::QDomElement wElement = dtXmlParserBase::getChild("Point_2", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &p2 );
      wElement = dtXmlParserBase::getNextSibling("Point_2", wElement);
    }

    //
    // get mapping
    //
    dt__ptrAss(
      map2dTo3d * map,
      map2dTo3d::DownCast(
        aG->get( dtXmlParserBase::getAttributeStr("part_label", toBuild) )
      )
    );

    std::vector< dtPoint3 > p3(p2.size());
    dt__forAllIndex(p2, ii) p3[ii] = map->getPoint( p2[ii] );

    result->push_back( 
      new splineCurve3d( bezierCurve_pointConstructOCC(p3).result() )
    );
  }
}