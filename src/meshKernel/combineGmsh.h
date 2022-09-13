#ifndef combineGmsh_H
#define	combineGmsh_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class combineGmsh : public gmshBoundedVolume {
    public:
      dt__class(combineGmsh, boundedVolume);
      dt__classSelfCreate(combineGmsh);               
      combineGmsh();
      virtual ~combineGmsh();     
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV
      );
      virtual void makeGrid( void );      
    private:
      lvH_boundedVolume _bV;
      vectorHandling< dtGmshModel const * > _dtGM;
      dtReal _relTol;
      dtReal _absTol;
      static bool _registrated;
  };
}
#endif	/* combineGmsh_H */

