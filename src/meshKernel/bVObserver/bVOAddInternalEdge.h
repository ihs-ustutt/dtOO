#ifndef bVOAddInternalEdge_H
#define	bVOAddInternalEdge_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  class map1dTo3d;
  
  class bVOAddInternalEdge : public bVOInterface {
    public:
      dt__class(bVOAddInternalEdge, bVOInterface);
      dt__classSelfCreate(bVOAddInternalEdge);
      bVOAddInternalEdge();
      virtual ~bVOAddInternalEdge();
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
      std::string _regionLabel;
      static bool _registrated;      
  };
}
#endif	/* bVOAddInternalEdge_H */

