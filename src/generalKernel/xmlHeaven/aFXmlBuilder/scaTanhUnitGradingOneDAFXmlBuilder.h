#ifndef scaTanhUnitGradingOneDAFXmlBuilder_H
#define	scaTanhUnitGradingOneDAFXmlBuilder_H

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
      cVPtrVec const * const cV, 
      aFPtrVec const * const aF,
      aFPtrVec * result
    ) const;
    virtual void buildPartCompound(
      ::QDomElement const & toBuild, 
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aFPtrVec * result
    ) const;
  };
}
#endif	/* scaTanhUnitGradingOneDAFXmlBuilder_H */