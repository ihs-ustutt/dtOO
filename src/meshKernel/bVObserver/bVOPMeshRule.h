#ifndef bVOPMeshRule_H
#define	bVOPMeshRule_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/vectorHandling.h>

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
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      vectorHandling< dtMeshOperator * > _meshOperator;
      std::vector< std::string > _rule1D;
      std::vector< std::string > _rule2D;
      std::vector< std::string > _rule3D;
  };
}
#endif	/* bVOPMeshRule_H */

