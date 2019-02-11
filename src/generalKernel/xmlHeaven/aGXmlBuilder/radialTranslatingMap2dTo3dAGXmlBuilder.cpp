#include "radialTranslatingMap2dTo3dAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/radialTranslatingMap2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  radialTranslatingMap2dTo3dAGXmlBuilder::radialTranslatingMap2dTo3dAGXmlBuilder() {
  }

  radialTranslatingMap2dTo3dAGXmlBuilder::~radialTranslatingMap2dTo3dAGXmlBuilder() {
  }

  void radialTranslatingMap2dTo3dAGXmlBuilder::buildPart( 
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
    dt__throwIf(
      !dtXmlParserBase::hasChild("Vector_3", toBuild)
      &&
      !dtXmlParserBase::hasAttribute("distance", toBuild), 
      buildPart()
    );
    
    //
    // get analyticGeometry and cast
    //
    dt__pH(analyticGeometry) aG_t(
      dtXmlParserBase::createAnalyticGeometry(
        dtXmlParserBase::getChild("analyticGeometry", toBuild), 
        bC, 
        cV, 
        aF, 
        aG
      )
    );
    map2dTo3d const * m2d = map2dTo3d::MustConstDownCast(aG_t.get());

    //
    // get vector
    //
    :: QDomElement wElement = dtXmlParserBase::getChild("Vector_3", toBuild);
    dtVector3 vv
    =				
    dtXmlParserBase::createDtVector3(&wElement, bC, cV, aF, aG);
    float distance 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("distance", toBuild, cV, aF, aG);
    //
    // create analyticGeometry
    //
    result->push_back( new radialTranslatingMap2dTo3d(vv, distance, m2d) );
  }
}
