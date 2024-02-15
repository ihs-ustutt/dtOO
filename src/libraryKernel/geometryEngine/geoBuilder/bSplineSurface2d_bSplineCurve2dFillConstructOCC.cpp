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

#include "bSplineSurface2d_bSplineCurve2dFillConstructOCC.h"

#include <logMe/logMe.h>
#include "bSplineSurface_bSplineCurveFillConstructOCC.h"
#include "geomCurve_baseConstructOCC.h"
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtSurface2d.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCBSplineSurface2d.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <GeomLib.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BSplineSurface.hxx>

namespace dtOO {
	bSplineSurface2d_bSplineCurve2dFillConstructOCC::bSplineSurface2d_bSplineCurve2dFillConstructOCC( 
	  vectorHandling< dtCurve2d const * > const & cc2d 
	) {
		gp_Ax2 ax2(gp_Pnt(0., 0., 0.), gp_Dir(0., 0., 1.));
		vectorHandling< dtCurve const * > cc(cc2d.size());
		dt__forAllIndex(cc2d, ii) {
			dt__ptrAss(
				dtOCCCurve2d const * occC2d, 
				dtOCCCurve2d::ConstDownCast(cc2d[ii])
			);
		  Handle(Geom_Curve) occC = GeomLib::To3d( ax2, occC2d->OCCRef().getOCC() );
			dtOCCCurveBase base;
			base.setOCC( occC );
			cc[ii] = geomCurve_baseConstructOCC(base).result();
		}
		dt__pH(dtSurface) dtS( 
		  bSplineSurface_bSplineCurveFillConstructOCC(cc).result() 
		);
		dt__ptrAss(
		  dtOCCSurface const * occS, 
			dtOCCBSplineSurface::ConstDownCast(dtS.get())
		);
		
		Handle(Geom_BSplineSurface) bS 
		= 
		Handle(Geom_BSplineSurface)::DownCast(occS->OCCRef().getOCC());
		
		dt__throwIf(bS.IsNull(), bSplineSurface2d_bSplineCurve2dFillConstructOCC());
		
		_dtS2d.reset( new dtOCCBSplineSurface2d(occS->OCCRef()) );
	}
	
	bSplineSurface2d_bSplineCurve2dFillConstructOCC::~bSplineSurface2d_bSplineCurve2dFillConstructOCC() {
	}
	
	dtSurface2d * bSplineSurface2d_bSplineCurve2dFillConstructOCC::result( void ) {
		return _dtS2d->clone();
	}
}
