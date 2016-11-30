#ifndef vec3dInMap3dTo3dAGXmlBuilder_H
#define	vec3dInMap3dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec3dInMap3dTo3dAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(vec3dInMap3dTo3dAGXmlBuilder);
      dt__classSelfCreate(vec3dInMap3dTo3dAGXmlBuilder);
      vec3dInMap3dTo3dAGXmlBuilder();
      virtual ~vec3dInMap3dTo3dAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
      virtual void buildPartCompound(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;       
  };
}
#endif	/* vec3dInMap3dTo3dAGXmlBuilder_H */

