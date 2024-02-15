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

#include "bSplineCurve2d_curveConnectConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>

#include <Precision.hxx>
#include <Geom2d_BoundedCurve.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2dConvert_CompCurveToBSplineCurve.hxx>

namespace dtOO {
	bSplineCurve2d_curveConnectConstructOCC::bSplineCurve2d_curveConnectConstructOCC( 
	  vectorHandling< dtCurve2d const * > const & cc 
	) {
		Geom2dConvert_CompCurveToBSplineCurve conv;
		dt__forAllIndex(cc, ii) {
			dt__ptrAss( dtOCCCurve2d const * occC, dtOCCCurve2d::ConstDownCast(cc[ii]) );
			Handle(Geom2d_BoundedCurve) boundedC
			=
			Handle(Geom2d_BoundedCurve)::DownCast( occC->OCCRef().getOCC() );
      conv.Add(boundedC, Precision::Confusion());//Precision::Confusion());			
    }
		dtOCCCurve2dBase base;
		base.setOCC( conv.BSplineCurve() );
		_dtC2d.reset( new dtOCCBSplineCurve2d(base) );		
	}
	
	bSplineCurve2d_curveConnectConstructOCC::~bSplineCurve2d_curveConnectConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_curveConnectConstructOCC::result( void ) {
		return _dtC2d->clone();
	}
}
