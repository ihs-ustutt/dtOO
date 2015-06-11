#ifndef bVOWriteMSH_H
#define	bVOWriteMSH_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteMSH : public bVOInterface {
  public:
    dt__class(bVOWriteMSH, bVOInterface);
    bVOWriteMSH();
    virtual ~bVOWriteMSH();
    virtual void init(
      QDomElement const & element,
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
    bool _saveAll;
  };
}
#endif	/* bVOWriteMSH_H */

