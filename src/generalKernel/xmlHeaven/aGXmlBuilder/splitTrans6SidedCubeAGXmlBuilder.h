#ifndef splitTrans6SidedCubeAGXmlBuilder_H
#define	splitTrans6SidedCubeAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class splitTrans6SidedCubeAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(splitTrans6SidedCubeAGXmlBuilder);
      dt__classSelfCreate(splitTrans6SidedCubeAGXmlBuilder);
      splitTrans6SidedCubeAGXmlBuilder();
      virtual ~splitTrans6SidedCubeAGXmlBuilder();
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
#endif	/* splitTrans6SidedCubeAGXmlBuilder_H */

