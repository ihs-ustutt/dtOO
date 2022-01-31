#ifndef map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H
#define	map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class map2dTo3dApproximateInMap3dTo3dAGXmlBuilder 
    : public aGXmlBuilder {
    public:
      dt__classOnlyName(map2dTo3dApproximateInMap3dTo3dAGXmlBuilder);      
      dt__classSelfCreate(map2dTo3dApproximateInMap3dTo3dAGXmlBuilder);      
      map2dTo3dApproximateInMap3dTo3dAGXmlBuilder();
      virtual ~map2dTo3dApproximateInMap3dTo3dAGXmlBuilder();
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

#endif	/* map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H */

