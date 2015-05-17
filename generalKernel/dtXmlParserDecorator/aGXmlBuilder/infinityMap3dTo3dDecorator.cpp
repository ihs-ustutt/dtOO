#include "infinityMap3dTo3dDecorator.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/infinityMap3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
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
    QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    result->push_back( new infinityMap3dTo3d() );
  }
}
