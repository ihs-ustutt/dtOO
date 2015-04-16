#ifndef bVOSetSimpleGrading_H
#define	bVOSetSimpleGrading_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetSimpleGrading : public bVOInterface {
  public:
    dt__class(bVOSetSimpleGrading, bVOInterface);
    bVOSetSimpleGrading();
    virtual ~bVOSetSimpleGrading();
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
#endif	/* bVOSetSimpleGrading_H */

