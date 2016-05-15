#ifndef aGXmlBuilderFactory_H
#define	aGXmlBuilderFactory_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aGXmlBuilder;
  
  class aGXmlBuilderFactory {
  public:  
    dt__classOnlyName(aGXmlBuilderFactory);
    virtual ~aGXmlBuilderFactory();
    static aGXmlBuilder* create( char const * const str );
    static aGXmlBuilder* create( std::string const str );
    static aGXmlBuilderFactory * instance( void );    
  private:
    vectorHandling< aGXmlBuilder * > _builder;
    static dt__pH(aGXmlBuilderFactory) _instance;    
    aGXmlBuilderFactory();
  };
}
#endif	/* aGXmlBuilderFactory_H */

