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
               QDomElement ** toBuildP,
               baseContainer * const bC,           
               vectorHandling< constValue * > const * const cValP,           
               vectorHandling< analyticFunction * > const * const sFunP,    
               vectorHandling< analyticGeometry * > const * const depAGeoP,
               vectorHandling< analyticGeometry * > * aGeoP ) const {
    bool hasAGeo = hasChild("analyticGeometry", **toBuildP);

    if ( hasAGeo ) { 
      //
      // copy
      //
      QDomElement wElement = getChild("analyticGeometry", **toBuildP);     
      vectorHandling< analyticGeometry * > toCopy;
      this->createAdvanced( 
				&wElement, bC, cValP, sFunP, depAGeoP, &toCopy 
			);
      dt__FORALL(toCopy, ii,
        aGeoP->push_back( toCopy[ii] );
      );
    }
  }
}
