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
      // check if it is a map1dTo3d
      //
      dt__ptrAss( map1dTo3d const * m1d, map1dTo3d::ConstDownCast(aG.get()) );
			
      //
      // get analyticFunction
      //
			QDomElement aFElement = getChild("analyticFunction", **toBuildP);
      dt__pH(analyticFunction const) aF(
			  createAnalyticFunction(&aFElement, bC, cValP, sFunP)
			);

			scaOneD const * s1d = scaOneD::ConstDownCast(aF.get());
			
			if (s1d) {
				aGeoP->push_back( new scaOneDInMap1dTo3d(s1d, m1d, optionPercent) );
			}
			else {
				dt__THROW(buildPart(), << DTLOGEVAL(s1d));
			}
    }
  }
}
