#ifndef analyticFunctionToCSV_H
#define	analyticFunctionToCSV_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class dtXmlParser;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;
  
  class analyticFunctionToCSV : public dtPlugin {
  public:
    dt__class(analyticFunctionToCSV, dtPlugin);
    analyticFunctionToCSV();
    virtual ~analyticFunctionToCSV();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC,      
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
		vectorHandling< analyticFunction const * > _aF;
    std::vector< int > _nP;
    dtXmlParser const * _parser;
    dtCase const * _case;
    
  };
}
#endif	/* analyticFunctionToCSV_H */

