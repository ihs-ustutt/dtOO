#ifndef scaSingleTanhGradingOneDAFXmlBuilder_H
#define	scaSingleTanhGradingOneDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaSingleTanhGradingOneDAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(scaSingleTanhGradingOneDAFXmlBuilder);
    dt__classSelfCreate(scaSingleTanhGradingOneDAFXmlBuilder);
    scaSingleTanhGradingOneDAFXmlBuilder();
    virtual ~scaSingleTanhGradingOneDAFXmlBuilder();
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
#endif	/* scaSingleTanhGradingOneDAFXmlBuilder_H */