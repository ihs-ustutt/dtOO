#ifndef scaOneD_forceBSplineCurve2dAFXmlBuilder_H
#define	scaOneD_forceBSplineCurve2dAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaOneD_forceBSplineCurve2dAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(scaOneD_forceBSplineCurve2dAFXmlBuilder);    
    dt__classSelfCreate(scaOneD_forceBSplineCurve2dAFXmlBuilder);
    scaOneD_forceBSplineCurve2dAFXmlBuilder();
    virtual ~scaOneD_forceBSplineCurve2dAFXmlBuilder();
    virtual std::vector< std::string > factoryAlias( void ) const;
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      cVPtrVec const * const cValP, 
      aFPtrVec const * const depSFunP,
      aFPtrVec * sFunP
    ) const;
  };
}
#endif	/* scaOneD_forceBSplineCurve2dAFXmlBuilder_H */

