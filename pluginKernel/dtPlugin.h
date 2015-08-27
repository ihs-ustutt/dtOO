#ifndef DTPLUGIN_H
#define	DTPLUGIN_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <interfaceHeaven/vectorHandling.h>

namespace pugg {
  class Kernel;
}

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class dtPlugin : public labelHandling, public optionHandling {
    public:
      dt__classOnlyName(dtPlugin);
      dtPlugin();
      virtual ~dtPlugin();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        vectorHandling< dtPlugin * > const * const pL
      );
      virtual void apply( void ) = 0;    
      //
      // pugg stuff
      //
      static const int version = 1;
      static const std::string server_name() {
        return "dtPluginServer";
      }   
      void setKernel( ::pugg::Kernel * kernel );
    private:
      dt__pH(::pugg::Kernel) _kernel;
  };
}
#endif	/* DTPLUGIN_H */

