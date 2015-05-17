#include "analyticFunctionAFXmlBuilder.h"

#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  analyticFunctionAFXmlBuilder::analyticFunctionAFXmlBuilder() {
  }

  analyticFunctionAFXmlBuilder::~analyticFunctionAFXmlBuilder() {
  }

  void analyticFunctionAFXmlBuilder::buildPart( 
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
		std::vector< QDomElement > elV = dtXmlParserBase::getChildVector(toBuildP);
		for (int ii=0; ii<elV.size(); ii++) {//dt__FORALL(elV, ii,
		  if ( dtXmlParserBase::hasAttribute("label", elV[ii]) ) {
				sFunP->push_back( 
  			  dtXmlParserBase::createAnalyticFunction( &elV[ii], bC, cValP, depSFunP)
				);
			}
		}
  }
}
