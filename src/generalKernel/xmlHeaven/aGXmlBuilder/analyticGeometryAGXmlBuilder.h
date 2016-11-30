#ifndef analyticGeometryAGXmlBuilder_H
#define	analyticGeometryAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class analyticGeometryAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(analyticGeometryAGXmlBuilder);
    dt__classSelfCreate(analyticGeometryAGXmlBuilder);
    analyticGeometryAGXmlBuilder();
    virtual ~analyticGeometryAGXmlBuilder();
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
#endif	/* analyticGeometryAGXmlBuilder_H */

