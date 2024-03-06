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

#ifndef AVERAGEPOINTS_H
#define	AVERAGEPOINTS_H

#include <dtOOTypeDef.h>

#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class averagePoints : public dtTransformer {
    public:    
      dt__class(averagePoints, dtTransformer);  
      averagePoints();
      averagePoints( averagePoints const & orig );
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;      
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cValP,
        lvH_analyticFunction const * const sFunP,
        lvH_analyticGeometry const * const depAGeoP 
      );    
      virtual ~averagePoints();
      virtual bool isNecessary( void ) const;
      virtual std::vector< dtPoint2 * > apply( 
        std::vector< dtPoint2 * > const * const toTrans 
      ) const;  
    private:
      static bool _registrated;
  };
}    

#endif	/* AVERAGEPOINTS_H */
