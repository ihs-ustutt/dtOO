#include "map2dTo3dGroupAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3dGroup.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  map2dTo3dGroupAGXmlBuilder::map2dTo3dGroupAGXmlBuilder() {
  }

  map2dTo3dGroupAGXmlBuilder::~map2dTo3dGroupAGXmlBuilder() {
  }

  void map2dTo3dGroupAGXmlBuilder::buildPart( 
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

    vectorHandling< map2dTo3d const * > m2d;
    
    dt__forAllRefAuto( 
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), wE
    ) {
      m2d.push_back( 
        map2dTo3d::MustDownCast(
          dtXmlParserBase::createAnalyticGeometry(wE, bC, cV, aF, aG)
        )
      );
    }
    
    result->push_back( new map2dTo3dGroup(m2d) );
      
    //
    // free memory
    //
    m2d.destroy();
  }
}