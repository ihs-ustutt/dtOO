#include "splineSurfaceDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  splineSurfaceDecorator::splineSurfaceDecorator() {
  }

  splineSurfaceDecorator::~splineSurfaceDecorator() {
  }

  void splineSurfaceDecorator::buildPart(
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
    bool hasSplineCurves3d = hasChild("splineCurve3d", **toBuildP);
    bool hasSplineSurface = hasChild("splineSurface", **toBuildP);
    bool hasOrder = hasAttribute("order", **toBuildP);

    if (hasOrder && hasSplineCurves3d) {
      int order = getAttributeInt("order", **toBuildP);      
      vectorHandling< analyticGeometry const * > cc;
      /* -------------------------------------------------------------------- */
      /* put splines in builder */
      /* -------------------------------------------------------------------- */
      QDomElement wElement = getChild( "splineCurve3d", **toBuildP );
      while ( !wElement.isNull() ) {
        analyticGeometry * aGeoSplineP
				=
        createAnalyticGeometry(
          &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
				);
				cc.push_back(aGeoSplineP);
        //goto next sibling
        wElement = getNextSibling("splineCurve3d", wElement);
      }

			splineSurface3d * sS = new splineSurface3d();
			sS->setLoftDirectionOrder(order);
			sS->createMe(&cc);
			aGeoP->push_back(sS);
			cc.destroy();
    }
    else if (hasSplineSurface) {
      QDomElement wElement = getChild( "splineSurface", **toBuildP );
      createAdvanced(
        &wElement,
        pointContainerP,
        vectorContainerP,
        cValP,
        sFunP,
        depAGeoP,
        aGeoP
      );
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasSplineCurves3d) << LOGDEL
              << DTLOGEVAL(hasSplineSurface) );
    }
  }
}