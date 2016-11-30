#ifndef bVOTransfiniteFaces_H
#define	bVOTransfiniteFaces_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOTransfiniteFaces : public bVOInterface {
  public:
    dt__class(bVOTransfiniteFaces, bVOInterface);
    dt__classSelfCreate(bVOTransfiniteFaces);
    bVOTransfiniteFaces();
    virtual ~bVOTransfiniteFaces();
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
		std::vector< int > _nE;     
  };
}
#endif	/* bVOTransfiniteFaces_H */

