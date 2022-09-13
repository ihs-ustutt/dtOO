#ifndef volScalarOnLineFieldRange_H
#define	volScalarOnLineFieldRange_H

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
  
  class volScalarOnLineFieldRange : public dtPlugin {
    public:
      dt__class(volScalarOnLineFieldRange, dtPlugin);
      dt__classSelfCreate(volScalarOnLineFieldRange);    
      volScalarOnLineFieldRange();
      virtual ~volScalarOnLineFieldRange();
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
      vectorHandling< analyticGeometry const * > _aG;    
      dtXmlParser const * _parser;
      dtCase const * _case;
      std::string _field;
      dtInt _nP;
      static bool _registrated;
  };
}

#endif	/* volScalarOnLineFieldRange_H */

