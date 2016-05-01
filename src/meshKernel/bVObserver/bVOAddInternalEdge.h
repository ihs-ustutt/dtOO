#ifndef bVOAddInternalEdge_H
#define	bVOAddInternalEdge_H

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
    bVOAddInternalEdge();
    virtual ~bVOAddInternalEdge();
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
    std::string _faceLabel;
		std::vector< map1dTo3d const * > _internalEdge;
  };
}
#endif	/* bVOAddInternalEdge_H */

