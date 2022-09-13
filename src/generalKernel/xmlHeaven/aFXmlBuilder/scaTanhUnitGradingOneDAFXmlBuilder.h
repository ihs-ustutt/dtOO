#ifndef scaTanhUnitGradingOneDAFXmlBuilder_H
#define	scaTanhUnitGradingOneDAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaTanhUnitGradingOneDAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(scaTanhUnitGradingOneDAFXmlBuilder);
      dt__classSelfCreate(scaTanhUnitGradingOneDAFXmlBuilder);
      scaTanhUnitGradingOneDAFXmlBuilder();
      virtual ~scaTanhUnitGradingOneDAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
      virtual void buildPartCompound(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* scaTanhUnitGradingOneDAFXmlBuilder_H */