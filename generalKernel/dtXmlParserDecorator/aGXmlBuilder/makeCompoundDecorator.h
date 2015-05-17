#ifndef makeCompoundDecorator_H
#define	makeCompoundDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class makeCompoundDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(makeCompoundDecorator);
    makeCompoundDecorator();
    virtual ~makeCompoundDecorator();
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
#endif	/* makeCompoundDecorator_H */

