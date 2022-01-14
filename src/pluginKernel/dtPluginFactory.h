#ifndef DTPLUGINFACTORY_H
#define	DTPLUGINFACTORY_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class dtPlugin;
  
  class dtPluginFactory {
    public:
      dt__classOnlyName(dtPluginFactory);
      virtual ~dtPluginFactory();
      static bool registrate( dtPlugin const * const );    
      static dtPlugin * create( std::string const str );
      static dtPlugin * createFromPlugin(
        std::string const & className, 
        std::string const & pluginName, 
        std::string const & pluginDriver
      );
    private:
      dtPluginFactory();    
    private:
      static dt__pVH(dtPlugin) _builder;
  };
}

#endif	/* DTPLUGINFACTORY_H */

