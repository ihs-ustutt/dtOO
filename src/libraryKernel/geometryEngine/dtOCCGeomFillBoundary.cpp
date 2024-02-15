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

#include "dtOCCGeomFillBoundary.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include "dtOCCGeomFillBoundaryBase.h"
#include "dtOCCCurveBase.h"
#include "progHelper.h"

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <Geom_Curve.hxx>
#include <GeomFill_Boundary.hxx>

namespace dtOO {				
	dtOCCGeomFillBoundary::dtOCCGeomFillBoundary() : dtOCCCurve() {
		_boundary.reset( new dtOCCGeomFillBoundaryBase() );
    _ptr = NULL;
	}

	dtOCCGeomFillBoundary::dtOCCGeomFillBoundary( 
    dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
  ) : dtOCCCurve(cBase) {
		_boundary.reset( new dtOCCGeomFillBoundaryBase() );		
		_boundary->setOCC( Handle(GeomFill_Boundary)::DownCast(bBase.getOCC()) );
		dt__mustCast(
      OCCBoundaryRef().getOCC().get(), GeomFill_Boundary const, _ptr
    );
	}
	
	dtOCCGeomFillBoundary::~dtOCCGeomFillBoundary() {
	}

  dtInt dtOCCGeomFillBoundary::order( void ) const {
		dt__throwUnexpected(order());		
	}
	
  bool dtOCCGeomFillBoundary::closed( void ) const {
    dt__throwUnexpected(closed());
  }
  
  dtVector3 dtOCCGeomFillBoundary::firstDer( dtReal const uu) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt pp;
		gp_Vec vv;

		dt__tryOcc(
		  _ptr->D1(uR, pp, vv);
		,
		  << dt__eval(uu)
		);
			
		return dtVector3(
			static_cast<dtReal>(vv.Coord(1)), 
			static_cast<dtReal>(vv.Coord(2)), 
			static_cast<dtReal>(vv.Coord(3))
		);
  }
      
  dtInt dtOCCGeomFillBoundary::nControlPoints( void ) const {
		return 0;
	}
	
  dtPoint3 dtOCCGeomFillBoundary::controlPoint( dtInt const nPoint ) const {
		dt__throwUnexpected(controlPoint());		
	}
	
  void dtOCCGeomFillBoundary::setControlPoint( dtInt const nPoint, dtPoint3 const point ) {
		dt__throwUnexpected(setControlPoint());
	}
  
  dtReal dtOCCGeomFillBoundary::l_u( dtReal const uu ) const {
    dt__throwUnexpected(l_u());
  }
  
  dtReal dtOCCGeomFillBoundary::u_l( dtReal const length ) const {
    dt__throwUnexpected(u_l());
  }
  
  dtReal dtOCCGeomFillBoundary::reparam(dtPoint3 const point) const {
    dt__throwUnexpected(reparam());
  }
  
  void dtOCCGeomFillBoundary::revert( void ) {
    dt__throwUnexpected(revert());
  }
  
  void dtOCCGeomFillBoundary::translate( dtVector3 const & tt ) {
    dt__throwUnexpected(translate());
  }
  
	dtOCCGeomFillBoundaryBase const & dtOCCGeomFillBoundary::OCCBoundaryRef( void ) const {
		return *(_boundary.get());
	}
  dt__C_addCloneForpVH(dtOCCGeomFillBoundary);
//	dtOCCGeomFillBoundaryBase & dtOCCGeomFillBoundary::OCCBoundaryRef( void ) {
//		return *(_boundary.get());
//	}	  
}
