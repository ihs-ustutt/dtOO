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
      QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result
    ) const; 
  };
}
#endif	/* trans6SidedCubeDecorator_H */

