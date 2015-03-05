#include "spline3dDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#define FAILURE 1
#define SUCCESS 0

namespace dtOO {
  spline3dDecorator::spline3dDecorator() {
  }

  spline3dDecorator::~spline3dDecorator() {
  }

  void spline3dDecorator::buildPart(QDomElement ** toBuildP,
                                           pointContainer * const pointContainerP,
                                           vectorContainer * const vectorContainerP,    
                                           vectorHandling< constValue * > const * const cValP,        
                                           vectorHandling< analyticFunction * > const * const sFunP,
                                           vectorHandling< analyticGeometry * > const * const depAGeoP,   
                                           vectorHandling< analyticGeometry * > * aGeoP ) const {
    /* ---------------------------------------------------------------------- */    
    /* check input */
    /* ---------------------------------------------------------------------- */    
    bool hasOrder = hasAttribute("order", **toBuildP);
    bool hasPoints = hasChild("Point_3", **toBuildP);
    bool hasAGeo = hasChild("splineCurve3d", **toBuildP);

    if (hasOrder && hasPoints) {
      int order = getAttributeInt("order", **toBuildP);

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
			aGeoP->push_back( new splineCurve3d(&workingPointP, order) );
    }
    else if (hasAGeo) {
      QDomElement wElement = getChild("splineCurve3d", **toBuildP);
      vectorHandling< analyticGeometry * > advanced;
			while ( !wElement.isNull() ) {
        createAdvanced( &wElement, 
                        pointContainerP, 
                        vectorContainerP, 
                        cValP, 
                        sFunP, 
                        depAGeoP, 
                        &advanced );
        wElement = getNextSibling("splineCurve3d", wElement);
      }
			aGeoP->push_back( new splineCurve3d(advanced) );
			advanced.destroy();
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAGeo) << LOGDEL
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasPoints) );
    }
  }
}