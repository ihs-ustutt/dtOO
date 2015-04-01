#ifndef bVOSetGrading_H
#define	bVOSetGrading_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetGrading : public bVOInterface {
  public:
    dt__CLASSNAME(bVOSetGrading);
    bVOSetGrading();
    virtual ~bVOSetGrading();
    virtual void init(
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
  private:
    std::vector< float > _grading;
    std::vector< float > _type;
  };
}
#endif	/* bVOSetGrading_H */

