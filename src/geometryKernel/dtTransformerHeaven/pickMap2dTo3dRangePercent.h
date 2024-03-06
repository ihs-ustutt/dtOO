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

#ifndef PICKMAP2DTO3DRANGEPERCENT_H
#define	PICKMAP2DTO3DRANGEPERCENT_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class pickMap2dTo3dRangePercent : public dtTransformer {
    public:
      dt__classOnlyName(pickMap2dTo3dRangePercent);   
      pickMap2dTo3dRangePercent();
      pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig);
      pickMap2dTo3dRangePercent( jsonPrimitive const & jE );
      virtual ~pickMap2dTo3dRangePercent();
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
      virtual lvH_analyticGeometry 
      apply( lvH_analyticGeometry const * const aGeoVecP ) const;
    private:
      static bool _registrated;    
  };
}

#endif	/* PICKMAP2DTO3DRANGEPERCENT_H */
