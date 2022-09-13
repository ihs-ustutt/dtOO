#ifndef bVOSetGrading_H
#define	bVOSetGrading_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< std::string > _regionLabel;    
      std::vector< std::string > _faceLabel;
      std::vector< std::string > _edgeLabel;
      std::vector< dtReal > _grading;
      std::vector< dtReal > _type;
      static bool _registrated;
  };
}
#endif	/* bVOSetGrading_H */

