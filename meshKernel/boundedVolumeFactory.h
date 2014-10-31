#ifndef BOUNDEDVOLUMEFACTORY_H
#define	BOUNDEDVOLUMEFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class boundedVolume;
  
  class boundedVolumeFactory {
  public:
    dt__CLASSNAME(boundedVolumeFactory);
    boundedVolumeFactory();
    virtual ~boundedVolumeFactory();
    static boundedVolume * create(char const * const str);
    static boundedVolume * create( std::string const str );
  private:

  };
}

#endif	/* BOUNDEDVOLUMEFACTORY_H */

