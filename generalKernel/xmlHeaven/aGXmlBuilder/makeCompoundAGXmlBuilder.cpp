#include "makeCompoundAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  makeCompoundAGXmlBuilder::makeCompoundAGXmlBuilder() {
  }

  makeCompoundAGXmlBuilder::~makeCompoundAGXmlBuilder() {
  }

  void makeCompoundAGXmlBuilder::buildPart( 
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

    //
    // get analyticGeometry
    //
    std::vector<::QDomElement> aGElVec 
    = 
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild);

    int ii = 0;
    dt__pH(analyticGeometry const) aG_t(
      dtXmlParserBase::createAnalyticGeometry(&aGElVec[ii], bC, cV, aF, aG)
    );

    //
    // check if it is a map3dTo3d
    //
    dt__ptrAss( 
      vec3dThreeDInMap3dTo3d const * v3dIn3d, 
      vec3dThreeDInMap3dTo3d::ConstDownCast(aG_t.get()) 
    );

    analyticGeometryCompound<vec3dThreeDInMap3dTo3d> * v3dIn3dC 
    = 
    new analyticGeometryCompound<vec3dThreeDInMap3dTo3d>();

    dt__forAllIndex(aGElVec, ii) {
      aG_t.reset( dtXmlParserBase::createAnalyticGeometry(&aGElVec[ii], bC, cV, aF, aG) );

      //
      // check if it is a map3dTo3d
      //
      dt__ptrAss( 
        vec3dThreeDInMap3dTo3d const * v3dIn3d, 
        vec3dThreeDInMap3dTo3d::ConstDownCast(aG_t.get()) 
      );
      v3dIn3dC->addComponent(*v3dIn3d);
    }

    result->push_back(v3dIn3dC);
  }
}
