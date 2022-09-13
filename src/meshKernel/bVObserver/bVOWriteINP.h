#ifndef bVOWriteINP_H
#define	bVOWriteINP_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteINP : public bVOInterface {
    public:
      dt__class(bVOWriteINP, bVOInterface);
      dt__classSelfCreate(bVOWriteINP);
      bVOWriteINP();
      virtual ~bVOWriteINP();
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
      bool _saveAll;
      bool _saveGroupOfNodes;
      static bool _registrated;
  };
}
#endif	/* bVOWriteINP_H */

