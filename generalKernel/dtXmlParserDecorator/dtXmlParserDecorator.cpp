#include "dtXmlParserDecorator.h"

#include <logMe/logMe.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  dtXmlParserDecorator::dtXmlParserDecorator() {
  }

  dtXmlParserDecorator::~dtXmlParserDecorator() {
  }

	void dtXmlParserDecorator::buildPartCompound(
		QDomElement ** toBuildP,
		pointContainer * const pointContainerP,
		vectorContainer * const vectorContainerP,      
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {
    DTINFOWF(buildPartCompound(), << "Call on dtXmlParserDecorator");		
	}
}