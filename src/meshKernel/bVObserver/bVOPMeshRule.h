#ifndef bVOPMeshRule_H
#define	bVOPMeshRule_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/labeledVectorHandling.h>

class GEntity;

namespace dtOO {
  class dtMeshOperator;
 
  class bVOPMeshRule : public bVOInterface {
    public:
      dt__class(bVOPMeshRule, bVOInterface);
      dt__classSelfCreate(bVOPMeshRule);
      bVOPMeshRule();
      virtual ~bVOPMeshRule();
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
      labeledVectorHandling< dtMeshOperator * > _meshOperator;
      std::vector< std::string > _rule1D;
      std::vector< std::string > _rule2D;
      std::vector< std::string > _rule3D;
      std::vector< std::string > _only;
      std::vector< float > _faceWeight;
  };
}
#endif	/* bVOPMeshRule_H */

