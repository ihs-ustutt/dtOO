#include "dtXmlParserFunctionDecorator.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  dtXmlParserFunctionDecorator::dtXmlParserFunctionDecorator() {
  }

  void dtXmlParserFunctionDecorator::buildPartCompound(
	  QDomElement const & toBuildP, 
    baseContainer * const bC,					
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
    dt__info(buildPartCompound(), << "Call on dtXmlParserFunctionDecorator");
  }
	

  dtXmlParserFunctionDecorator::~dtXmlParserFunctionDecorator() {
  }
}