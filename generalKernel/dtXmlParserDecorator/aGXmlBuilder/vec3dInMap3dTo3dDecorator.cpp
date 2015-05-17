#include "vec3dInMap3dTo3dDecorator.h"
#include "analyticFunctionHeaven/vec3dTransVolThreeD.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dInMap3dTo3dDecorator::vec3dInMap3dTo3dDecorator() {
  }

  vec3dInMap3dTo3dDecorator::~vec3dInMap3dTo3dDecorator() {
  }

  void vec3dInMap3dTo3dDecorator::buildPart( 
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
    // check if it is a map3dTo3d
    //
    dt__ptrAss( map3dTo3d const * m3d, map3dTo3d::ConstDownCast(aG_t.get()) );

    //
    // get analyticFunction
    //
    QDomElement aFElement = getChild("analyticFunction", toBuild);
    dt__pH(analyticFunction const) aF_t(
      createAnalyticFunction(&aFElement, bC, cV, aF)
    );

    vec3dOneD const * v1d = vec3dOneD::ConstDownCast(aF_t.get());
    vec3dTwoD const * v2d = vec3dTwoD::ConstDownCast(aF_t.get());
    vec3dThreeD const * v3d = vec3dThreeD::ConstDownCast(aF_t.get());

    if (v1d) {
      result->push_back( new vec3dOneDInMap3dTo3d(v1d, m3d, optionPercent) );
    }
    else if (v2d) {
      result->push_back( new vec3dTwoDInMap3dTo3d(v2d, m3d, optionPercent) );
    }
    else if (v3d) {
      result->push_back( new vec3dThreeDInMap3dTo3d(v3d, m3d, optionPercent) );
    }
    else {
      dt__throw(
        buildPart(), 
        << dt__eval(v1d) << std::endl
        << dt__eval(v2d) << std::endl
        << dt__eval(v3d)
      );
    }
  }
	
  void vec3dInMap3dTo3dDecorator::buildPartCompound( 
    QDomElement const & toBuild,
    baseContainer * const bC,
    vectorHandling< constValue * > const * const cV,  
    vectorHandling< analyticFunction * > const * const aF,  
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
  ) const {
    dt__throwIf(!hasChild("analyticGeometry", toBuild), buildPart());
		dt__throwIf(!hasChild("analyticFunction", toBuild), buildPart());

//    if ( hasAG && hasAF ) {
      //
      // get analyticGeometry
      //
      analyticGeometry const * aG_t 
      = 
      aG->get( 
        getAttributeStr("label", getChild( "analyticGeometry", toBuild )) 
      );
      //
      // check if it is a map3dTo3d
      //
      dt__ptrAss( 
			  map3dTo3d const * m3d, 
				map3dTo3d::ConstDownCast(aG_t) 
			);
			
      //
      // get analyticFunction
      //
      analyticFunction const * aF_t
      = 
      aF->get( 
        getAttributeStr("label", getChild( "analyticFunction", toBuild )) 
      );

//			vec3dOneD const * v1d = vec3dOneD::ConstDownCast(aF);
//			vec3dTwoD const * v2d = vec3dTwoD::ConstDownCast(aF);
			vec3dTransVolThreeDCompound const * v3d 
      = 
      vec3dTransVolThreeDCompound::ConstDownCast(aF_t);
//			
//			if (v1d) {
//				aGeoP->push_back( new vec3dOneDInMap3dTo3d(v1d, m3d) );
//			}
//			else if (v2d) {
//			  aGeoP->push_back( new vec3dTwoDInMap3dTo3d(v2d, m3d) );
//			}
			if (v3d) {
				vec3dThreeDInMap3dTo3dCompound * v3dC = new vec3dThreeDInMap3dTo3dCompound();
				for (int ii=0; ii<v3d->nComponents(); ii++) {
					v3dC->addComponent( vec3dThreeDInMap3dTo3d( &(v3d->component(ii)), m3d) );
//			    aGeoP->push_back( new vec3dThreeDInMap3dTo3d(v3d, m3d) );
				}
				result->push_back( v3dC );
			}
			else {
				dt__throw(
				  buildPart(), 
//					<< dt__eval(v1d) << std::endl
//					<< dt__eval(v2d) << std::endl
					<< dt__eval(v3d)
				);
			}
//    }
  }	
}
