#ifndef DTPLUGIN_H
#define	DTPLUGIN_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class dtPlugin : public labelHandling,
                   public optionHandling,
                   public dtXmlParserBase {
  public:
    dt__CLASSNAME(dtPlugin);
    dtPlugin();
    virtual ~dtPlugin();
    virtual void init( 
      QDomElement const & element,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtPlugin * > const * const pL
    );
    virtual void apply( void ) = 0;    
  private:
  };
}
#endif	/* DTPLUGIN_H */

