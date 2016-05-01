#ifndef bVOSetRotationalPeriodicity_H
#define	bVOSetRotationalPeriodicity_H

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
    bVOSetRotationalPeriodicity();
    virtual ~bVOSetRotationalPeriodicity();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
  private:
    std::string _faceMaster;
    std::string _faceSlave;
    xYz_rPhiZ const * _dtT;
  };
}
#endif	/* bVOSetRotationalPeriodicity_H */

