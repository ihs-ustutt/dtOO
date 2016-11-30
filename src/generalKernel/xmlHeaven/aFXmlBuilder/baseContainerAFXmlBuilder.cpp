#include "baseContainerAFXmlBuilder.h"
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  baseContainerAFXmlBuilder::baseContainerAFXmlBuilder() {
  }

  baseContainerAFXmlBuilder::~baseContainerAFXmlBuilder() {
  }

  void baseContainerAFXmlBuilder::buildPart(
      ::QDomElement const & toBuildP,
			baseContainer * const bC,
      cVPtrVec const * const cValP,  
      aFPtrVec const * const depSFunP,
      aFPtrVec * sFunP
	) const {
    std::vector< ::QDomElement > wElement = dtXmlParserBase::getChildVector(toBuildP);
		
    //dt__FORALL(wElement, ii,
		for (int ii=0; ii<wElement.size(); ii++) {
			if ( dtXmlParserBase::is("Point_3", wElement[ii]) ) {
				vectorHandling< dtPoint3 > workingPointP;
				dtXmlParserBase::createBasic(&wElement[ii], bC, cValP, depSFunP, &workingPointP);
			}
		  else if ( dtXmlParserBase::is("Vector_3", wElement[ii]) ) {
				dtVector3 workingVectorP 
				= 
				dtXmlParserBase::createDtVector3(&wElement[ii], bC, cValP, depSFunP);
			}
		  else if ( dtXmlParserBase::is("transformer", wElement[ii]) ) {
				dt__pH(dtTransformer) dtT(
				  dtXmlParserBase::createTransformer(&wElement[ii], bC, cValP, depSFunP)
				);
			}		
    }
  }
}

