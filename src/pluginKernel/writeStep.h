#ifndef WRITESTEP_H
#define	WRITESTEP_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;
  
  class writeStep : public dtPlugin {
  public:
    dt__class(writeStep, dtPlugin);
    dt__classSelfCreate(writeStep);    
    writeStep();
    virtual ~writeStep();
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
    aGPtrVec _aG;
    std::string _fn;
    int _translator;
    static bool _registrated;
  };
}
#endif	/* WRITESTEP_H */

