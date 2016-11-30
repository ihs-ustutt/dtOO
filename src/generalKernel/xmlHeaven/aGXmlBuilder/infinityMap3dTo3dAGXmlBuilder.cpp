#include "infinityMap3dTo3dAGXmlBuilder.h"
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
  infinityMap3dTo3dAGXmlBuilder::infinityMap3dTo3dAGXmlBuilder() {
  }

  infinityMap3dTo3dAGXmlBuilder::~infinityMap3dTo3dAGXmlBuilder() {
  }

  void infinityMap3dTo3dAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
  ) const {
    result->push_back( new infinityMap3dTo3d() );
  }
}
