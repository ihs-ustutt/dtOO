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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      static bool _registrated;      
  };
}
#endif	/* bVOAddInternalEdge_H */

