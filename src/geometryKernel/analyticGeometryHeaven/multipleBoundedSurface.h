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

#ifndef multipleBoundedSurface_H
#define	multipleBoundedSurface_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class multipleBoundedSurface : public analyticGeometry {
  public:
    dt__class(multipleBoundedSurface, analyticGeometry);
    multipleBoundedSurface();
    multipleBoundedSurface(const multipleBoundedSurface& orig);
    virtual ~multipleBoundedSurface();
    multipleBoundedSurface( 
      analyticGeometry const * const m2d, 
      ptrVectorHandling< analyticGeometry > const & m1d
    );
    
    virtual multipleBoundedSurface * create( void ) const;        
    virtual multipleBoundedSurface * clone( void ) const;
    virtual dtInt dim( void ) const;    
    virtual bool isClosed( dtInt const & dir) const;
    virtual dtReal getMin( dtInt const & dir) const;
    virtual dtReal getMax( dtInt const & dir) const;
    virtual dtPoint3 getPoint( dtReal const * const uvw ) const; 
    vectorHandling< renderInterface * > getRender( void ) const;
    ptrVectorHandling< analyticGeometry > const & 
    boundsVectorConstRef( void ) const;
    analyticGeometry const * const surfaceConstPtr( void ) const;
  private:
    ptrHandling< analyticGeometry > _m2d;
    ptrVectorHandling< analyticGeometry > _m1d;
  };
}
#endif	/* multipleBoundedSurface_H */
