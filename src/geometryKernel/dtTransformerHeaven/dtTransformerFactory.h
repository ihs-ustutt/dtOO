#ifndef DTTRANSFORMERFACTORY_H
#define	DTTRANSFORMERFACTORY_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtTransformer;

  class dtTransformerFactory {
  public:
    dt__classOnlyName(dtTransformerFactory);
    dtTransformerFactory();
    virtual ~dtTransformerFactory();
    static dtTransformer * create(char const * const str);
    static dtTransformer * create( std::string const str); 
    static dtTransformerFactory * instance( void );         
  private:
    vectorHandling< dtTransformer * > _transformer;
    static dt__pH(dtTransformerFactory) _instance;      

  };
}

#endif	/* DTTRANSFORMERFACTORY_H */

