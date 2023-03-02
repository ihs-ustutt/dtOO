#ifndef bVOMeshRule_H
#define	bVOMeshRule_H

#include <dtOOTypeDef.h>

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
      virtual void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      labeledVectorHandling< dtMeshOperator * > _meshOperator;
      static bool _registrated;
  };
}
#endif	/* bVOMeshRule_H */

