#include "dtPlugin.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>


namespace dtOO {  
  dtPlugin::dtPlugin() { 
  }

  dtPlugin::~dtPlugin() {
  }

  void dtPlugin::init(
	  QDomElement const & element,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtPlugin * > const * const pL
	) {  
		DTINFOWF(
			init(),
			<< "Init dtPlugin ..."
	  );		
		
    //
    // set label of dtPlugin
    //
    labelHandling::setLabel( getAttributeStr("label", element ) );
    
    //
    // set options
    //
    optionHandling::init( &element );
  }
}


