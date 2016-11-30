#ifndef helloWorldPlugin_H
#define	helloWorldPlugin_H

#include <pluginEngine/pugg/Driver.h>
#include <dtPlugin.h>
#include <dtPluginDriver.h>

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;  
  
  class helloWorldPlugin : public dtPlugin {
    public:
      dt__class(helloWorldPlugin, dtPlugin);
      helloWorldPlugin();
      virtual ~helloWorldPlugin();
      virtual helloWorldPlugin * create( void ) const;
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        dCPtrVec const * const dC,        
        dPPtrVec const * const pL
      );
      virtual void apply(void);
    private:
  };
}

class helloWorldPluginDriver : public dtOO::dtPluginDriver {
public:
    helloWorldPluginDriver();
    dtOO::dtPlugin * create();
};

#endif	/* helloWorldPlugin_H */