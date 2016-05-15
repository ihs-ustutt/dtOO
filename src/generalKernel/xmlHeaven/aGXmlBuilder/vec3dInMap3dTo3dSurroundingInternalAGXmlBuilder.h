#ifndef vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H
#define	vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder 
    : public aGXmlBuilder {
    public:
      dt__classOnlyName(vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder);
      dt__classSelfCreate(vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder);
      vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder();
      virtual ~vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cV,  
        vectorHandling< analyticFunction * > const * const aF,  
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * result 
      ) const;      
    private:

  };
}

#endif	/* vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H */

