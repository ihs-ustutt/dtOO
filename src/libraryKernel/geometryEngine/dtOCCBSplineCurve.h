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

#ifndef DTOCCBSPLINECURVE_H
#define	DTOCCBSPLINECURVE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_BSplineCurve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCBSplineCurve : public dtOCCCurve {
  public:
    dt__class(dtOCCBSplineCurve, dtCurve);
    dtOCCBSplineCurve();
    dtOCCBSplineCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCBSplineCurve();
    virtual dtOCCBSplineCurve * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point );
    virtual std::string dumpToString(void) const;
  private:
    Geom_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE_H */
