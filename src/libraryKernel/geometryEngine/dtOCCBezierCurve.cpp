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

#include "dtOCCBezierCurve.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_BezierCurve.hxx>

namespace dtOO {				
	dtOCCBezierCurve::dtOCCBezierCurve() : dtOCCCurve() {
		_ptr = NULL;
	}

	dtOCCBezierCurve::dtOCCBezierCurve( 
    dtOCCCurveBase const & orig
  ) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().get(), Geom_BezierCurve const, _ptr);
	}
	
	dtOCCBezierCurve::~dtOCCBezierCurve() {
	}

  dtOCCBezierCurve * dtOCCBezierCurve::clone( void ) const {
		return new dtOCCBezierCurve( OCCRef() );
	}
	
  dtInt dtOCCBezierCurve::order( void ) const {
    return static_cast<int>(_ptr->Degree());		
	}
	
  dtInt dtOCCBezierCurve::nControlPoints( void ) const {
		return static_cast<int>(_ptr->NbPoles());		
	}
	
  dtPoint3 dtOCCBezierCurve::controlPoint( dtInt const nPoint ) const {
		Standard_Integer nPI = static_cast<Standard_Integer>(nPoint+1);		
		gp_Pnt pp = _ptr->Pole(nPI);
		
		return dtPoint3(
      static_cast<dtReal>(pp.Coord(1)), 
      static_cast<dtReal>(pp.Coord(2)), 
      static_cast<dtReal>(pp.Coord(3))
		);		
	}
	
  void dtOCCBezierCurve::setControlPoint( 
    dtInt const nPoint, dtPoint3 const point 
  ) {
		dt__throwUnexpected(setControlPoint());
	}
}
