#include "dtXmlParserDecorator.h"

#include <logMe/logMe.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  dtXmlParserDecorator::dtXmlParserDecorator() {
  }

  dtXmlParserDecorator::~dtXmlParserDecorator() {
  }

	void dtXmlParserDecorator::buildPartCompound(
		QDomElement ** toBuildP,
    baseContainer * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {
    dt__info(buildPartCompound(), << "Call on dtXmlParserDecorator");		
	}
}