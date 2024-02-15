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

#ifndef aFJsonBuilder_H
#define	aFJsonBuilder_H

#include <dtOOTypeDef.h>

#include <mainConceptFwd.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <logMe/dtMacros.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  class baseContainer;  
  class constValue;
  class analyticFunction;

  class aFJsonBuilder {
    public:  
      dt__classOnlyName(aFJsonBuilder);
      aFJsonBuilder();
      ~aFJsonBuilder();
      virtual lvH_analyticFunction buildPart(
        jsonPrimitive const & config, 
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      ) const = 0;
      virtual lvH_analyticFunction buildPart( 
        jsonPrimitive const & config 
      ) const;
  };
}
#endif	/* aFJsonBuilder_H */
