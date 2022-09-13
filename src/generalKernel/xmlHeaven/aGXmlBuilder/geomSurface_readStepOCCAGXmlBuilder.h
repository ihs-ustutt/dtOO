#ifndef geomSurface_readStepOCCAGXmlBuilder_H
#define	geomSurface_readStepOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
      dt__classSelfCreate(geomSurface_readStepOCCAGXmlBuilder);
      geomSurface_readStepOCCAGXmlBuilder();
      virtual ~geomSurface_readStepOCCAGXmlBuilder();
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
#endif	/* geomSurface_readStepOCCAGXmlBuilder_H */

