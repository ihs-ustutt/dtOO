#ifndef muParserFunctions_H
#define	muParserFunctions_H

#include <logMe/dtMacros.h>
#include <muParser.h>

namespace dtOO {
  class muParserFunctions  {
  public:
    dt__classOnlyName(muParserFunctions);
    static muParserFunctions * instance( void );
    virtual ~muParserFunctions();
    static mu::value_type oneInRange(
      mu::value_type low, mu::value_type high, mu::value_type value
    );
  private:
    muParserFunctions();
  private:
    static dt__pH(muParserFunctions) _pH;
            
  };
}
#endif	/* muParserFunctions_H */

