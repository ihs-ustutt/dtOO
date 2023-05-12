#ifndef scaTanhGradingOneDAFXmlBuilder_H
#define	scaTanhGradingOneDAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaTanhGradingOneDAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(scaTanhGradingOneDAFXmlBuilder);
      dt__classSelfCreate(scaTanhGradingOneDAFXmlBuilder);
      scaTanhGradingOneDAFXmlBuilder();
      virtual ~scaTanhGradingOneDAFXmlBuilder();
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
#endif	/* scaTanhGradingOneDAFXmlBuilder_H */