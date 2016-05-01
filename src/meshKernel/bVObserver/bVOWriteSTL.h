#ifndef bVOWriteSTL_H
#define	bVOWriteSTL_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteSTL : public bVOInterface {
  public:
    dt__class(bVOWriteSTL, bVOInterface);
    bVOWriteSTL();
    virtual ~bVOWriteSTL();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    std::string _filename;
  };
}
#endif	/* bVOWriteSTL_H */

