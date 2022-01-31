#ifndef bVOEvilGodfather_H
#define	bVOEvilGodfather_H

#include <dtOOTypeDef.h>

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
      dt__classSelfCreate(bVOEvilGodfather);
      bVOEvilGodfather();
      virtual ~bVOEvilGodfather();
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
      static void perturbElement(
        ::MElement * const jinx, dtReal const & perturbFactor
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
      dtReal _perHex;
      dtReal _perTet;
      dtReal _perturbTet;
      dtReal _perturbHex;
  };
}
#endif	/* bVOEvilGodfather_H */

