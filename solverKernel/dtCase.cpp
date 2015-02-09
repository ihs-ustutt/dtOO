#include "dtCase.h"
#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

namespace dtOO {
	dtCase::dtCase() {
	}

	dtCase::~dtCase() {
	}
	
  void dtCase::init(
	  QDomElement const & element,
		baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtCase * > const * const dC
	) {  
    //
    // set label of dtPlugin
    //
    labelHandling::setLabel( 
		  dtXmlParserBase::getAttributeStr("label", element ) 
		);
    
    //
    // set options
    //
    optionHandling::init( &element );
  }	
}
