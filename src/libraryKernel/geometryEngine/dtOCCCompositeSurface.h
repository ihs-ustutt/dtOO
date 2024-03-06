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

#ifndef dtOCCCompositeSurface_H
#define	dtOCCCompositeSurface_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class ShapeExtend_CompositeSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCCompositeSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCCompositeSurface, dtSurface);
    dtOCCCompositeSurface();
    dtOCCCompositeSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCCompositeSurface();
    virtual dtOCCCompositeSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( 
      dtReal const uu, dtReal const vvMin, dtReal const vvMax
    ) const;
	  virtual dtCurve * segmentConstV( 
      dtReal const vv, dtReal const uuMin, dtReal const uuMax
    ) const;     
  private:
    ShapeExtend_CompositeSurface const * _ptr;
  };
}
#endif	/* dtOCCCompositeSurface_H */
