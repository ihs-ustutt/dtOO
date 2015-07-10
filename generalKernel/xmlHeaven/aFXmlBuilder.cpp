#include "aFXmlBuilder.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  aFXmlBuilder::aFXmlBuilder() {
  }

  void aFXmlBuilder::buildPartCompound(
    ::QDomElement const & toBuild, 
    baseContainer * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticFunction * > * result
	) const {
    dt__info(buildPartCompound(), << "Call on aFXmlBuilder");
  }
	

  aFXmlBuilder::~aFXmlBuilder() {
  }
}