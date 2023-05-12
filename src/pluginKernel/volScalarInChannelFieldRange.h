#ifndef volScalarInChannelFieldRange_H
#define	volScalarInChannelFieldRange_H

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
  
  class volScalarInChannelFieldRange : public dtPlugin {
    public:
      dt__class(volScalarInChannelFieldRange, dtPlugin);
      dt__classSelfCreate(volScalarInChannelFieldRange);    
      volScalarInChannelFieldRange();
      virtual ~volScalarInChannelFieldRange();
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
      analyticGeometry const * _aG;    
      dtXmlParser const * _parser;
      dtCase const * _case;
      std::string _field;
      dtReal _min;
      dtReal _max;    
      std::vector< dtInt > _nP;
      bool _noRange;
      static bool _registrated;
  };
}

#endif	/* volScalarInChannelFieldRange_H */

