#ifndef geomSurface_readStlOCCAGXmlBuilder_H
#define	geomSurface_readStlOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomSurface_readStlOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(geomSurface_readStlOCCAGXmlBuilder);
    dt__classSelfCreate(geomSurface_readStlOCCAGXmlBuilder);
    geomSurface_readStlOCCAGXmlBuilder();
    virtual ~geomSurface_readStlOCCAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aGPtrVec const * const aG,
      aGPtrVec * result 
    ) const; 
  };
}
#endif	/* geomSurface_readStlOCCAGXmlBuilder_H */

