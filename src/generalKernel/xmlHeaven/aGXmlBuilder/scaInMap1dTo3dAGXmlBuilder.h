#ifndef scaInMap1dTo3dAGXmlBuilder_H
#define	scaInMap1dTo3dAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaInMap1dTo3dAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(scaInMap1dTo3dAGXmlBuilder);
      dt__classSelfCreate(scaInMap1dTo3dAGXmlBuilder);
      scaInMap1dTo3dAGXmlBuilder();
      virtual ~scaInMap1dTo3dAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;          
  };
}
#endif	/* scaInMap1dTo3dAGXmlBuilder_H */

