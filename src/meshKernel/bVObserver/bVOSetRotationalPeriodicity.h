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
    dt__classSelfCreate(bVOSetRotationalPeriodicity);
    bVOSetRotationalPeriodicity();
    virtual ~bVOSetRotationalPeriodicity();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
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

