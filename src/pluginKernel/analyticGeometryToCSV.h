#ifndef analyticGeometryToCSV_H
#define	analyticGeometryToCSV_H

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
  
  class analyticGeometryToCSV : public dtPlugin {
    public:
      dt__class(analyticGeometryToCSV, dtPlugin);
      dt__classSelfCreate(analyticGeometryToCSV);    
      analyticGeometryToCSV();
      virtual ~analyticGeometryToCSV();
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
      vectorHandling< analyticGeometry const * > _aG;
      std::vector< int > _nP;
      dtXmlParser const * _parser;
      dtCase const * _case;      
  };
}
#endif	/* analyticGeometryToCSV_H */

