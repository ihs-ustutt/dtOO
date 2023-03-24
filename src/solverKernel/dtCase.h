#ifndef dtCase_H
#define	dtCase_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <mainConceptFwd.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class resultValue;
  
  class dtCase : public labelHandling, public optionHandling {
    public:
      dt__class(dtCase, labelHandling);
      dtCase();
      virtual ~dtCase();
      virtual dtCase * create( void ) const = 0;
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC
      );
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC
      );    
      virtual void runCurrentState( void ) = 0;
      std::string createDirectory( std::string const & state ) const;
      std::string getDirectory( std::string const & state ) const;
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
    private:
      jsonPrimitive _config;
  };
  dt__I_addCloneForpVHNotImpl(dtCase);
}
#endif	/* dtCase_H */

