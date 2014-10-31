#ifndef DTTRANSFORMERFACTORY_H
#define	DTTRANSFORMERFACTORY_H

#include <string>
using namespace std;
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;

  class dtTransformerFactory {
  public:
    dt__CLASSNAME(dtTransformerFactory);
    dtTransformerFactory();
    virtual ~dtTransformerFactory();
    dtTransformer* create(char const * const str) const;
    dtTransformer* create(string const str) const; 
  private:

  };
}

#endif	/* DTTRANSFORMERFACTORY_H */

