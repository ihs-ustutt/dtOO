#include "dtPluginFactory.h"

#include <logMe/logMe.h>
#include "dtPlugin.h"
#include "writeStep.h"
#include "writeTurboGrid.h"

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
    DTINFOWF(create(), << "creating " << str <<  "...");
    
		__IFRET(writeStep);
		__IFRET(writeTurboGrid);

    dt__THROW(create(), << str <<  " could not be created");  
  }

  dtPlugin * dtPluginFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}