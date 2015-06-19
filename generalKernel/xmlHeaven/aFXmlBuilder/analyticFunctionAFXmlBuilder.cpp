#include "analyticFunctionAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
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
		QDomElement const & toBuild, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticFunction * > * result
	) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart()
    );

    //
    // copy
    //
    QDomElement wElement 
    = 
    dtXmlParserBase::getChild("analyticFunction", toBuild);     
    vectorHandling< analyticFunction * > toCopy;
    dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, &toCopy);
    dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );    
  }
}
