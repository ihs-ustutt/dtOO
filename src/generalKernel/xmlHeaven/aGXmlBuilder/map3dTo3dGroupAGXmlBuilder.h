#ifndef map3dTo3dGroupAGXmlBuilder_H
#define	map3dTo3dGroupAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class map3dTo3dGroupAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(map3dTo3dGroupAGXmlBuilder);
    dt__classSelfCreate(map3dTo3dGroupAGXmlBuilder);
    map3dTo3dGroupAGXmlBuilder();
    virtual ~map3dTo3dGroupAGXmlBuilder();
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
#endif	/* map3dTo3dGroupAGXmlBuilder_H */

