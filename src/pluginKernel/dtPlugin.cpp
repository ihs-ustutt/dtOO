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
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC,
    lvH_dtPlugin const * const pL
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
  
  void dtPlugin::apply( void ) {
    dt__throwUnexpected(apply());
  }
       
  std::vector< std::string > dtPlugin::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
  
  void dtPlugin::setKernel( ::pugg::Kernel * kernel ) {
    _kernel.reset( kernel );
  }
  
}


