#ifndef volVectorOnRotatingLineFieldRange_H
#define	volVectorOnRotatingLineFieldRange_H

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
  
  class volVectorOnRotatingLineFieldRange : public dtPlugin {
    public:
      dt__class(volVectorOnRotatingLineFieldRange, dtPlugin);
      dt__classSelfCreate(volVectorOnRotatingLineFieldRange);    
      volVectorOnRotatingLineFieldRange();
      virtual ~volVectorOnRotatingLineFieldRange();
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
      analyticGeometry const * _aG;    
      dtXmlParser const * _parser;
      dtCase const * _case;
      std::string _field;
      float _min;
      float _max;    
      int _nP;
      bool _noRange;
      dtVector3 _rotAxis;
      dtVector3 _refAxis;
      dtPoint3 _origin;
      float _relBandwithR;
      float _relBandwithZ;
      static bool _registrated;
  };
}

#endif	/* volVectorOnRotatingLineFieldRange_H */

