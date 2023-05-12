#ifndef geomSurface_readStlOCCAGXmlBuilder_H
#define	geomSurface_readStlOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const; 
    private: 
      static bool _registrated;          
  };
}
#endif	/* geomSurface_readStlOCCAGXmlBuilder_H */

