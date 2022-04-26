#ifndef bVOReadMSH_H
#define	bVOReadMSH_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOReadMSH : public bVOInterface {
    public:
      dt__class(bVOReadMSH, bVOInterface);
      dt__classSelfCreate(bVOReadMSH);
      bVOReadMSH();
      virtual ~bVOReadMSH();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::string _filename;
  //    bool _saveAll;
      bool _mustRead;
      static bool _registrated;      
  };
}
#endif	/* bVOReadMSH_H */

