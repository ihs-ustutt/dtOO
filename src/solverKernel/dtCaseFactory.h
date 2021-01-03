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
      static bool registrate( dtCase const * const );
      static dtCase * create( std::string const str );
    private:
      static dt__pVH(dtCase) _builder;
  };
}

#endif	/* DTCASEFACTORY_H */

