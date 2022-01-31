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
      static bVOInterface * create(char const * const str);
      static bVOInterface * create(std::string const str);
      static bVOInterfaceFactory * instance( void );       
    private:
      bVOInterfaceFactory();    
    private:
      vectorHandling< bVOInterface * > _builder;
      static dt__pH(bVOInterfaceFactory) _instance;       
  };
}
#endif	/* BVOINTERFACEFACTORY_H */

