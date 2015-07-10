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
    vec3dInMap3dTo3dAGXmlBuilder();
    virtual ~vec3dInMap3dTo3dAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;
    virtual void buildPartCompound(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;       
  };
}
#endif	/* vec3dInMap3dTo3dAGXmlBuilder_H */

