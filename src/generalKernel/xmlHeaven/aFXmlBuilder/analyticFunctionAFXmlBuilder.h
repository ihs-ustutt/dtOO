#ifndef analyticFunctionAFXmlBuilder_H
#define	analyticFunctionAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class scaFunction;
  class constValue;
  class baseContainer;

  class analyticFunctionAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(analyticFunctionAFXmlBuilder);
      dt__classSelfCreate(analyticFunctionAFXmlBuilder);
      analyticFunctionAFXmlBuilder();
      virtual ~analyticFunctionAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aFPtrVec * result
      ) const;
    private: 
      static bool _registrated;
  };
}
#endif	/* analyticFunctionAFXmlBuilder_H */

