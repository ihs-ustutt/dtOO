/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef muParserFunctions_H
#define	muParserFunctions_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <muParser.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class muParserFunctions  {
    public:
      dt__classOnlyName(muParserFunctions);
      static muParserFunctions * instance( void );
      virtual ~muParserFunctions();
      void registrateAnalyticFunctions( lvH_analyticFunction const * aFPtrVec );
      static mu::value_type YFix( ::mu::char_type const * expr);
      static mu::value_type Y( ::mu::char_type const * expr, ::mu::value_type val );
      static mu::value_type oneInRange(
        ::mu::value_type low, ::mu::value_type high, ::mu::value_type value
      );
    private:
      muParserFunctions();
    private:
      static dt__pH(muParserFunctions) _pH;
      lvH_analyticFunction const * _aFPtrVec;
            
  };
}
#endif	/* muParserFunctions_H */
