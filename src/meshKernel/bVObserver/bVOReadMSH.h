#ifndef bVOReadMSH_H
#define	bVOReadMSH_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOReadMSH : public bVOInterface {
  public:
    dt__class(bVOReadMSH, bVOInterface);
    bVOReadMSH();
    virtual ~bVOReadMSH();
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
    std::string _filename;
    bool _saveAll;
  };
}
#endif	/* bVOReadMSH_H */

