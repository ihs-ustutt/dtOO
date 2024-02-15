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

#ifndef applyVec3dThreeD_H
#define	applyVec3dThreeD_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class vec3dThreeD;
  
  class applyVec3dThreeD : public dtTransformer {
    public:      
      dt__class(applyVec3dThreeD, dtTransformer);
      applyVec3dThreeD();
      applyVec3dThreeD( applyVec3dThreeD const & orig );
      applyVec3dThreeD( vec3dThreeD const * const v3 );
      virtual ~applyVec3dThreeD();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void jInit( 
        jsonPrimitive const & jE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );    
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual std::vector< dtPoint3 > 
      apply( std::vector< dtPoint3 > const * const toTrans ) const;        
      std::vector< dtPoint3 > 
      retract(std::vector< dtPoint3 > const * const toRetract) const;    
    private:
      dt__pH(vec3dThreeD) _v3;
      static bool _registrated;      
  };
}

#endif	/* applyVec3dThreeD_H */
