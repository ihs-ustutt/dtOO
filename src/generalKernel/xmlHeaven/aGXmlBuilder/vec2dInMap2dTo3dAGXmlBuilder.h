#ifndef vec2dInMap2dTo3dAGXmlBuilder_H
#define	vec2dInMap2dTo3dAGXmlBuilder_H

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
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* vec2dInMap2dTo3dAGXmlBuilder_H */

