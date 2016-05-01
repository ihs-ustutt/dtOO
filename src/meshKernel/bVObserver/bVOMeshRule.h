#ifndef bVOMeshRule_H
#define	bVOMeshRule_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtMeshOperator;
 
  class bVOMeshRule : public bVOInterface {
    public:
      dt__class(bVOMeshRule, bVOInterface);
      bVOMeshRule();
      virtual ~bVOMeshRule();
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
      std::vector< std::string > _only;
  };
}
#endif	/* bVOMeshRule_H */

