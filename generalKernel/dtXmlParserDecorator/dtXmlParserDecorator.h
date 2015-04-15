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
      QDomElement ** toBuildP,
      baseContainer * const bC,   
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const = 0;
    virtual void buildPartCompound(
      QDomElement ** toBuildP,
      baseContainer * const bC,  
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;        
  };
}

#endif	/* DTXMLPARSERDECORATOR_H */

