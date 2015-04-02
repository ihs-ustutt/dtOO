#ifndef trans6SidedCubeDecorator_H
#define	trans6SidedCubeDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class trans6SidedCubeDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(trans6SidedCubeDecorator);
    trans6SidedCubeDecorator();
    virtual ~trans6SidedCubeDecorator();
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
#endif	/* trans6SidedCubeDecorator_H */

