#include "analyticGeometryDecorator.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  analyticGeometryDecorator::analyticGeometryDecorator() {
  }

  analyticGeometryDecorator::~analyticGeometryDecorator() {
  }

  void analyticGeometryDecorator::buildPart( 
               QDomElement ** toBuildP,
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,            
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
      this->createAdvanced( &wElement, 
                            pointContainerP, 
                            vectorContainerP, 
                            cValP, 
                            sFunP,
                            depAGeoP,
                            &toCopy );
      dt__FORALL(toCopy, ii,
        aGeoP->push_back( toCopy[ii] );
      );
    }
  }
}
