#include "analyticFunctionAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool analyticFunctionAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(analyticFunctionAFXmlBuilder, new analyticFunctionAFXmlBuilder())
  );
    
  analyticFunctionAFXmlBuilder::analyticFunctionAFXmlBuilder() {
  }

  analyticFunctionAFXmlBuilder::~analyticFunctionAFXmlBuilder() {
  }

  void analyticFunctionAFXmlBuilder::buildPart( 
		::QDomElement const & toBuild, 
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticFunction * result
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
    ::QDomElement wElement 
    = 
    dtXmlParserBase::getChild("analyticFunction", toBuild);     
    lvH_analyticFunction toCopy;
    dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, &toCopy);
    dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );    
  }
}
