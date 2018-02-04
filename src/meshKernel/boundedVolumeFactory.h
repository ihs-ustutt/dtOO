#ifndef BOUNDEDVOLUMEFACTORY_H
#define	BOUNDEDVOLUMEFACTORY_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class boundedVolume;
  
  class boundedVolumeFactory {
    public:
      dt__classOnlyName(boundedVolumeFactory);
      boundedVolumeFactory();
      virtual ~boundedVolumeFactory();
      static boundedVolume * create(char const * const str);
      static boundedVolume * create( std::string const str );
      static boundedVolumeFactory * instance( void );       
    private:
      vectorHandling< boundedVolume * > _boundedVolume;
      static dt__pH(boundedVolumeFactory) _instance;   
  };
}

#endif	/* BOUNDEDVOLUMEFACTORY_H */

