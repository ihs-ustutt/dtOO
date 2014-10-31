#ifndef DTXMLPARSERDECORATORFACTORY_H
#define	DTXMLPARSERDECORATORFACTORY_H

#include <string>
using namespace std;
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtXmlParserDecorator;
  
  class dtXmlParserDecoratorFactory {
  public:  
    dt__CLASSNAME(dtXmlParserDecoratorFactory);
    dtXmlParserDecoratorFactory();
    virtual ~dtXmlParserDecoratorFactory();
    static dtXmlParserDecorator* create(char const * const str);
    static dtXmlParserDecorator* create(string const str);
  private:

  };
}
#endif	/* DTXMLPARSERDECORATORFACTORY_H */

