#include "analyticFunctionFunctionDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  analyticFunctionFunctionDecorator::analyticFunctionFunctionDecorator() {
  }

  analyticFunctionFunctionDecorator::~analyticFunctionFunctionDecorator() {
  }

  void analyticFunctionFunctionDecorator::buildPart( 
         QDomElement const & toBuildP, 
         vectorHandling< constValue * > const * const cValP,
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP) const {

		std::vector< QDomElement > elV = getChildVector(toBuildP);
		for (int ii=0; ii<elV.size(); ii++) {//dt__FORALL(elV, ii,
		  if ( hasAttribute("label", elV[ii]) ) {
				if ( depSFunP->has( getAttributeStr("label", elV[ii]) ) ) {
					sFunP->push_back( 
					  this->createAnalyticFunction( &elV[ii], cValP, depSFunP)
					);
					return;
				}
			}
		}

  }
}
