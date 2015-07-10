#ifndef aGXmlBuilder_H
#define	aGXmlBuilder_H

#include "dtXmlParserBase.h"
#include <vector>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class aGXmlBuilder {
    public:
      dt__classOnlyName(aGXmlBuilder);
      aGXmlBuilder();
      virtual ~aGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,   
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * result 
      ) const = 0;
      virtual void buildPartCompound(
        ::QDomElement const & toBuild,
        baseContainer * const bC,  
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * result 
      ) const;
  };
}

#endif	/* aGXmlBuilder_H */

