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

#include "dtOCCLine.h"

#include <logMe/logMe.h>
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Line.hxx>

namespace dtOO {				
	dtOCCLine::dtOCCLine() : dtOCCCurve() {
		_ptr = NULL;
	}

	dtOCCLine::dtOCCLine( dtOCCCurveBase const & orig) : dtOCCCurve(orig) {
		dt__mustCast(OCCRef().getOCC().get(), Geom_Line const, _ptr);
	}
	
	dtOCCLine::~dtOCCLine() {
	}

  dtOCCLine * dtOCCLine::clone( void ) const {
		return new dtOCCLine( OCCRef() );
	}
	
  dtInt dtOCCLine::order( void ) const {
		dt__throwUnexpected(order());		
	}
	
  dtInt dtOCCLine::nControlPoints( void ) const {
		dt__throwUnexpected(nControlPoints());		
	}
	
  dtPoint3 dtOCCLine::controlPoint( dtInt const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCLine::setControlPoint( dtInt const nPoint, dtPoint3 const point ) {
		dt__throwUnexpected(setControlPoint());
	}
}
