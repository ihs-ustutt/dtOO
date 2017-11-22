#ifndef volVectorPatchFieldRange_H
#define	volVectorPatchFieldRange_H

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
  
  class volVectorPatchFieldRange : public dtPlugin {
  public:
    dt__class(volVectorPatchFieldRange, dtPlugin);
    dt__classSelfCreate(volVectorPatchFieldRange);    
    volVectorPatchFieldRange();
    virtual ~volVectorPatchFieldRange();
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
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::string _field;
    float _min;
    float _max;
    std::string _patchName;
    bool _noRange;
  };
}

#endif	/* volVectorPatchFieldRange_H */

