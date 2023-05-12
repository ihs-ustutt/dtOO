#ifndef bVOWriteSTL_H
#define	bVOWriteSTL_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteSTL : public bVOInterface {
    public:
      dt__class(bVOWriteSTL, bVOInterface);
      dt__classSelfCreate(bVOWriteSTL);
      bVOWriteSTL();
      virtual ~bVOWriteSTL();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      std::string _filename;
      static bool _registrated;      
  };
}
#endif	/* bVOWriteSTL_H */

