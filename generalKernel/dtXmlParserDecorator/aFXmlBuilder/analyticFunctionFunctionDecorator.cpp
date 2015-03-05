#include "analyticFunctionFunctionDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  analyticFunctionFunctionDecorator::analyticFunctionFunctionDecorator() {
  }

  analyticFunctionFunctionDecorator::~analyticFunctionFunctionDecorator() {
  }

  void analyticFunctionFunctionDecorator::buildPart( 
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
		std::vector< QDomElement > elV = getChildVector(toBuildP);
		for (int ii=0; ii<elV.size(); ii++) {//dt__FORALL(elV, ii,
		  if ( hasAttribute("label", elV[ii]) ) {
				sFunP->push_back( 
  			  this->createAnalyticFunction( &elV[ii], bC, cValP, depSFunP)
				);
			}
		}
  }
}
