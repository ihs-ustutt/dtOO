#ifndef DTPLUGINDRIVER_H
#define	DTPLUGINDRIVER_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Driver.h>

namespace dtOO {
  class dtPlugin;
  
  class dtPluginDriver : public ::pugg::Driver {
    public:
      dt__classOnlyName(dtPluginDriver);       
      dtPluginDriver(std::string name, int version);
      ~dtPluginDriver();
      virtual dtPlugin * create( void ) = 0;
    private:

  };
}

#endif	/* DTPLUGINDRIVER_H */

