#ifndef bVOSetGradingToFaceRule_H
#define	bVOSetGradingToFaceRule_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetGradingToFaceRule : public bVOInterface {
    public:
      dt__class(bVOSetGradingToFaceRule, bVOInterface);
      dt__classSelfCreate(bVOSetGradingToFaceRule);
      bVOSetGradingToFaceRule();
      virtual ~bVOSetGradingToFaceRule();
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
      std::vector< std::string > _rule;
      static bool _registrated;
  };
}
#endif	/* bVOSetGradingToFaceRule_H */

