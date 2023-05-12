#ifndef analyticFunctionToCSV_H
#define	analyticFunctionToCSV_H

#include <dtOOTypeDef.h>

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
      dt__classSelfCreate(analyticFunctionToCSV);    
      analyticFunctionToCSV();
      virtual ~analyticFunctionToCSV();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC,      
        lvH_dtPlugin const * const pL
      );    
      virtual void apply(void);
    private:
      vectorHandling< analyticFunction const * > _aF;
      std::vector< dtInt > _nP;
      dtXmlParser const * _parser;
      dtCase const * _case;
      static bool _registrated;
    
  };
}
#endif	/* analyticFunctionToCSV_H */

