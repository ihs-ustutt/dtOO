#ifndef DTXMLPARSERFUNCTIONDECORATORFACTORY_H
#define	DTXMLPARSERFUNCTIONDECORATORFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtXmlParserFunctionDecorator;
  
  class dtXmlParserFunctionDecoratorFactory {
  public:  
    dt__CLASSNAME(dtXmlParserFunctionDecoratorFactory);
    dtXmlParserFunctionDecoratorFactory();
    virtual ~dtXmlParserFunctionDecoratorFactory();
    static dtXmlParserFunctionDecorator * create( char const * const str );
    static dtXmlParserFunctionDecorator * create( std::string const str );
  private:
  };
}
#endif	/* DTXMLPARSERFUNCTIONDECORATORFACTORY_H */

