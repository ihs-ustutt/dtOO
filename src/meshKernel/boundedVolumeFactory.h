#ifndef BOUNDEDVOLUMEFACTORY_H
#define	BOUNDEDVOLUMEFACTORY_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class boundedVolume;
  
  class boundedVolumeFactory {
    public:
      dt__classOnlyName(boundedVolumeFactory);
      virtual ~boundedVolumeFactory();
      static bool registrate( boundedVolume const * const );
      static boundedVolume * create(char const * const str);
      static boundedVolume * create( std::string const str );
    private:
      boundedVolumeFactory();
    private:
      static dt__pVH(boundedVolume) _product;
  };
}

#endif	/* BOUNDEDVOLUMEFACTORY_H */

