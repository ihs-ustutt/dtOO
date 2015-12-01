#include "dtPluginFactory.h"

#include <logMe/logMe.h>
#include "dtPlugin.h"
#include "writeStep.h"
#include "constValueAssingRule.h"
//#include "createOpenFOAMCase/createOpenFOAMCase.h"

#include "dtPluginDriver.h"
#include "dtPluginKernel.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  dtPluginFactory::dtPluginFactory() {
  }

  dtPluginFactory::~dtPluginFactory() {
  }

//  dtPlugin * dtPluginFactory::create(char const * const str) {
//    dt__info(create(), << "creating " << str <<  "...");
//    
//		__IFRET(writeStep);
//    __IFRET(constValueAssingRule);
////    __IFRET(createOpenFOAMCase);
//    
//    dt__throw(create(), << str <<  " could not be created");  
//  } 

  dtPlugin * dtPluginFactory::create( std::string const str ) {
    dt__info(create(), << "creating " << str <<  "...");
    
		if (str == "writeStep") return new writeStep();
    if (str == "constValueAssingRule") return new constValueAssingRule();
    
    dt__throw(create(), << str <<  " could not be created");  
  }
  
  
  dtPlugin * dtPluginFactory::createFromPlugin(
    std::string const & str, 
    std::string const & pluginName, 
    std::string const & pluginDriver
  ) {
    dt__info( createFromPlugin(), << "creating " << str <<  "..." );
  
    //
    // init kernel
    //
    dtPluginKernel * kernel = new dtPluginKernel();
    kernel->add_server(dtPlugin::server_name(), dtPlugin::version);  

    //
    // output
    //
    dt__info(
      createFromPlugin(), 
      << dt__eval(pluginName) << std::endl
      << dt__eval(pluginDriver)
    );    
    
    //
    // load shared library
    //
    dt__throwIf( 
      !kernel->load_plugin(pluginName), << "Loading plugin failed."
    );

    //
    // create plugin using plugin driver
    //
    dtPlugin *  ret 
    = 
    kernel->get_driver< dtPluginDriver >(
      dtPlugin::server_name(), pluginDriver
    )->create();

    //
    // store kernel in plugin
    //
    ret->setKernel(kernel);
    
    return ret;
  }  
}