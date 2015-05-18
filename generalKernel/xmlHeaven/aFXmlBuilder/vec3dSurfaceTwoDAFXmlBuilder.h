#ifndef VEC3DSURFACETWODAFXmlBuilder_H
#define	VEC3DSURFACETWODAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec3dSurfaceTwoDAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(vec3dSurfaceTwoDAFXmlBuilder);    
    vec3dSurfaceTwoDAFXmlBuilder();
    virtual ~vec3dSurfaceTwoDAFXmlBuilder();
    virtual void buildPart(
     QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* VEC3DSURFACETWODAFXmlBuilder_H */

