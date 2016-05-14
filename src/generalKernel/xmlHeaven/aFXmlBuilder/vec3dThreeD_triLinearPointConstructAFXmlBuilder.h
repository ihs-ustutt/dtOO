#ifndef vec3dThreeD_triLinearPointConstructAFXmlBuilder_H
#define	vec3dThreeD_triLinearPointConstructAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dThreeD_triLinearPointConstructAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dThreeD_triLinearPointConstructAFXmlBuilder);
      dt__classSelfCreate(vec3dThreeD_triLinearPointConstructAFXmlBuilder);
      vec3dThreeD_triLinearPointConstructAFXmlBuilder();
      virtual ~vec3dThreeD_triLinearPointConstructAFXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cValP, 
        vectorHandling< analyticFunction * > const * const depSFunP,
        vectorHandling< analyticFunction * > * sFunP
      ) const;
    };
}
#endif	/* vec3dThreeD_triLinearPointConstructAFXmlBuilder_H */