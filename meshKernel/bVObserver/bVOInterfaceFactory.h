#ifndef BVOINTERFACEFACTORY_H
#define	BVOINTERFACEFACTORY_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class bVOInterface;
  
  class bVOInterfaceFactory {
    public:    
      dt__CLASSNAME(bVOInterfaceFactory);
      bVOInterfaceFactory();
      virtual ~bVOInterfaceFactory();
      bVOInterface * create(char const * const str) const;
      bVOInterface * create(std::string const str) const;
  };
}
#endif	/* BVOINTERFACEFACTORY_H */

