#include "analyticGeometryAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
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
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
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
    aGPtrVec toCopy;
    dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, aG, &toCopy);
    dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );
  }
}
