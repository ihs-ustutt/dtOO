#ifndef DTPLUGINFACTORY_H
#define	DTPLUGINFACTORY_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtPlugin;
  
  class dtPluginFactory {
  public:
    dt__classOnlyName(dtPluginFactory);
    virtual ~dtPluginFactory();
//    static dtPlugin * create(char const * const str);
    static dtPlugin * create( std::string const str );
    static dtPlugin * createFromPlugin(
      std::string const & className, 
      std::string const & pluginName, 
      std::string const & pluginDriver
    );
    static dtPluginFactory * instance( void );    
  private:
    dtPluginFactory();    
  private:
    vectorHandling< dtPlugin * > _builder;
    static dt__pH(dtPluginFactory) _instance;
  };
}

#endif	/* DTPLUGINFACTORY_H */

