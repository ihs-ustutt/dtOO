#ifndef bVOMeshRule_H
#define	bVOMeshRule_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  class dtMeshOperator;
 
  class bVOMeshRule : public bVOInterface {
    public:
      dt__class(bVOMeshRule, bVOInterface);
      dt__classSelfCreate(bVOMeshRule);
      bVOMeshRule();
      virtual ~bVOMeshRule();
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
  };
}
#endif	/* bVOMeshRule_H */

