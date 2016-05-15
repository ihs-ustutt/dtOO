#ifndef aFXmlBuilderFactory_H
#define	aFXmlBuilderFactory_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aFXmlBuilder;
  
  class aFXmlBuilderFactory {
  public:  
    dt__classOnlyName(aFXmlBuilderFactory);
    virtual ~aFXmlBuilderFactory();
    static aFXmlBuilder * create( char const * const str );
    static aFXmlBuilder * create( std::string const str );
    static aFXmlBuilderFactory * instance( void );
  private:
    vectorHandling< aFXmlBuilder * > _builder;
    static dt__pH(aFXmlBuilderFactory) _instance;
    aFXmlBuilderFactory();
  };
}
#endif	/* aFXmlBuilderFactory_H */

