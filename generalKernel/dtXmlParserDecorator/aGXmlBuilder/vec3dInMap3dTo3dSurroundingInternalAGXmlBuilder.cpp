#include "vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder.h"

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
	vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder
		::vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder() {

	}

	vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder
		::~vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder() {

	}

	void vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::getNChildren("analyticGeometry", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("number_points_one", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("number_points_two", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());
		
		dt__pH(map2dTo3d const) m2d;
		dt__pH(map3dTo3d const) m3d;     		
		QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
		while ( !wElement.isNull() ) {
			analyticGeometry * aG_t 
			= 
			dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG);
			if ( map2dTo3d::DownCast(aG_t) ) m2d.reset(map2dTo3d::SecureCast(aG_t));
			if ( map3dTo3d::DownCast(aG_t) ) m3d.reset(map3dTo3d::SecureCast(aG_t));				
			wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);				
		}
    dt__throwIf(m2d.isNull() && m3d.isNull(), buildPart());
		
		int nU = dtXmlParserBase::getAttributeIntMuParse("number_points_one", toBuild, cV, aF);
		int nV = dtXmlParserBase::getAttributeIntMuParse("number_points_two", toBuild, cV, aF);
		int order = dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV, aF);

    dt__pH(map2dTo3d) toRet(
			map2dTo3d_approximateMap2dTo3dInMap3dTo3d(
		    m2d.get(), m3d.get(), nU, nV, order
		  ).result()				
		);
    toRet.reset( map2dTo3d_fullExtentInMap3dTo3d(toRet.get(), 0, 1).result() );
		
		result->push_back( toRet.get()->clone() );
   		
	}
}