#ifndef bVOSetTranslationalPeriodicity_H
#define	bVOSetTranslationalPeriodicity_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  class translate;
  
  class bVOSetTranslationalPeriodicity : public bVOInterface {
    public:
      dt__class(bVOSetTranslationalPeriodicity, bVOInterface);
      dt__classSelfCreate(bVOSetTranslationalPeriodicity);
      bVOSetTranslationalPeriodicity();
      virtual ~bVOSetTranslationalPeriodicity();
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
      translate const * _dtT;
      static bool _registrated;
  };
}
#endif	/* bVOSetTranslationalPeriodicity_H */

