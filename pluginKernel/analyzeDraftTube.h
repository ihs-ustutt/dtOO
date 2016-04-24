#ifndef analyzeDraftTube_H
#define	analyzeDraftTube_H

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
  
  class analyzeDraftTube : public dtPlugin {
  public:
    dt__class(analyzeDraftTube, dtPlugin);
    analyzeDraftTube();
    virtual ~analyzeDraftTube();
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
    dtXmlParser const * _parser;
    dtCase const * _case;
    int _nBins;
    float _max;
    std::string _wallName;
    std::string _inletName;
    std::string _outletName;
    std::vector< double > _quantileWallShear;
  };
}

#endif	/* analyzeDraftTube_H */

