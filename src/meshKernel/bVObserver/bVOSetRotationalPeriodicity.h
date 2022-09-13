#ifndef bVOSetRotationalPeriodicity_H
#define	bVOSetRotationalPeriodicity_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  class xYz_rPhiZ;
  
  class bVOSetRotationalPeriodicity : public bVOInterface {
    public:
      dt__class(bVOSetRotationalPeriodicity, bVOInterface);
      dt__classSelfCreate(bVOSetRotationalPeriodicity);
      bVOSetRotationalPeriodicity();
      virtual ~bVOSetRotationalPeriodicity();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::string _faceMaster;
      std::string _faceSlave;
      xYz_rPhiZ const * _dtT;
      static bool _registrated;
  };
}
#endif	/* bVOSetRotationalPeriodicity_H */

