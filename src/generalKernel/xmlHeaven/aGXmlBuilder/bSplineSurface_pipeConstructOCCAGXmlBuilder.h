#ifndef bSplineSurface_pipeConstructOCCAGXmlBuilder_H
#define	bSplineSurface_pipeConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_pipeConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_pipeConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineSurface_pipeConstructOCCAGXmlBuilder);
      bSplineSurface_pipeConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_pipeConstructOCCAGXmlBuilder();
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
#endif	/* bSplineSurface_pipeConstructOCCAGXmlBuilder_H */

