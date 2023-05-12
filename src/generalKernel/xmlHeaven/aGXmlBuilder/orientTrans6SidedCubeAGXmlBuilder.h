#ifndef orientTrans6SidedCubeAGXmlBuilder_H
#define	orientTrans6SidedCubeAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result
      ) const; 
    private: 
      static bool _registrated;            
  };
}
#endif	/* orientTrans6SidedCubeAGXmlBuilder_H */

