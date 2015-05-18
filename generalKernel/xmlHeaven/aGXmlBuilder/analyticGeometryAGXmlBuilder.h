#ifndef analyticGeometryAGXmlBuilder_H
#define	analyticGeometryAGXmlBuilder_H

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
    analyticGeometryAGXmlBuilder();
    virtual ~analyticGeometryAGXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;
  };
}
#endif	/* analyticGeometryAGXmlBuilder_H */

