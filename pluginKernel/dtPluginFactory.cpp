#include "dtPluginFactory.h"

#include <logMe/logMe.h>
#include "dtPlugin.h"
#include "writeStep.h"
#include "constValueAssingRule.h"
#include "dtPluginDriver.h"
#include <pluginEngine/pugg/Kernel.h>

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

  dtPlugin * dtPluginFactory::create(char const * const str) {
    dt__info(create(), << "creating " << str <<  "...");
    
		__IFRET(writeStep);
    __IFRET(constValueAssingRule);
    
    dt__throw(create(), << str <<  " could not be created");  
  } 

  dtPlugin * dtPluginFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
  
  
  dtPlugin * dtPluginFactory::createFromPlugin(
    std::string const & str, 
    std::string const & pluginName, 
    std::string const & pluginDriver
  ) {
    dt__info(
      createFromPlugin(), 
      << "create dtPlugin " << str <<  " ..."
    );
  
    ::pugg::Kernel * kernel = new ::pugg::Kernel();
    kernel->add_server(dtPlugin::server_name(), dtPlugin::version);  

    dt__info(
      createFromPlugin(), 
      << "load " << pluginName <<  " ..."
    );    
    
// #ifdef WIN32      
//     kernel.load_plugin(pluginName); // Cat class is in this dll.
// #else
    kernel->load_plugin(pluginName);
// #endif

    dt__info(
      createFromPlugin(), 
      << "using " << pluginDriver <<  " ..."
    );    
    


    dtPlugin *  ret 
    = 
    kernel->get_driver< dtPluginDriver >(
      dtPlugin::server_name(), pluginDriver
    )->create();

    ret->apply();
    
    ret->setKernel(kernel);
    
//    return new dtPlugin( *plugin );
    
    return ret;
//    dt__throw(create(), << str <<  " could not be created");  
  }  
}