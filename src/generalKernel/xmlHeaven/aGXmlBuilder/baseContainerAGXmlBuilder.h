#ifndef baseContainerAGXmlBuilder_H
#define	baseContainerAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include "baseContainerAGXmlBuilder.h"
#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class vectorContainer;

  class baseContainerAGXmlBuilder : public aGXmlBuilder {
    public:
      dt__classOnlyName(baseContainerAGXmlBuilder);    
      dt__classSelfCreate(baseContainerAGXmlBuilder);    
      baseContainerAGXmlBuilder();
      virtual ~baseContainerAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;  
    private: 
      static bool _registrated;            
  };
}
#endif	/* baseContainerAGXmlBuilder_H */

