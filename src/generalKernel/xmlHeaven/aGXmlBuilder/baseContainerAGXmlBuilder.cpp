#include "baseContainerAGXmlBuilder.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  baseContainerAGXmlBuilder::baseContainerAGXmlBuilder() {
  }

  baseContainerAGXmlBuilder::~baseContainerAGXmlBuilder() {
  }

  void baseContainerAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    cVPtrVec const * const cV,  
    aFPtrVec const * const aF,  
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {
   
    std::vector< ::QDomElement > wElement 
    = 
    dtXmlParserBase::getChildVector(toBuild);
		
    //dt__FORALL(wElement, ii,
		for (int ii=0; ii<wElement.size(); ii++) {
			if ( dtXmlParserBase::is("Point_3", wElement[ii]) ) {
				vectorHandling< dtPoint3 > workingPointP;
				dtXmlParserBase::createBasic(
          &wElement[ii], bC, cV, aF, aG, NULL, &workingPointP
        );
			}
		  else if ( dtXmlParserBase::is("Vector_3", wElement[ii]) ) {
				dtVector3 workingVectorP 
				= 
				dtXmlParserBase::createDtVector3(&wElement[ii], bC, cV, aF, aG);
			}
		  else if ( dtXmlParserBase::is("transformer", wElement[ii]) ) {
				dt__pH(dtTransformer) dtT(
				  dtXmlParserBase::createTransformer(&wElement[ii], bC, cV, aF, aG)
				);
			}		
    }
  }
}

