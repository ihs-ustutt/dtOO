#include "baseContainerDecorator.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  baseContainerDecorator::baseContainerDecorator() {
  }

  baseContainerDecorator::~baseContainerDecorator() {
  }

  void baseContainerDecorator::buildPart(
	  QDomElement ** toBuildP,
    baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP,        
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {
   
    std::vector< QDomElement > wElement = getChildVector(**toBuildP);
		
    //dt__FORALL(wElement, ii,
		for (int ii=0; ii<wElement.size(); ii++) {
			if ( is("Point_3", wElement[ii]) ) {
				vectorHandling< dtPoint3 > workingPointP;
				this->createBasic( 
					&wElement[ii], bC, cValP, sFunP, depAGeoP, &workingPointP 
				);
			}
		  else if ( is("Vector_3", wElement[ii]) ) {
				dtVector3 workingVectorP 
				= 
				createDtVector3(&wElement[ii], bC, cValP, sFunP, depAGeoP);
			}
		  else if ( is("transformer", wElement[ii]) ) {
				dt__pH(dtTransformer) dtT(
				  createTransformer(&wElement[ii], bC, cValP, sFunP, depAGeoP)
				);
//				if ( hasAttribute("label", wElement[ii]) ) {
//				  bC->ptrTransformerContainer()->add(dtT.get());
//				}
			}		
    }
  }
}
