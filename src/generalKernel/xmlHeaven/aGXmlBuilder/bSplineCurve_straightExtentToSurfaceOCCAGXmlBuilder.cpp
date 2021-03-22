#include "bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_straightIntersectToMap2dTo3d.h>

namespace dtOO {
  bool bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder, 
      new bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder
	  ::bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder() {
  }

  bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder
	  ::~bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder() {
  }

  void bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("attribute", toBuild), buildPart());
     		
    map1dTo3d const * m1d = NULL;
		map2dTo3d const * m2d = NULL;
		::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
		while ( !wElement.isNull() ) {
			dt__pH(analyticGeometry) aG_t(
		    dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
       );

		  m1d = map1dTo3d::SecureCast(aG_t.get());
			m2d = map2dTo3d::SecureCast(aG_t.get());				
      
			wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
		}

		dt__throwIf(!m1d && !m2d, buildPart());

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
			dtPoint3_straightIntersectToMap2dTo3d(pV[0], v0, m2d).result();			
		}
		else if (dtXmlParserBase::getAttributeStr("attribute", toBuild) == "forward") {
		  v0 = dtLinearAlgebra::normalize(m1d->firstDerU((*m1d) % 0.));
			pV[1] = m1d->getPoint( (*m1d) % 0. );			
			//
			// get intersection point
			// 			
      pV[0]
			= 
			dtPoint3_straightIntersectToMap2dTo3d(pV[1], -v0, m2d).result();
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
		  new analyticCurve( 
		    dt__tmpPtr(dtCurve, bSplineCurve_pointConstructOCC(pV, 1).result())  
		  ) 
		);
  }
}