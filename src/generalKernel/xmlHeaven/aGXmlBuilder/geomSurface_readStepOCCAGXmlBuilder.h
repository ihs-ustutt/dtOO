#ifndef geomSurface_readStepOCCAGXmlBuilder_H
#define	geomSurface_readStepOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomSurface_readStepOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(geomSurface_readStepOCCAGXmlBuilder);
    geomSurface_readStepOCCAGXmlBuilder();
    virtual ~geomSurface_readStepOCCAGXmlBuilder();
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
#endif	/* geomSurface_readStepOCCAGXmlBuilder_H */

