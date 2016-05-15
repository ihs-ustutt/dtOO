#ifndef geomShape_readIgesOCCAGXmlBuilder_H
#define	geomShape_readIgesOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomShape_readIgesOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(geomShape_readIgesOCCAGXmlBuilder);
    dt__classSelfCreate(geomShape_readIgesOCCAGXmlBuilder);
    geomShape_readIgesOCCAGXmlBuilder();
    virtual ~geomShape_readIgesOCCAGXmlBuilder();
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
#endif	/* geomShape_readIgesOCCAGXmlBuilder_H */

