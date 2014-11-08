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
      static bVOInterface * create(char const * const str);
      static bVOInterface * create(std::string const str);
  };
}
#endif	/* BVOINTERFACEFACTORY_H */

