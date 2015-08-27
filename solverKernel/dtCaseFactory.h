#ifndef DTCASEFACTORY_H
#define	DTCASEFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCase;
  
  class dtCaseFactory {
  public:
    dt__classOnlyName(dtCaseFactory);
    dtCaseFactory();
    virtual ~dtCaseFactory();
//    static dtCase * create(char const * const str);
    static dtCase * create( std::string const str );
  private:

  };
}

#endif	/* DTCASEFACTORY_H */

