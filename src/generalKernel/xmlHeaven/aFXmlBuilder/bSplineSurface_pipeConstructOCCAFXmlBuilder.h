#ifndef bSplineSurface_pipeConstructOCCAFXmlBuilder_H
#define	bSplineSurface_pipeConstructOCCAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_pipeConstructOCCAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineSurface_pipeConstructOCCAFXmlBuilder);   
    dt__classSelfCreate(bSplineSurface_pipeConstructOCCAFXmlBuilder);     
    bSplineSurface_pipeConstructOCCAFXmlBuilder();
    virtual ~bSplineSurface_pipeConstructOCCAFXmlBuilder();
//    virtual std::vector< std::string > factoryAlias( void ) const;     
    virtual void buildPart(
     ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      cVPtrVec const * const cValP, 
      aFPtrVec const * const depSFunP,
      aFPtrVec * sFunP
    ) const;
  };
}
#endif	/* bSplineSurface_pipeConstructOCCAFXmlBuilder_H */

