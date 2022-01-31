#ifndef aFXmlBuilderFactory_H
#define	aFXmlBuilderFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aFXmlBuilder;
  
  class aFXmlBuilderFactory {
  public:  
    dt__classOnlyName(aFXmlBuilderFactory);
    virtual ~aFXmlBuilderFactory();
    static bool registrate( aFXmlBuilder const * const );    
    static aFXmlBuilder * create( char const * const str );
    static aFXmlBuilder * create( std::string const str );
  private:
    aFXmlBuilderFactory();
  private:
    static dt__pVH(aFXmlBuilder) _builder;
  };
}
#endif	/* aFXmlBuilderFactory_H */

