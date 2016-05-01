#ifndef trans6SidedCubeAGXmlBuilder_H
#define	trans6SidedCubeAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class trans6SidedCubeAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(trans6SidedCubeAGXmlBuilder);
    trans6SidedCubeAGXmlBuilder();
    virtual ~trans6SidedCubeAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result
    ) const; 
  };
}
#endif	/* trans6SidedCubeAGXmlBuilder_H */

