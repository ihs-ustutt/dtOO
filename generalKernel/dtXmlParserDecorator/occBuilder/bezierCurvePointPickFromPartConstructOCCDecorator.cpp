#include "bezierCurvePointPickFromPartConstructOCCDecorator.h"
#include "analyticGeometryHeaven/map2dTo3d.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bezierCurvePointPickFromPartConstructOCCDecorator::bezierCurvePointPickFromPartConstructOCCDecorator() {
  }

  bezierCurvePointPickFromPartConstructOCCDecorator::~bezierCurvePointPickFromPartConstructOCCDecorator() {
  }

  void bezierCurvePointPickFromPartConstructOCCDecorator::buildPart(
	  QDomElement ** toBuildP,
		pointContainer * const pointContainerP,
		vectorContainer * const vectorContainerP,    
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
		
    if (hasPoints && hasPartLabel) {
      std::vector< dtPoint2 * > p2;
      QDomElement wElement = getChild("Point_2", **toBuildP);
      while ( !wElement.isNull() ) {
        this->createBasic( &wElement, 
                           pointContainerP, 
                           vectorContainerP, 
                           cValP, 
                           sFunP, 
                           depAGeoP, 
                           &p2 );
  			wElement = getNextSibling("Point_2", wElement);
      }
			
      //
      // get mapping
      //
			dt__PTRASS(
				map2dTo3d * map,
				map2dTo3d::DownCast(
					depAGeoP->get( getAttributeStr("part_label", **toBuildP) )
				)
			);
			
			std::vector< dtPoint3 > p3(p2.size());
			dt__FORALL(p2, ii,
			  p3[ii] = map->getPoint( *(p2[ii]) );
			  delete p2[ii];
			);
			
			aGeoP->push_back( 
			  new splineCurve3d(
			    bezierCurve_pointConstructOCC(p3).result()
			  )
			);
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasPoints) );
    }
  }
}