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

#ifndef DTOCCBEZIERSURFACE_H
#define	DTOCCBEZIERSURFACE_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOCCSurface.h"

class Geom_BezierSurface;

namespace dtOO {
  class dtOCCSurfaceBase;
  
  class dtOCCBezierSurface : public dtOCCSurface {
  public:
    dt__class(dtOCCBezierSurface, dtSurface);
    dtOCCBezierSurface();
    dtOCCBezierSurface(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCBezierSurface();
    virtual dtOCCBezierSurface * clone( void ) const;
	  virtual dtCurve * segmentConstU( dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
	  virtual dtCurve * segmentConstV( dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;     
    virtual dtPoint3 controlPoint( dtInt const uI, dtInt const vI ) const;
    virtual void setControlPoint( dtInt const uI, dtInt const vI, dtPoint3 const point );    
    virtual dtInt nControlPoints( dtInt const dim ) const;   
  private:
    Geom_BezierSurface const * _ptr;
  };
}
#endif	/* DTOCCBEZIERSURFACE_H */
