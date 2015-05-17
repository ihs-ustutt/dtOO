#include "rotatingMap2dTo3dDecorator.h"


#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  rotatingMap2dTo3dDecorator::rotatingMap2dTo3dDecorator() {
  }

  rotatingMap2dTo3dDecorator::~rotatingMap2dTo3dDecorator() {
  }

  void rotatingMap2dTo3dDecorator::buildPart( 
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
    dt__throwIf(!hasChild("Vector_3", toBuild), buildPart());
    
    QDomElement wElement;			
    //
    // get analyticGeometry and cast
    //
    wElement = getChild("analyticGeometry", toBuild);
    analyticGeometry * aG_t
    =
    this->createAnalyticGeometry(&wElement, bC, cV, aF, aG);
    dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(aG_t));

    //
    // get vector
    //
    wElement = getChild("Vector_3", toBuild);
    dtVector3 vv
    =				
    this->createDtVector3(&wElement, bC, cV, aF, aG);

    //
    // create analyticGeometry
    //
    result->push_back( new rotatingMap2dTo3d(vv, m2d) );
  }
}
