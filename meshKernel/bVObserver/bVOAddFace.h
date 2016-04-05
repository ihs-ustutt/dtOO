#ifndef bVOAddFace_H
#define	bVOAddFace_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  class map1dTo3d;
  
  class bVOAddFace : public bVOInterface {
  public:
    dt__class(bVOAddFace, bVOInterface);
    bVOAddFace();
    virtual ~bVOAddFace();
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
    std::string _regionLabel;
		std::vector< std::string > _faceLabel;
  };
}
#endif	/* bVOAddFace_H */

