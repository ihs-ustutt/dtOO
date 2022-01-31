#ifndef aGXmlBuilderFactory_H
#define	aGXmlBuilderFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aGXmlBuilder;
  
  class aGXmlBuilderFactory {
  public:  
    dt__classOnlyName(aGXmlBuilderFactory);
    virtual ~aGXmlBuilderFactory();
    static bool registrate( aGXmlBuilder const * const );        
    static aGXmlBuilder* create( char const * const str );
    static aGXmlBuilder* create( std::string const str );
    static aGXmlBuilderFactory * instance( void );    
  private:
    aGXmlBuilderFactory();
  private:
    static dt__pVH(aGXmlBuilder) _builder;    
  };
}
#endif	/* aGXmlBuilderFactory_H */

