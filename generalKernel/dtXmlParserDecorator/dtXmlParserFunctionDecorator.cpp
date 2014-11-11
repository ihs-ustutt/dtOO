#include "dtXmlParserFunctionDecorator.h"
#include <logMe/logMe.h>

namespace dtOO {
  dtXmlParserFunctionDecorator::dtXmlParserFunctionDecorator() {
  }

  void dtXmlParserFunctionDecorator::buildPartCompound(
	  QDomElement const & toBuildP, 
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
    DTINFOWF(buildPartCompound(), << "Call on dtXmlParserFunctionDecorator");
  }
	

  dtXmlParserFunctionDecorator::~dtXmlParserFunctionDecorator() {
  }
}