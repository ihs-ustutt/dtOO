#include "dtXmlParserDecoratorInterface.h"

#include <logMe/logMe.h>

namespace dtOO {
  dtXmlParserDecoratorInterface::dtXmlParserDecoratorInterface() {
  }

  dtXmlParserDecoratorInterface::~dtXmlParserDecoratorInterface() {
  }
	
	void dtXmlParserDecoratorInterface::buildPartCompound(
		QDomElement ** toBuildP,
		pointContainer * const pointContainerP,
		vectorContainer * const vectorContainerP,      
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {
		dt__throw(buildPartCompound(), << "Call on dtXmlParserDecoratorInterface");
	}
}