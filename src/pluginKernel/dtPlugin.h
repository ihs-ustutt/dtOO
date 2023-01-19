#ifndef dtPlugin_H
#define	dtPlugin_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <interfaceHeaven/labeledVectorHandling.h>
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
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC,
        lvH_dtPlugin const * const pL
      );
      virtual void apply( void ) = 0;    
      virtual std::vector< std::string > factoryAlias( void ) const;      
      //
      // pugg stuff
      //
      static const dtInt version = 1;
      static const std::string server_name() {
        return "dtPluginServer";
      }   
      void setKernel( ::pugg::Kernel * kernel );
    private:
      dt__pH(::pugg::Kernel) _kernel;
  };
}
#endif	/* dtPlugin_H */

