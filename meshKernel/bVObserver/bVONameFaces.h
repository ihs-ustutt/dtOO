#ifndef bVONameFaces_H
#define	bVONameFaces_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVONameFaces : public bVOInterface {
  public:
    dt__class(bVONameFaces, bVOInterface);
    bVONameFaces();
    virtual ~bVONameFaces();
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
    std::vector< std::string > _faceLabel;
  };
}
#endif	/* bVONameFaces_H */

