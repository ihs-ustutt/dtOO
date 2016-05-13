#ifndef muParserFunctions_H
#define	muParserFunctions_H

//#include "optionHandling.h"
#include <logMe/dtMacros.h>
#include <muParser.h>

namespace dtOO {
  class muParserFunctions  {
  public:
    dt__classOnlyName(muParserFunctions);
    static muParserFunctions * instance( void );
    virtual ~muParserFunctions();
    static mu::value_type evalAF(mu::char_type const * cc);
  private:
    muParserFunctions();
  private:
    static dt__pH(muParserFunctions) _pH;..
            
  };
}
#endif	/* muParserFunctions_H */

