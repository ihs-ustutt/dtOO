#include "bSplineCurve_pointPickFromPartConstructOCCDecorator.h"
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
  bSplineCurve_pointPickFromPartConstructOCCDecorator::bSplineCurve_pointPickFromPartConstructOCCDecorator() {
  }

  bSplineCurve_pointPickFromPartConstructOCCDecorator::~bSplineCurve_pointPickFromPartConstructOCCDecorator() {
  }

  void bSplineCurve_pointPickFromPartConstructOCCDecorator::buildPart(
	  QDomElement ** toBuildP,
    baseContainer * const bC, 
		vectorHandling< constValue * > const * const cValP,        
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,   
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {

    /* ---------------------------------------------------------------------- */    
    /* check input */
    /* ---------------------------------------------------------------------- */    
    bool hasPoints = hasChild("Point_2", **toBuildP);
    bool hasPartLabel = hasAttribute("part_label", **toBuildP);
		bool hasOrder = hasAttribute("order", **toBuildP);
		
    if (hasPoints && hasPartLabel && hasOrder) {
      std::vector< dtPoint2 > p2;
      QDomElement wElement = getChild("Point_2", **toBuildP);
      while ( !wElement.isNull() ) {
        dtXmlParserBase::createBasic( &wElement, bC, cValP, sFunP, depAGeoP, &p2 );
  			wElement = getNextSibling("Point_2", wElement);
      }
			
      //
      // get mapping
      //
			dt__ptrAss(
				map2dTo3d * map,
				map2dTo3d::DownCast(
					depAGeoP->get( getAttributeStr("part_label", **toBuildP) )
				)
			);
			
			std::vector< dtPoint3 > p3(p2.size());
			dt__forAllIndex(p2, ii) {
			  p3[ii] = map->getPoint( p2[ii] );
//			  delete p2[ii];
			}
			
		  int order = getAttributeInt("order", **toBuildP);
			aGeoP->push_back( 
			  new splineCurve3d(
			    bSplineCurve_pointConstructOCC(p3, order).result()
			  )
			);
    }
    else {
      dt__throw(buildPart(), << dt__eval(hasPoints) );
    }
  }
}