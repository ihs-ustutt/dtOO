#ifndef vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder_H
#define	vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder);
      dt__classSelfCreate(vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder);
      vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder();
      virtual ~vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        cVPtrVec const * const cValP, 
        aFPtrVec const * const depSFunP,
        aFPtrVec * sFunP
      ) const;
    private: 
      static bool _registrated;      
    };
}
#endif	/* vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder_H */