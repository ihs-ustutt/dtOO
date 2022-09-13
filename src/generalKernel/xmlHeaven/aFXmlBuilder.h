#ifndef aFXmlBuilder_H
#define	aFXmlBuilder_H

#include <dtOOTypeDef.h>

#include "dtXmlParserBase.h"
#include "aFXmlBuilder.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;  
  class constValue;
  class analyticFunction;

  class aFXmlBuilder {
  public:  
    dt__classOnlyName(aFXmlBuilder);
    aFXmlBuilder();
    ~aFXmlBuilder();
    virtual aFXmlBuilder * create( void ) const = 0;
    virtual void buildPart(
      ::QDomElement const & toBuild, 
      baseContainer * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticFunction * result
    ) const = 0;
    virtual void buildPartCompound(
      ::QDomElement const & toBuild, 
      baseContainer * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticFunction * result
    ) const;     
    virtual std::vector< std::string > factoryAlias( void ) const;
  };
}
#endif	/* aFXmlBuilder_H */

