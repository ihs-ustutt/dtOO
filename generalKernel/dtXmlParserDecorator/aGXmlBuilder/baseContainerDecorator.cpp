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
    QDomElement const & toBuild,
    baseContainer * const bC,
    vectorHandling< constValue * > const * const cV,  
    vectorHandling< analyticFunction * > const * const aF,  
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {
   
    std::vector< QDomElement > wElement = getChildVector(toBuild);
		
    //dt__FORALL(wElement, ii,
		for (int ii=0; ii<wElement.size(); ii++) {
			if ( is("Point_3", wElement[ii]) ) {
				vectorHandling< dtPoint3 > workingPointP;
				createBasic(&wElement[ii], bC, cV, aF, aG, &workingPointP);
			}
		  else if ( is("Vector_3", wElement[ii]) ) {
				dtVector3 workingVectorP 
				= 
				createDtVector3(&wElement[ii], bC, cV, aF, aG);
			}
		  else if ( is("transformer", wElement[ii]) ) {
				dt__pH(dtTransformer) dtT(
				  createTransformer(&wElement[ii], bC, cV, aF, aG)
				);
//				if ( hasAttribute("label", wElement[ii]) ) {
//				  bC->ptrTransformerContainer()->add(dtT.get());
//				}
			}		
    }
  }
}

