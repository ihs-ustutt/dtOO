#ifndef muParserFunctions_H
#define	muParserFunctions_H

#include <logMe/dtMacros.h>
#include <muParser.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class muParserFunctions  {
    public:
      dt__classOnlyName(muParserFunctions);
      static muParserFunctions * instance( void );
      virtual ~muParserFunctions();
      void registrateAnalyticFunctions( aFPtrVec const * aFPtrVec );
      static mu::value_type YFix( ::mu::char_type const * expr);
      static mu::value_type Y( ::mu::char_type const * expr, ::mu::value_type val );
      static mu::value_type oneInRange(
        ::mu::value_type low, ::mu::value_type high, ::mu::value_type value
      );
    private:
      muParserFunctions();
    private:
      static dt__pH(muParserFunctions) _pH;
      aFPtrVec const * _aFPtrVec;
            
  };
}
#endif	/* muParserFunctions_H */

