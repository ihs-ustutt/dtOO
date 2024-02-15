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

#ifndef xYz_localCoordinates_H
#define	xYz_localCoordinates_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class xYz_localCoordinates : public dtTransformer {
    public:      
      dt__class(xYz_localCoordinates, dtTransformer);
      xYz_localCoordinates();
      xYz_localCoordinates( xYz_localCoordinates const & orig );
      xYz_localCoordinates( dtPoint3 origin, dtVector3 e1, dtVector3 e2 );
      virtual ~xYz_localCoordinates();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
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
      dtPoint3 _origin;
      dtVector3 _e1;
      dtVector3 _e2;
      dtVector3 _e3;
      static bool _registrated;    
  };
}

#endif	/* xYz_localCoordinates_H */
