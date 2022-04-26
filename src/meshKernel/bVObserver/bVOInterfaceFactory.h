#ifndef BVOINTERFACEFACTORY_H
#define	BVOINTERFACEFACTORY_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class bVOInterface;
  
  class bVOInterfaceFactory {
    public:    
      dt__classOnlyName(bVOInterfaceFactory);
      virtual ~bVOInterfaceFactory();
      static bool registrate( bVOInterface const * const );           
      static bVOInterface * create(char const * const str);
      static bVOInterface * create(std::string const str);
    private:
      bVOInterfaceFactory();    
    private:
      static dt__pVH(bVOInterface) _observer;
  };
}
#endif	/* BVOINTERFACEFACTORY_H */

