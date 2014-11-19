#ifndef VEC3DSURFACETWODFUNCTIONDECORATOR_H
#define	VEC3DSURFACETWODFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec3dSurfaceTwoDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(vec3dSurfaceTwoDFunctionDecorator);    
    vec3dSurfaceTwoDFunctionDecorator();
    virtual ~vec3dSurfaceTwoDFunctionDecorator();
    virtual void buildPart(
     QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* VEC3DSURFACETWODFUNCTIONDECORATOR_H */

