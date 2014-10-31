#ifndef DTXMLPARSERDECORATOR_H
#define	DTXMLPARSERDECORATOR_H

#include "dtXmlParserDecoratorInterface.h"
#include "dtXmlParserBase.h"
#include <vector>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtXmlParserDecorator : public dtXmlParserDecoratorInterface, public dtXmlParserBase {
  public:
    dt__CLASSNAME(dtXmlParserDecorator);
    dtXmlParserDecorator();
    virtual ~dtXmlParserDecorator();
  };
}

#endif	/* DTXMLPARSERDECORATOR_H */

