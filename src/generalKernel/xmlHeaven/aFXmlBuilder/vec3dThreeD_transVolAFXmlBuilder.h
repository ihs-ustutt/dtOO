#ifndef vec3dThreeD_transVolAFXmlBuilder_H
#define	vec3dThreeD_transVolAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dThreeD_transVolAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dThreeD_transVolAFXmlBuilder);    
      dt__classSelfCreate(vec3dThreeD_transVolAFXmlBuilder);
      vec3dThreeD_transVolAFXmlBuilder();
      virtual ~vec3dThreeD_transVolAFXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;    
      virtual void buildPart(
        ::QDomElement const & toBuildP, 
        baseContainer * const bC,
        lvH_constValue const * const cValP, 
        lvH_analyticFunction const * const depSFunP,
        lvH_analyticFunction * sFunP
      ) const;
      virtual void buildPartCompound(
        ::QDomElement const & toBuildP, 
        baseContainer * const bC,
        lvH_constValue const * const cValP, 
        lvH_analyticFunction const * const depSFunP,
        lvH_analyticFunction * sFunP
      ) const;    
    private: 
      static bool _registrated;      
  };
}
#endif	/* vec3dThreeD_transVolAFXmlBuilder_H */

