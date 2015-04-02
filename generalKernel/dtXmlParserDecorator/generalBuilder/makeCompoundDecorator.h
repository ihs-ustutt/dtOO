#ifndef makeCompoundDecorator_H
#define	makeCompoundDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class makeCompoundDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(makeCompoundDecorator);
    makeCompoundDecorator();
    virtual ~makeCompoundDecorator();
    virtual void buildPart(
      QDomElement ** toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,  
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;       
  };
}
#endif	/* makeCompoundDecorator_H */

