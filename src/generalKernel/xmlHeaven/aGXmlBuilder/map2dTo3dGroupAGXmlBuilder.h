#ifndef map2dTo3dGroupAGXmlBuilder_H
#define	map2dTo3dGroupAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class map2dTo3dGroupAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(map2dTo3dGroupAGXmlBuilder);
    dt__classSelfCreate(map2dTo3dGroupAGXmlBuilder);
    map2dTo3dGroupAGXmlBuilder();
    virtual ~map2dTo3dGroupAGXmlBuilder();
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
#endif	/* map2dTo3dGroupAGXmlBuilder_H */

