#include "pointDefinitionDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/pointContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  pointDefinitionDecorator::pointDefinitionDecorator() {
  }

  pointDefinitionDecorator::~pointDefinitionDecorator() {
  }

  void pointDefinitionDecorator::buildPart(QDomElement ** toBuildP,
                                             pointContainer * const pointContainerP,
                                             vectorContainer * const vectorContainerP,    
                                             vectorHandling< constValue * > const * const cValP,        
                                             vectorHandling< analyticFunction * > const * const sFunP,
                                             vectorHandling< analyticGeometry * > const * const depAGeoP,
                                             vectorHandling< analyticGeometry * > * aGeoP ) const {
   
    QDomElement wElement = getChild("Point_3", **toBuildP);
    int nPoints = 0;
    while ( !wElement.isNull() ) {
			vectorHandling< dtPoint3 > workingPointP;
      this->createBasic( &wElement, 
                         pointContainerP, 
                         vectorContainerP, 
                         cValP, 
                         sFunP, 
                         depAGeoP, 
                         &workingPointP );
      nPoints = nPoints + workingPointP.size();
      wElement = getNextSibling("Point_3", wElement);       
    }

    DTINFOWF(buildPart(), << nPoints << " points created.");
  }
}

