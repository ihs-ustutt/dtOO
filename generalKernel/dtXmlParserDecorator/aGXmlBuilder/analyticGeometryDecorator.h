#ifndef ANALYTICGEOMETRYDECORATOR_H
#define	ANALYTICGEOMETRYDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class analyticGeometryDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(analyticGeometryDecorator);
    analyticGeometryDecorator();
    virtual ~analyticGeometryDecorator();
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
#endif	/* ANALYTICGEOMETRYDECORATOR_H */

