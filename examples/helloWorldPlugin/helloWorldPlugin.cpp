#include "helloWorldPlugin.h"

#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO {
  helloWorldPlugin::helloWorldPlugin() : dtPlugin() {
    dt__info(
      helloWorldPlugin(), 
      << "Create helloWorldPlugin"
    );
  }

  helloWorldPlugin::~helloWorldPlugin() {
    dt__info(
      helloWorldPlugin(), 
      << "Destroy helloWorldPlugin"
    );
  }

  helloWorldPlugin * helloWorldPlugin::create( void ) const {
    return new helloWorldPlugin();  
  }
  
  void helloWorldPlugin::init( 
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtCase * > const * const dC,    
    vectorHandling< dtPlugin * > const * const pL
  ) {
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

    dt__info(
      helloWorldPlugin(), 
      << "Init helloWorldPlugin"
    );    
    
    //
    // dump for example a vector
    //
    aG->dump();
  }

  void helloWorldPlugin::apply(void) {
    dt__info(
      helloWorldPlugin(), 
      << "Apply helloWorldPlugin"
    );          
  }
}

//
// pugg driver
//
helloWorldPluginDriver::helloWorldPluginDriver() : dtPluginDriver(
  std::string("helloWorldPluginDriver"), dtOO::dtPlugin::version
) {

}

//
// pugg plugin creation via driver
//
dtOO::dtPlugin * helloWorldPluginDriver::create() {
  return new dtOO::helloWorldPlugin();
}

//
// registrate plugin in driver
//
#include <pluginEngine/pugg/Kernel.h>
extern "C" void register_pugg_plugin(pugg::Kernel* kernel) {
  kernel->add_driver(new helloWorldPluginDriver());
}


