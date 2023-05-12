#ifndef bVOSetOrder_H
#define	bVOSetOrder_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetOrder : public bVOInterface {
    public:
      dt__class(bVOSetOrder, bVOInterface);
      dt__classSelfCreate(bVOSetOrder);
      bVOSetOrder();
      virtual ~bVOSetOrder();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      dtInt _order;
      static bool _registrated;
  };
}
#endif	/* bVOSetOrder_H */

