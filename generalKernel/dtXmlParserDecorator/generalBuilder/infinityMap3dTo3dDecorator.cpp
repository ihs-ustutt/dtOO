#include "infinityMap3dTo3dDecorator.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/infinityMap3dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  infinityMap3dTo3dDecorator::infinityMap3dTo3dDecorator() {
  }

  infinityMap3dTo3dDecorator::~infinityMap3dTo3dDecorator() {
  }

  void infinityMap3dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    baseContainer * const bC,        
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    aGeoP->push_back( new infinityMap3dTo3d() );
  }
}
