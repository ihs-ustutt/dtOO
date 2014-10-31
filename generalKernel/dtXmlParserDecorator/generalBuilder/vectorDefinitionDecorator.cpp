#include "vectorDefinitionDecorator.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainer/vectorContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vectorDefinitionDecorator::vectorDefinitionDecorator() {
  }

  vectorDefinitionDecorator::~vectorDefinitionDecorator() {
  }

  void vectorDefinitionDecorator::buildPart(QDomElement ** toBuildP,
                                             pointContainer * const pointContainerP,
                                             vectorContainer * const vectorContainerP,    
                                             vectorHandling< constValue * > const * const cValP,        
                                             vectorHandling< analyticFunction * > const * const sFunP,
                                             vectorHandling< analyticGeometry * > const * const depAGeoP,    
                                             vectorHandling< analyticGeometry * > * aGeoP ) const {
    QDomElement wElement = getChild("Vector_3", **toBuildP);
    while ( !wElement.isNull() ) {
      dtVector3 workingVectorP 
			= 
			createDtVector3( 
        &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
			);
      wElement = getNextSibling("Vector_3", wElement);
    }
  }
}

