#ifndef translatingMap2dTo3dAGXmlBuilder_H
#define	translatingMap2dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class translatingMap2dTo3dAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(translatingMap2dTo3dAGXmlBuilder);
    dt__classSelfCreate(translatingMap2dTo3dAGXmlBuilder);
    translatingMap2dTo3dAGXmlBuilder();
    virtual ~translatingMap2dTo3dAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aGPtrVec const * const aG,
      aGPtrVec * result 
    ) const;
  };
}
#endif	/* translatingMap2dTo3dAGXmlBuilder_H */

