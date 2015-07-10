#include "analyticGeometryAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  analyticGeometryAGXmlBuilder::analyticGeometryAGXmlBuilder() {
  }

  analyticGeometryAGXmlBuilder::~analyticGeometryAGXmlBuilder() {
  }

  void analyticGeometryAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
    );

    //
    // copy
    //
    ::QDomElement wElement 
    = 
    dtXmlParserBase::getChild("analyticGeometry", toBuild);     
    vectorHandling< analyticGeometry * > toCopy;
    dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, aG, &toCopy);
    dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );
  }
}
