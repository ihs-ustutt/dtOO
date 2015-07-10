#include "vec2dInMap2dTo3dAGXmlBuilder.h"
#include "analyticFunctionHeaven/vec3dTransVolThreeD.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <analyticGeometryHeaven/vec2dTwoDInMap2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec2dTwoD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec2dInMap2dTo3dAGXmlBuilder::vec2dInMap2dTo3dAGXmlBuilder() {
  }

  vec2dInMap2dTo3dAGXmlBuilder::~vec2dInMap2dTo3dAGXmlBuilder() {
  }

  void vec2dInMap2dTo3dAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    vectorHandling< constValue * > const * const cV,  
    vectorHandling< analyticFunction * > const * const aF,  
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    //
		// check input
		//    
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart());
    bool optionPercent = dtXmlParserBase::getAttributeBool("percent", toBuild);
		
    //
    // get analyticGeometry
    //
    ::QDomElement aGElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    dt__pH(analyticGeometry const) aG_t(
      dtXmlParserBase::createAnalyticGeometry(&aGElement, bC, cV, aF, aG)
    );

    //
    // check if it is a map2dTo3d
    //
    dt__ptrAss( map2dTo3d const * m2d, map2dTo3d::ConstDownCast(aG_t.get()) );

    //
    // get analyticFunction
    //
    ::QDomElement aFElement = dtXmlParserBase::getChild("analyticFunction", toBuild);
    dt__pH(analyticFunction const) aF_t(
      dtXmlParserBase::createAnalyticFunction(&aFElement, bC, cV, aF)
    );

    vec2dOneD const * v1d = vec2dOneD::ConstDownCast(aF_t.get());
    vec2dTwoD const * v2d = vec2dTwoD::ConstDownCast(aF_t.get());

    if (v1d) {
      result->push_back( new vec2dOneDInMap2dTo3d(v1d, m2d, optionPercent) );
    }
    else if (v2d) {
      result->push_back( new vec2dTwoDInMap2dTo3d(v2d, m2d, optionPercent) );
    }
    else {
      dt__throw(
        buildPart(), 
        << dt__eval(v1d) << std::endl
        << dt__eval(v2d)
      );
    }
  }
}
