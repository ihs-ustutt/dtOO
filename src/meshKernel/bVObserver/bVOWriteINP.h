#ifndef bVOWriteINP_H
#define	bVOWriteINP_H

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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    std::string _filename;
    bool _saveAll;
    bool _saveGroupOfNodes;
  };
}
#endif	/* bVOWriteINP_H */

