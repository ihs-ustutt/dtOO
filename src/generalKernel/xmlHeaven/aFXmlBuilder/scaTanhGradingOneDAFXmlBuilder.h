#ifndef scaTanhGradingOneDAFXmlBuilder_H
#define	scaTanhGradingOneDAFXmlBuilder_H

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
#endif	/* scaTanhGradingOneDAFXmlBuilder_H */