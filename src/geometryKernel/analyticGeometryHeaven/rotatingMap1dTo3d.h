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

#ifndef rotatingMap1dTo3d_H
#define	rotatingMap1dTo3d_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "map2dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map1dTo3d;
  class dtTransformer;
  
  class rotatingMap1dTo3d : public map2dTo3d {
    public:
      dt__class(rotatingMap1dTo3d, analyticGeometry);
      rotatingMap1dTo3d();
      rotatingMap1dTo3d( dtVector3 const & vv, map1dTo3d const * const m1d );
      rotatingMap1dTo3d( const rotatingMap1dTo3d& orig );
      virtual ~rotatingMap1dTo3d();
      virtual rotatingMap1dTo3d * create( void ) const;        
      virtual rotatingMap1dTo3d * clone( void ) const;
      virtual rotatingMap1dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;       
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      virtual dtPoint3 getPoint( dtReal const & uu, dtReal const & vv ) const; 
      virtual std::string dumpToString( void ) const;
      map1dTo3d const & constRefMap1dTo3d( void ) const;
      dtVector3 const & rotationAxis( void ) const;
      dtPoint3 const & origin( void ) const;    
    private:
  	  void correctOrigin( void );
    private:
      ptrHandling< map1dTo3d > _m1d;
      dtVector3 _vv;
      dtPoint3 _pp;
  };
}
#endif	/* rotatingMap1dTo3d_H */
