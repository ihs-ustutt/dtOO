#ifndef analyticGeometryAGXmlBuilder_H
#define	analyticGeometryAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class analyticGeometryAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(analyticGeometryAGXmlBuilder);
      dt__classSelfCreate(analyticGeometryAGXmlBuilder);
      analyticGeometryAGXmlBuilder();
      virtual ~analyticGeometryAGXmlBuilder();
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
#endif	/* analyticGeometryAGXmlBuilder_H */

