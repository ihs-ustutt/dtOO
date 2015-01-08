#ifndef WRITETURBOGRID_H
#define	WRITETURBOGRID_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class writeTurboGrid : public dtPlugin {
  public:
    dt__CLASSSTD(writeTurboGrid, dtPlugin);
    writeTurboGrid();
    virtual ~writeTurboGrid();
    virtual void init( 
      QDomElement const & element,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
    dt__pH(analyticGeometry) _hub;
    dt__pH(analyticGeometry) _shroud;
    dt__pH(analyticGeometry) _blade;
    int _nBladeCuts;
    std::string _dir;
  };
}
#endif	/* WRITETURBOGRID_H */

