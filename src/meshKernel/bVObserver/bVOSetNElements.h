#ifndef bVOSetNElements_H
#define	bVOSetNElements_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetNElements : public bVOInterface {
    public:
      dt__class(bVOSetNElements, bVOInterface);
      dt__classSelfCreate(bVOSetNElements);
      bVOSetNElements();
      virtual ~bVOSetNElements();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< std::string > _faceLabel;
      std::vector< std::string > _regionLabel;
      std::vector< int > _nE;     
  };
}
#endif	/* bVOSetNElements_H */

