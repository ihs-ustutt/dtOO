#include "vec3dInMap3dTo3dSurroundingInternalDecorator.h"

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
	vec3dInMap3dTo3dSurroundingInternalDecorator
		::vec3dInMap3dTo3dSurroundingInternalDecorator() {

	}

	vec3dInMap3dTo3dSurroundingInternalDecorator
		::~vec3dInMap3dTo3dSurroundingInternalDecorator() {

	}

	void vec3dInMap3dTo3dSurroundingInternalDecorator::buildPart(
		QDomElement ** toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF,  
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< analyticGeometry * > * rAG 
	) const {
    //
		// check input
		//
    dt__throwIf(!getNChildren("analyticGeometry", **toBuildP), buildPart());
		dt__throwIf(!hasAttribute("number_points_one", **toBuildP), buildPart());
		dt__throwIf(!hasAttribute("number_points_two", **toBuildP), buildPart());
		dt__throwIf(!hasAttribute("order", **toBuildP), buildPart());
		
		dt__pH(map2dTo3d const) m2d;
		dt__pH(map3dTo3d const) m3d;     		
		QDomElement wElement = getChild("analyticGeometry", **toBuildP);
		while ( !wElement.isNull() ) {
			analyticGeometry * aG_t 
			= 
			createAnalyticGeometry(&wElement, bC, cV, aF, aG);
			if ( map2dTo3d::DownCast(aG_t) ) m2d.reset(map2dTo3d::SecureCast(aG_t));
			if ( map3dTo3d::DownCast(aG_t) ) m3d.reset(map3dTo3d::SecureCast(aG_t));				
			wElement = getNextSibling("analyticGeometry", wElement);				
		}
    dt__throwIf(m2d.isNull() && m3d.isNull(), buildPart());
		
		int nU = getAttributeIntMuParse("number_points_one", **toBuildP, cV, aF);
		int nV = getAttributeIntMuParse("number_points_two", **toBuildP, cV, aF);
		int order = getAttributeIntMuParse("order", **toBuildP, cV, aF);

    dt__pH(map2dTo3d) toRet(
			map2dTo3d_approximateMap2dTo3dInMap3dTo3d(
		    m2d.get(), m3d.get(), nU, nV, order
		  ).result()				
		);
    toRet.reset( map2dTo3d_fullExtentInMap3dTo3d(toRet.get(), 0, 1).result() );
		
		rAG->push_back( toRet.get()->clone() );
   		
	}
}