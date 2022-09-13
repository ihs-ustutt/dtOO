#ifndef makeCompoundAGXmlBuilder_H
#define	makeCompoundAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class makeCompoundAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(makeCompoundAGXmlBuilder);
      dt__classSelfCreate(makeCompoundAGXmlBuilder);
      makeCompoundAGXmlBuilder();
      virtual ~makeCompoundAGXmlBuilder();
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
#endif	/* makeCompoundAGXmlBuilder_H */

