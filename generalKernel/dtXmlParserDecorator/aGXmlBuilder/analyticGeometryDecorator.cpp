#include "analyticGeometryDecorator.h"

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
  analyticGeometryDecorator::analyticGeometryDecorator() {
  }

  analyticGeometryDecorator::~analyticGeometryDecorator() {
  }

  void analyticGeometryDecorator::buildPart( 
    QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    //
		// check input
		//    
    dt__throwIf(!hasChild("analyticGeometry", toBuild), buildPart());

    //
    // copy
    //
    QDomElement wElement = getChild("analyticGeometry", toBuild);     
    vectorHandling< analyticGeometry * > toCopy;
    createAdvanced(&wElement, bC, cV, aF, aG, &toCopy);
    dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );
  }
}
