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

#ifndef MAP1DTO3D_H
#define	MAP1DTO3D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  
  class map1dTo3d : public analyticGeometry {
    public:
      dt__class(map1dTo3d, analyticGeometry);
      map1dTo3d();
      map1dTo3d(const map1dTo3d& orig);
      virtual ~map1dTo3d();
      virtual dtInt dim( void ) const;            
      virtual dtPoint3 getPoint( dtReal const * const uvw ) const;      
      //
      // overload
      //
      virtual map1dTo3d * create( void ) const = 0;
      virtual map1dTo3d * clone( void ) const = 0;
      virtual map1dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const = 0;      
      virtual dtPoint3 getPoint( dtReal const & uu ) const = 0;
      //
      // optional overload
      //
      virtual dtVector3 firstDerU( dtReal const & uu) const;
      virtual map1dTo3d * segment( dtReal const & u0, dtReal const & u1 ) const;      
      virtual dtReal l_u( dtReal const & uu ) const;
      virtual dtReal u_l( dtReal const & ll ) const;
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      //
      //
      //
      dtPoint3 getPointPercent( dtReal const & uu ) const;  
      dtVector3 firstDerUPercent( dtReal const & uP) const;
      map1dTo3d * segmentPercent( dtReal const & u0, dtReal const & u1 ) const;     
      dtInt getRenderResolutionU( void ) const;
      dtReal u_percent(dtReal const & uu) const;
      dtReal u_lPercent(dtReal const & lP) const;
      dtReal percent_u(dtReal const & uu) const;
      dtReal percent_l(dtReal const & ll) const;  
      dtReal lPercent_u(dtReal const & uu) const;
      bool isClosedU( void ) const;
      dtReal getUMin( void ) const;
      dtReal getUMax( void ) const;
      dtReal length( void ) const;
      dtReal operator%( const dtReal &percent ) const;        
      dtReal operator|( const dtReal &length ) const;  
      dtReal operator&( const dtReal &lengthPercent ) const;
    private:
      dtReal l_u( dtReal const & uu, dtInt const & nP ) const;
	    double funValue(const double xx ) const;
    private:
      mutable dtReal _tmpL;
      static dtReal _deltaPer;      
  };  
  
	//
	// boost requires this method in ptr_vector
	//    
  map1dTo3d * new_clone(map1dTo3d const & m1d);  
}
#endif	/* MAP1DTO3D_H */
