#include "vec2dInMap2dTo3dDecorator.h"
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
  vec2dInMap2dTo3dDecorator::vec2dInMap2dTo3dDecorator() {
  }

  vec2dInMap2dTo3dDecorator::~vec2dInMap2dTo3dDecorator() {
  }

  void vec2dInMap2dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    baseContainer * const bC,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
		
    bool hasAG = hasChild("analyticGeometry", **toBuildP);
		bool hasAF = hasChild("analyticFunction", **toBuildP);
    bool optionPercent = false;
		if ( hasAttribute("percent", **toBuildP) ) {
		  optionPercent = getAttributeBool("percent", **toBuildP);
		}
		
    if ( hasAG && hasAF ) {
      //
      // get analyticGeometry
      //
			QDomElement aGElement = getChild("analyticGeometry", **toBuildP);
      dt__pH(analyticGeometry const) aG(
        createAnalyticGeometry(&aGElement, bC, cValP, sFunP, depAGeoP)
		  );
        
      //
      // check if it is a map2dTo3d
      //
      dt__PTRASS( map2dTo3d const * m2d, map2dTo3d::ConstDownCast(aG.get()) );
			
      //
      // get analyticFunction
      //
			QDomElement aFElement = getChild("analyticFunction", **toBuildP);
      dt__pH(analyticFunction const) aF(
			  createAnalyticFunction(&aFElement, bC, cValP, sFunP)
			);

			vec2dOneD const * v1d = vec2dOneD::ConstDownCast(aF.get());
			vec2dTwoD const * v2d = vec2dTwoD::ConstDownCast(aF.get());
			
			if (v1d) {
				aGeoP->push_back( new vec2dOneDInMap2dTo3d(v1d, m2d, optionPercent) );
			}
			else if (v2d) {
			  aGeoP->push_back( new vec2dTwoDInMap2dTo3d(v2d, m2d, optionPercent) );
			}
			else {
				dt__THROW(
				  buildPart(), 
					<< DTLOGEVAL(v1d) << LOGDEL
					<< DTLOGEVAL(v2d)
				);
			}
    }
  }
}
