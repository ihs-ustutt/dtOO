#include "translatingMap2dTo3dAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/translatingMap2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  translatingMap2dTo3dAGXmlBuilder::translatingMap2dTo3dAGXmlBuilder() {
  }

  translatingMap2dTo3dAGXmlBuilder::~translatingMap2dTo3dAGXmlBuilder() {
  }

  void translatingMap2dTo3dAGXmlBuilder::buildPart( 
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
    dt__throwIf(!dtXmlParserBase::hasChild("Vector_3", toBuild), buildPart());
    
    ::QDomElement wElement;			
    //
    // get analyticGeometry and cast
    //
    wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    dt__pH(analyticGeometry) aG_t(
      dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
    );
    dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(aG_t.get()));

    //
    // get vector
    //
    wElement = dtXmlParserBase::getChild("Vector_3", toBuild);
    dtVector3 vv
    =				
    dtXmlParserBase::createDtVector3(&wElement, bC, cV, aF, aG);

    //
    // create analyticGeometry
    //
    result->push_back( new translatingMap2dTo3d(vv, m2d) );
  }
}
