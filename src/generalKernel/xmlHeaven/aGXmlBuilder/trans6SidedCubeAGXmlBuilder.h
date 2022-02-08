#ifndef trans6SidedCubeAGXmlBuilder_H
#define	trans6SidedCubeAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class trans6SidedCubeAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(trans6SidedCubeAGXmlBuilder);
      dt__classSelfCreate(trans6SidedCubeAGXmlBuilder);
      trans6SidedCubeAGXmlBuilder();
      virtual ~trans6SidedCubeAGXmlBuilder();
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
#endif	/* trans6SidedCubeAGXmlBuilder_H */

