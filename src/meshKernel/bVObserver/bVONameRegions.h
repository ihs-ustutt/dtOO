#ifndef bVONameRegions_H
#define	bVONameRegions_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVONameRegions : public bVOInterface {
    public:
      dt__class(bVONameRegions, bVOInterface);
      dt__classSelfCreate(bVONameRegions);
      bVONameRegions();
      virtual ~bVONameRegions();
      void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );  
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
      using bVOInterface::postUpdate;
    private:
      static bool _registrated;
  };
}
#endif	/* bVONameRegions_H */

