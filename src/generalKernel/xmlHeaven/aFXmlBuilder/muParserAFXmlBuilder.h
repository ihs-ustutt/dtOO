#ifndef muParserAFXmlBuilder_H
#define	muParserAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class muParserAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(muParserAFXmlBuilder);
      dt__classSelfCreate(muParserAFXmlBuilder);
      muParserAFXmlBuilder();
      virtual ~muParserAFXmlBuilder();
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
#endif	/* muParserAFXmlBuilder_H */