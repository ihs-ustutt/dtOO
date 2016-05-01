#ifndef aFXmlBuilderFactory_H
#define	aFXmlBuilderFactory_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class aFXmlBuilder;
  
  class aFXmlBuilderFactory {
  public:  
    dt__classOnlyName(aFXmlBuilderFactory);
    aFXmlBuilderFactory();
    virtual ~aFXmlBuilderFactory();
    static aFXmlBuilder * create( char const * const str );
    static aFXmlBuilder * create( std::string const str );
  private:
  };
}
#endif	/* aFXmlBuilderFactory_H */

