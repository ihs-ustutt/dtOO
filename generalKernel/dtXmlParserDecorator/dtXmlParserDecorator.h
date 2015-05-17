#ifndef DTXMLPARSERDECORATOR_H
#define	DTXMLPARSERDECORATOR_H

#include "dtXmlParserBase.h"
#include <vector>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  
  class dtXmlParserDecorator : public dtXmlParserBase {
  public:
    dt__classOnlyName(dtXmlParserDecorator);
    dtXmlParserDecorator();
    virtual ~dtXmlParserDecorator();
    virtual void buildPart(
      QDomElement const & toBuild,
      baseContainer * const bC,   
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const = 0;
    virtual void buildPartCompound(
      QDomElement const & toBuild,
      baseContainer * const bC,  
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;        
  };
}

#endif	/* DTXMLPARSERDECORATOR_H */

