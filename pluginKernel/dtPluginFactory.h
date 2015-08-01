#ifndef DTPLUGINFACTORY_H
#define	DTPLUGINFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPlugin;
  
  class dtPluginFactory {
  public:
    dt__classOnlyName(dtPluginFactory);
    dtPluginFactory();
    virtual ~dtPluginFactory();
    static dtPlugin * create(char const * const str);
    static dtPlugin * create( std::string const str );
    static dtPlugin * createFromPlugin(
      std::string const & className, 
      std::string const & pluginName, 
      std::string const & pluginDriver
    );
  private:

  };
}

#endif	/* DTPLUGINFACTORY_H */

