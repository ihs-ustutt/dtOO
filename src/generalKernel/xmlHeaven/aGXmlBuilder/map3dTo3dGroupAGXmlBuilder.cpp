#include "map3dTo3dGroupAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3dGroup.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  map3dTo3dGroupAGXmlBuilder::map3dTo3dGroupAGXmlBuilder() {
  }

  map3dTo3dGroupAGXmlBuilder::~map3dTo3dGroupAGXmlBuilder() {
  }

  void map3dTo3dGroupAGXmlBuilder::buildPart( 
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

    vectorHandling< map3dTo3d const * > m3d;
    
    dt__forAllRefAuto( 
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), wE
    ) {
      m3d.push_back( 
        map3dTo3d::MustDownCast(
          dtXmlParserBase::createAnalyticGeometry(wE, bC, cV, aF, aG)
        )
      );
    }
    
    result->push_back( new map3dTo3dGroup(m3d) );
      
    //
    // free memory
    //
    m3d.destroy();
  }
}
