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

#ifndef TRANS6SIDEDCUBE_H
#define	TRANS6SIDEDCUBE_H

#include <dtOOTypeDef.h>

#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  class transIntCube;
  class dtTransformer;
  
  class trans6SidedCube : public map3dTo3d {
    public:
      dt__class(trans6SidedCube, analyticGeometry);   
      trans6SidedCube();
      trans6SidedCube(const trans6SidedCube& orig);
      trans6SidedCube(
        map2dTo3d const * const aS0, 
        map2dTo3d const * const aS1,
        map2dTo3d const * const aS2, 
        map2dTo3d const * const aS3, 
        map2dTo3d const * const aS4,
        map2dTo3d const * const aS5
      );    
      virtual trans6SidedCube * create( void ) const;       
      virtual trans6SidedCube * clone( void ) const;
      virtual trans6SidedCube * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;     
      virtual ~trans6SidedCube();
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      virtual dtPoint3 getPoint( 
        dtReal const & uu, dtReal const & vv, dtReal const & ww 
      ) const;
      virtual map2dTo3d * segmentConstU( dtReal const & uu ) const;
      virtual map2dTo3d * segmentConstV( dtReal const & vv ) const;
      virtual map2dTo3d * segmentConstW( dtReal const & ww ) const;
      vectorHandling< map2dTo3d const * > const & 
      constRefMap2dTo3d( void ) const;
      void reverseU( void );
      void reverseV( void );
      void reverseW( void );
      void exchangeUV( void );
      void exchangeUW( void );
      void exchangeVW( void );      
    private:
      ptrHandling< transIntCube > _tI;
  };
}
#endif	/* TRANS6SIDEDCUBE_H */
