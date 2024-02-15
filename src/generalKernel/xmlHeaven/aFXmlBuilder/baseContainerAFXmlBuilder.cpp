/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "baseContainerAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool baseContainerAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(baseContainerAFXmlBuilder, new baseContainerAFXmlBuilder())
  );
  
  baseContainerAFXmlBuilder::baseContainerAFXmlBuilder() {
  }

  baseContainerAFXmlBuilder::~baseContainerAFXmlBuilder() {
  }

  void baseContainerAFXmlBuilder::buildPart(
      ::QDomElement const & toBuildP,
			baseContainer * const bC,
      lvH_constValue const * const cValP,  
      lvH_analyticFunction const * const depSFunP,
      lvH_analyticFunction * sFunP
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
