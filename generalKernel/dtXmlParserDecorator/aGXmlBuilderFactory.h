#ifndef aGXmlBuilderFactory_H
#define	aGXmlBuilderFactory_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class aGXmlBuilder;
  
  class aGXmlBuilderFactory {
  public:  
    dt__classOnlyName(aGXmlBuilderFactory);
    aGXmlBuilderFactory();
    virtual ~aGXmlBuilderFactory();
    static aGXmlBuilder* create( char const * const str );
    static aGXmlBuilder* create( std::string const str );
  private:

  };
}
#endif	/* aGXmlBuilderFactory_H */

