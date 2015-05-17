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
    QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result  
	) const {
    dt__info(buildPartCompound(), << "Call on dtXmlParserDecorator");		
	}
}