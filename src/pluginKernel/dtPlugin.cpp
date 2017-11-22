#include "dtPlugin.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <pluginEngine/pugg/Kernel.h>

namespace dtOO {  
  dtPlugin::dtPlugin() { 
  }

  dtPlugin::~dtPlugin() {
  }

  void dtPlugin::init(
	  ::QDomElement const & element,
		baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    dCPtrVec const * const dC,
    dPPtrVec const * const pL
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
    optionHandling::init( element, bC, cV, aF, aG );
  }
  
  std::vector< std::string > dtPlugin::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
  
  void dtPlugin::setKernel( ::pugg::Kernel * kernel ) {
    _kernel.reset( kernel );
  }
  
}


