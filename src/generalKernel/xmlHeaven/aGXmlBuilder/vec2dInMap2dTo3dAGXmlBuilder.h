#ifndef vec2dInMap2dTo3dAGXmlBuilder_H
#define	vec2dInMap2dTo3dAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec2dInMap2dTo3dAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(vec2dInMap2dTo3dAGXmlBuilder);
      dt__classSelfCreate(vec2dInMap2dTo3dAGXmlBuilder);
      vec2dInMap2dTo3dAGXmlBuilder();
      virtual ~vec2dInMap2dTo3dAGXmlBuilder();
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
#endif	/* vec2dInMap2dTo3dAGXmlBuilder_H */

