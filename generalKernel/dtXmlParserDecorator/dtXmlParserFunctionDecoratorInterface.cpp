#include "dtXmlParserFunctionDecoratorInterface.h"

#include <logMe/logMe.h>

namespace dtOO {
  dtXmlParserFunctionDecoratorInterface::dtXmlParserFunctionDecoratorInterface() {
  }

  dtXmlParserFunctionDecoratorInterface::~dtXmlParserFunctionDecoratorInterface() {
  }

  void dtXmlParserFunctionDecoratorInterface::buildPartCompound(
	  QDomElement const & toBuildP, 
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
		dt__THROW(buildPartCompound(), "Call on interface dtXmlParserFunctionDecoratorInterface.");
	}
}
