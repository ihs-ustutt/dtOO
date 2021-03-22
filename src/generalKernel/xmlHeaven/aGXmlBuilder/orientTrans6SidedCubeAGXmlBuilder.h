#ifndef orientTrans6SidedCubeAGXmlBuilder_H
#define	orientTrans6SidedCubeAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class orientTrans6SidedCubeAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(orientTrans6SidedCubeAGXmlBuilder);
      dt__classSelfCreate(orientTrans6SidedCubeAGXmlBuilder);
      orientTrans6SidedCubeAGXmlBuilder();
      virtual ~orientTrans6SidedCubeAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result
      ) const; 
    private: 
      static bool _registrated;            
  };
}
#endif	/* orientTrans6SidedCubeAGXmlBuilder_H */

