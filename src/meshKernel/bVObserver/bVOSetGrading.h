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
    dt__class(bVOSetGrading, bVOInterface);
    dt__classSelfCreate(bVOSetGrading);
    bVOSetGrading();
    virtual ~bVOSetGrading();
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
    std::vector< std::string > _regionLabel;    
    std::vector< std::string > _faceLabel;
    std::vector< std::string > _edgeLabel;
    std::vector< float > _grading;
    std::vector< float > _type;
  };
}
#endif	/* bVOSetGrading_H */

