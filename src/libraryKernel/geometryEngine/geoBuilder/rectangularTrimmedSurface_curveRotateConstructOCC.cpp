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

#include "rectangularTrimmedSurface_curveRotateConstructOCC.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCRectangularTrimmedSurface.h>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>


namespace dtOO {
  rectangularTrimmedSurface_curveRotateConstructOCC::rectangularTrimmedSurface_curveRotateConstructOCC( 
	  dtCurve const & cL, 
    dtPoint3 const & oP, 					
		dtVector3 const & rA, 
		dtReal const & angle 
	) {
		dt__ptrAss(dtOCCCurve const * bC, dtOCCCurve::ConstDownCast( &cL ));
		
	  gp_Pnt pp(
		  static_cast<Standard_Real>(oP.x()), 
			static_cast<Standard_Real>(oP.y()),
			static_cast<Standard_Real>(oP.z()) 
		);
		gp_Dir dir(
		  static_cast<Standard_Real>(rA.x()), 
			static_cast<Standard_Real>(rA.y()),
			static_cast<Standard_Real>(rA.z()) 
		);		
	  gp_Ax1 rotAx(pp, dir);
		Handle(Geom_SurfaceOfRevolution) sor 
		= 
		new Geom_SurfaceOfRevolution( bC->OCCRef().getOCC(), rotAx);

		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;
		sor->Bounds(U1, U2, V1, V2);
		U2 = (U2-U1) * (static_cast<Standard_Real>(angle)/(2.*M_PI));
		
		Handle(Geom_RectangularTrimmedSurface) rts
		=
		new Geom_RectangularTrimmedSurface(sor, U1, U2, V1, V2);
		
		dtOCCSurfaceBase base;
		base.setOCC(rts);

		_dtS.reset( new dtOCCRectangularTrimmedSurface(base) );
	}
	
  rectangularTrimmedSurface_curveRotateConstructOCC::~rectangularTrimmedSurface_curveRotateConstructOCC() {
		
	}
	
  dtSurface * rectangularTrimmedSurface_curveRotateConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
