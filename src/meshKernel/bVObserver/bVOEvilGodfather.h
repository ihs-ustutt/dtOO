#ifndef bVOEvilGodfather_H
#define	bVOEvilGodfather_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

class GRegion;
class MElement;
class MTetrahedron;
class MHexahedron;
class MPyramid;
class MVertex;

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOEvilGodfather : public bVOInterface {
  public:
    dt__class(bVOEvilGodfather, bVOInterface);
    bVOEvilGodfather();
    virtual ~bVOEvilGodfather();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    static void perturbElement(
      ::MElement * const jinx, float const & perturbFactor
    );
    static void divideTetrahedron(
      ::MTetrahedron * const jinx, 
      std::pair< std::vector< ::MTetrahedron * >, ::GRegion * > & newTets,
      ::MVertex * & newVertex
    );    
    static void divideHexahedron(
      ::MHexahedron * const jinx,
      std::pair< std::vector< ::MPyramid * >, ::GRegion * > & newPyrs,
      ::MVertex * & newVertex     
    );        
  private:
    float _perHex;
    float _perTet;
    float _perturbTet;
    float _perturbHex;
  };
}
#endif	/* bVOEvilGodfather_H */
