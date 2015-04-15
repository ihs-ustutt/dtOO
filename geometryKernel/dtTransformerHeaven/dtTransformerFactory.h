#ifndef DTTRANSFORMERFACTORY_H
#define	DTTRANSFORMERFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;

  class dtTransformerFactory {
  public:
    dt__classOnlyName(dtTransformerFactory);
    dtTransformerFactory();
    virtual ~dtTransformerFactory();
    static dtTransformer* create(char const * const str);
    static dtTransformer* create( std::string const str); 
  private:

  };
}

#endif	/* DTTRANSFORMERFACTORY_H */

