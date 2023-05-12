#ifndef radialTranslatingMap2dTo3dAGXmlBuilder_H
#define	radialTranslatingMap2dTo3dAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class radialTranslatingMap2dTo3dAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(radialTranslatingMap2dTo3dAGXmlBuilder);
      dt__classSelfCreate(radialTranslatingMap2dTo3dAGXmlBuilder);
      radialTranslatingMap2dTo3dAGXmlBuilder();
      virtual ~radialTranslatingMap2dTo3dAGXmlBuilder();
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
#endif	/* radialTranslatingMap2dTo3dAGXmlBuilder_H */

