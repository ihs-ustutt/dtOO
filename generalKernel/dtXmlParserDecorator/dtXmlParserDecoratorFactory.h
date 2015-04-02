#ifndef DTXMLPARSERDECORATORFACTORY_H
#define	DTXMLPARSERDECORATORFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtXmlParserDecorator;
  
  class dtXmlParserDecoratorFactory {
  public:  
    dt__classOnlyName(dtXmlParserDecoratorFactory);
    dtXmlParserDecoratorFactory();
    virtual ~dtXmlParserDecoratorFactory();
    static dtXmlParserDecorator* create( char const * const str );
    static dtXmlParserDecorator* create( std::string const str );
  private:

  };
}
#endif	/* DTXMLPARSERDECORATORFACTORY_H */

