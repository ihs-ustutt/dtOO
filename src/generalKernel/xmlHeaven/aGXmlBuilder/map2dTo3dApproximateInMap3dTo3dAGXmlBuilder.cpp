#include "map2dTo3dApproximateInMap3dTo3dAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
	map2dTo3dApproximateInMap3dTo3dAGXmlBuilder
		::map2dTo3dApproximateInMap3dTo3dAGXmlBuilder() {

	}

	map2dTo3dApproximateInMap3dTo3dAGXmlBuilder
		::~map2dTo3dApproximateInMap3dTo3dAGXmlBuilder() {

	}

	void map2dTo3dApproximateInMap3dTo3dAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::getNChildren("analyticGeometry", toBuild)
      || 
      !dtXmlParserBase::hasAttribute("number_points_one", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("number_points_two", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("order", toBuild), 
      buildPart()
    );
		
		map2dTo3d const * m2d = NULL;
		map3dTo3d const * m3d = NULL;     		
		::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
		while ( !wElement.isNull() ) {
			dt__pH(analyticGeometry) aG_t(
			  dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
      );
			m2d = map2dTo3d::SecureCast(aG_t.get());
			m3d = map3dTo3d::SecureCast(aG_t.get());				
			wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);				
		}
    dt__throwIf(!m2d && !m3d, buildPart());
		
		int nU 
    = 
    dtXmlParserBase::getAttributeIntMuParse("number_points_one", toBuild, cV, aF);
		int nV 
    = 
    dtXmlParserBase::getAttributeIntMuParse("number_points_two", toBuild, cV, aF);
		int order 
    = 
    dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV, aF);

		result->push_back( 
    	map2dTo3d_approximateMap2dTo3dInMap3dTo3d(
		    m2d, m3d, nU, nV, order
		  ).result()				
    );
   		
	}
}