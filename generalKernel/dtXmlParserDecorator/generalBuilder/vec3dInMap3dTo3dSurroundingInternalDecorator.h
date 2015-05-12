#ifndef VEC3DINMAP3DTO3DSURROUNDINGINTERNALDECORATOR_H
#define	VEC3DINMAP3DTO3DSURROUNDINGINTERNALDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class vec3dInMap3dTo3dSurroundingInternalDecorator 
    : public dtXmlParserDecorator {
    public:
      dt__classOnlyName(vec3dInMap3dTo3dSurroundingInternalDecorator);      
      vec3dInMap3dTo3dSurroundingInternalDecorator();
      virtual ~vec3dInMap3dTo3dSurroundingInternalDecorator();
      virtual void buildPart(
        QDomElement ** toBuildP,
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cV,  
        vectorHandling< analyticFunction * > const * const aF,  
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * rAG 
      ) const;      
    private:

  };
}

#endif	/* VEC3DINMAP3DTO3DSURROUNDINGINTERNALDECORATOR_H */

