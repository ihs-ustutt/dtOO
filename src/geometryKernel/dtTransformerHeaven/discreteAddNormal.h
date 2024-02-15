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

#ifndef discreteAddNormal_H
#define	discreteAddNormal_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec3dTwoD;
  
  class discreteAddNormal : public dtTransformer {
    public:    
      dt__classOnlyName(discreteAddNormal);
      discreteAddNormal();
      discreteAddNormal(discreteAddNormal const & orig);
      discreteAddNormal( jsonPrimitive const & jE );
      virtual ~discreteAddNormal();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );        
      virtual lvH_analyticFunction 
      apply( lvH_analyticFunction const * const aFP ) const;
    private:
      dt__pH(vec3dTwoD) _tt;
      static bool _registrated;    
  };
}    

#endif	/* discreteAddNormal_H */
