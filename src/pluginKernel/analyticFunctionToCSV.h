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
      dt__classSelfCreate(analyticFunctionToCSV);    
      analyticFunctionToCSV();
      virtual ~analyticFunctionToCSV();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        dCPtrVec const * const dC,      
        dPPtrVec const * const pL
      );    
      virtual void apply(void);
    private:
      vectorHandling< analyticFunction const * > _aF;
      std::vector< int > _nP;
      dtXmlParser const * _parser;
      dtCase const * _case;
      static bool _registrated;
    
  };
}
#endif	/* analyticFunctionToCSV_H */

