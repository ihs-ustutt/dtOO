#include "bezierCurve_pointConstructOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
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
  bezierCurve_pointConstructOCCDecorator::bezierCurve_pointConstructOCCDecorator() {
  }

  bezierCurve_pointConstructOCCDecorator::~bezierCurve_pointConstructOCCDecorator() {
  }

  void bezierCurve_pointConstructOCCDecorator::buildPart(QDomElement ** toBuildP,
                                           pointContainer * const pointContainerP,
                                           vectorContainer * const vectorContainerP,    
                                           vectorHandling< constValue * > const * const cValP,        
                                           vectorHandling< analyticFunction * > const * const sFunP,
                                           vectorHandling< analyticGeometry * > const * const depAGeoP,   
                                           vectorHandling< analyticGeometry * > * aGeoP ) const {

    /* ---------------------------------------------------------------------- */    
    /* check input */
    /* ---------------------------------------------------------------------- */    
    bool hasPoints = hasChild("Point_3", **toBuildP);

    if (hasPoints) {
      std::vector< dtPoint3 > workingPointP;
      QDomElement wElement = getChild("Point_3", **toBuildP);
      while ( !wElement.isNull() ) {
        this->createBasic( &wElement, 
                           pointContainerP, 
                           vectorContainerP, 
                           cValP, 
                           sFunP, 
                           depAGeoP, 
                           &workingPointP );
  			wElement = getNextSibling("Point_3", wElement);
      }
			aGeoP->push_back( 
			  new splineCurve3d(
			    bezierCurve_pointConstructOCC(workingPointP).result()
			  )
			);
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasPoints) );
    }
  }
}