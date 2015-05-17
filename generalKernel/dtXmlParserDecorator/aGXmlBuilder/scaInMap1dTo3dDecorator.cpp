#include "scaInMap1dTo3dDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/scaOneDInMap1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  scaInMap1dTo3dDecorator::scaInMap1dTo3dDecorator() {
  }

  scaInMap1dTo3dDecorator::~scaInMap1dTo3dDecorator() {
  }

  void scaInMap1dTo3dDecorator::buildPart( 
    QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    //
		// check input
		//		
    dt__throwIf(!hasChild("analyticGeometry", toBuild), buildPart());
    dt__throwIf(!hasChild("analyticFunction", toBuild), buildPart());
    bool optionPercent = getAttributeBool("percent", toBuild);
		

    //
    // get analyticGeometry
    //
    QDomElement aGElement = getChild("analyticGeometry", toBuild);
    dt__pH(analyticGeometry const) aG_t(
      createAnalyticGeometry(&aGElement, bC, cV, aF, aG)
    );

    //
    // check if it is a map1dTo3d
    //
    dt__ptrAss( map1dTo3d const * m1d, map1dTo3d::ConstDownCast(aG_t.get()) );

    //
    // get analyticFunction
    //
    QDomElement aFElement = getChild("analyticFunction", toBuild);
    dt__pH(analyticFunction const) aF_t(
      createAnalyticFunction(&aFElement, bC, cV, aF)
    );
    dt__ptrAss( scaOneD const * s1d, scaOneD::ConstDownCast(aF_t.get()) );

    //
    // create and return scaOneDInMap1dTo3d
    //
    result->push_back( new scaOneDInMap1dTo3d(s1d, m1d, optionPercent) );
  }
}
