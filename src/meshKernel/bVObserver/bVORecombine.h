#ifndef bVORecombine_H
#define	bVORecombine_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  
  class bVORecombine : public bVOInterface {
  public:
    dt__class(bVORecombine, bVOInterface);
    dt__classSelfCreate(bVORecombine);
    bVORecombine();
    virtual ~bVORecombine();
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
    std::vector< std::string > _faceLabel;
  };
}
#endif	/* bVORecombine_H */

