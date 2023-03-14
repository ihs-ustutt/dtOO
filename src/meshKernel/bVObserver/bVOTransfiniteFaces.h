#ifndef bVOTransfiniteFaces_H
#define	bVOTransfiniteFaces_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOTransfiniteFaces : public bVOInterface {
    public:
      dt__class(bVOTransfiniteFaces, bVOInterface);
      dt__classSelfCreate(bVOTransfiniteFaces);
      bVOTransfiniteFaces();
      virtual ~bVOTransfiniteFaces();
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
#endif	/* bVOTransfiniteFaces_H */

