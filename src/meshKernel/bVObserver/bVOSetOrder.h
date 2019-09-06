#ifndef bVOSetOrder_H
#define	bVOSetOrder_H

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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    int _order;
  };
}
#endif	/* bVOSetOrder_H */

