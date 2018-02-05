#ifndef dtTransformerDriver_H
#define	dtTransformerDriver_H

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Driver.h>

namespace dtOO {
  class dtTransformer;
  
  class dtTransformerDriver : public ::pugg::Driver {
  public:
    dt__classOnlyName(dtTransformerDriver);       
    dtTransformerDriver(std::string name, int version);
    ~dtTransformerDriver();
    virtual dtTransformer * create( void ) = 0;
  private:

  };
}

#endif	/* dtTransformerDriver_H */

