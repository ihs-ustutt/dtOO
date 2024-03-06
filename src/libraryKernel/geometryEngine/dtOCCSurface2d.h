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

#ifndef DTOCCSURFACE2D_H
#define	DTOCCSURFACE2D_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
#include "dtOCCSurfaceBase.h"
#include "dtSurface2d.h"

class Geom_Surface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCSurface2d : public dtSurface2d {
    public:
      dt__class(dtOCCSurface2d, dtSurface2d);    
      dtOCCSurface2d();
      dtOCCSurface2d(const dtOCCSurfaceBase& orig);
      virtual dtOCCSurface2d * clone( void ) const = 0;
      virtual ~dtOCCSurface2d();
      //
      // overload
      //
      virtual dtReal minPara( dtInt const dim ) const;
      virtual dtReal maxPara( dtInt const dim ) const;
      virtual bool closed( dtInt const dim ) const;
      virtual dtPoint2 point( dtReal const uu, dtReal const vv) const;
      virtual std::vector<dtVector2> firstDer( dtReal const uu, dtReal const vv) const;
      virtual std::vector<dtVector2> secondDer( dtReal const uu, dtReal const vv) const;
      virtual dtPoint2 reparam(dtPoint2 const point) const;
      //
      // optional
      //
  	  virtual std::string dumpToString( void ) const;
      dtOCCSurfaceBase const & OCCRef( void ) const;
      dtOCCSurfaceBase & OCCRef( void );    
    private:
      ptrHandling< dtOCCSurfaceBase > _surface;
      Geom_Surface const * _ptr;
  };
  dt__H_addCloneForpVH(dtOCCSurface2d);
}
#endif	/* DTOCCSURFACE2D_H */
