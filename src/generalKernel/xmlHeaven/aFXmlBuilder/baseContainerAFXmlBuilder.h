#ifndef baseContainerAFXmlBuilder_H
#define	baseContainerAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include "baseContainerAFXmlBuilder.h"
#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class baseContainerAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(baseContainerAFXmlBuilder);
      dt__classSelfCreate(baseContainerAFXmlBuilder);    
      baseContainerAFXmlBuilder();
      virtual ~baseContainerAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        cVPtrVec const * const cValP,  
        aFPtrVec const * const depSFunP,
        aFPtrVec * sFunP
      ) const;  
    private: 
      static bool _registrated;      
  };
}
#endif	/* baseContainerAFXmlBuilder_H */

