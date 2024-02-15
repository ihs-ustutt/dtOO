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

#ifndef DTOCCBSPLINECURVE2D_H
#define	DTOCCBSPLINECURVE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_BSplineCurve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCBSplineCurve2d : public dtOCCCurve2d {
  public:
    dt__class(dtOCCBSplineCurve2d, dtCurve2d);
    dtOCCBSplineCurve2d();
    dtOCCBSplineCurve2d( dtOCCCurve2dBase const & orig);
    virtual ~dtOCCBSplineCurve2d();
    virtual dtOCCBSplineCurve2d * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint2 const point );    
  private:
    Geom2d_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE2D_H */
