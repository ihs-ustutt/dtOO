#ifndef dtPlugin_H
#define	dtPlugin_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <interfaceHeaven/vectorHandling.h>
#include <mainConceptFwd.h>

namespace pugg {
  class Kernel;
}

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtCase;
  
  class dtPlugin : public labelHandling, public optionHandling {
    public:
      dt__class(dtPlugin, dtPlugin);
      dtPlugin();
      virtual ~dtPlugin();
      virtual dtPlugin * create( void ) const = 0;      
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
      virtual void apply( void ) = 0;    
      virtual std::vector< std::string > factoryAlias( void ) const;      
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
#endif	/* dtPlugin_H */

