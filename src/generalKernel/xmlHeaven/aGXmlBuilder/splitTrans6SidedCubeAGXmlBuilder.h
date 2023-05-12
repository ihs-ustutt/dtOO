#ifndef splitTrans6SidedCubeAGXmlBuilder_H
#define	splitTrans6SidedCubeAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result
      ) const; 
    private: 
      static bool _registrated;            
  };
}
#endif	/* splitTrans6SidedCubeAGXmlBuilder_H */

