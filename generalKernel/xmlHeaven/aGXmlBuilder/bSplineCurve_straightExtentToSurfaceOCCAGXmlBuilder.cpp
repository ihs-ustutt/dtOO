#include "bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_straightIntersectToMap2dTo3d.h>

namespace dtOO {
  bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder
	  ::bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder() {
  }

  bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder
	  ::~bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder() {
  }

  void bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder::buildPart(
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
		dt__throwIf(!dtXmlParserBase::hasAttribute("attribute", toBuild), buildPart());

		dt__pH(map1dTo3d const) m1d;
		dt__pH(map2dTo3d const) m2d;     		
		::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
		while ( !wElement.isNull() ) {
			analyticGeometry * aG_t 
			= 
			dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG);
			if ( map1dTo3d::DownCast(aG_t) ) m1d.reset(map1dTo3d::SecureCast(aG_t));
			if ( map2dTo3d::DownCast(aG_t) ) m2d.reset(map2dTo3d::SecureCast(aG_t));				
			wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);				
		}

		dt__throwIf(m1d.isNull() && m2d.isNull(), buildPart());

		dtVector3 v0;
		std::vector< dtPoint3 > pV(2);
		if (dtXmlParserBase::getAttributeStr("attribute", toBuild) == "backward") {
		  v0 = dtLinearAlgebra::normalize(m1d->firstDerU((*m1d) % 1.));
		  pV[0] = m1d->getPoint( (*m1d) % 1. );
			//
			// get intersection point
			// 			
		  pV[1]
			= 
			dtPoint3_straightIntersectToMap2dTo3d(pV[0], v0, m2d.get()).result();			
		}
		else if (dtXmlParserBase::getAttributeStr("attribute", toBuild) == "forward") {
		  v0 = dtLinearAlgebra::normalize(m1d->firstDerU((*m1d) % 0.));
			pV[1] = m1d->getPoint( (*m1d) % 0. );			
			//
			// get intersection point
			// 			
      pV[0]
			= 
			dtPoint3_straightIntersectToMap2dTo3d(pV[1], -v0, m2d.get()).result();
		}
		else {
			dt__throw(
				buildPart(), 
				<< "Attribute should be forward or backward"
			);
		}

		//
		// create and return bSplineCurve
		//
		result->push_back(
		  new splineCurve3d( 
		    dt__tmpPtr(dtCurve, bSplineCurve_pointConstructOCC(pV, 1).result())  
		  ) 
		);
  }
}